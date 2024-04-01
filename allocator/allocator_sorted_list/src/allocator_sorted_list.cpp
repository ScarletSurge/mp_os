#include <not_implemented.h>
#include "../include/allocator_sorted_list.h"
#include <vector>
#include <chrono>
#include <mutex>
#include <thread>
allocator_sorted_list::~allocator_sorted_list()
{
    allocator* parent_allocator = get_allocator();
    debug_with_guard(get_typename() + " "+ "START: ~allocator_sorted_list()");
    debug_with_guard(get_typename() + " " + "END: ~allocator_sorted_list()");
    if(parent_allocator != nullptr)
    {
        parent_allocator->deallocate(_trusted_memory);
    }
    else
    {
        ::operator delete(_trusted_memory);
    }
}

allocator_sorted_list::allocator_sorted_list(
        size_t space_size,
        allocator *parent_allocator,
        logger *logger,
        allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->debug(get_typename() + " START: constructor");
    }
    if (space_size < sizeof(block_pointer_t) + sizeof(block_size_t))
    {
        if (logger != nullptr)
        {
            logger->error("too small size\n");
        }
        throw std::logic_error("too small size");
    }

    auto common_size = space_size + get_ancillary_space_size();

    try
    {
        _trusted_memory = parent_allocator == nullptr
                          ? ::operator new(common_size)
                          : parent_allocator->allocate(common_size, 1);
    }
    catch (std::bad_alloc const &ex)
    {
        logger->error("can't get memory");
        throw std::bad_alloc();
    }

    allocator** parent_allocator_space_address = reinterpret_cast<allocator **>(_trusted_memory);
    *parent_allocator_space_address = parent_allocator;

    class logger** logger_space_address = reinterpret_cast<class logger**>(parent_allocator_space_address + 1);
    *logger_space_address = logger;

    size_t *space_size_space_address = reinterpret_cast<size_t *>(logger_space_address + 1);
    *space_size_space_address = space_size;

    size_t *size_of_free_memory = reinterpret_cast<size_t *>(space_size_space_address + 1);
    *size_of_free_memory = space_size;

    allocator_with_fit_mode::fit_mode *fit_mode_space_address = reinterpret_cast<allocator_with_fit_mode::fit_mode *>(size_of_free_memory + 1);
    *fit_mode_space_address = allocate_fit_mode;

    std::mutex** mutex_space_address = reinterpret_cast<std::mutex** >(fit_mode_space_address + 1);
    *mutex_space_address = new std::mutex;

    void **first_block_address_space_address = reinterpret_cast<void **>(mutex_space_address + 1);
    *first_block_address_space_address = reinterpret_cast<void *>(first_block_address_space_address + 1);
    auto metadata = sizeof(void*) + sizeof(size_t);

    *reinterpret_cast<size_t *>(*first_block_address_space_address) = space_size - metadata;
    *reinterpret_cast<void** >(reinterpret_cast<size_t *>(*first_block_address_space_address) + 1) = nullptr;

    if (logger != nullptr)
    {
        logger->debug(get_typename() + " END: constructor. Allocator was created");
    }
}

[[nodiscard]] void *allocator_sorted_list::allocate(size_t value_size,size_t values_count)
{
    debug_with_guard(get_typename() + " "+ "START: allocate");
    std::mutex* mutex = get_mutex();
    //захватываем мьютекс
    std::unique_lock<std::mutex> lock(*mutex);
    //если мьютекс захвачен другим потоком поток будет ждать 1 секунду перед повторной попыткой захвата
    while(!lock.owns_lock())
    {
        //задерживаем поток на 1 секунду перед следующей попыткой захвата мьютекса
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    auto requested_size = value_size * values_count;
    if (requested_size < sizeof(block_pointer_t) + sizeof(block_size_t))
    {
        requested_size = sizeof(block_pointer_t) + sizeof(block_size_t);
        warning_with_guard(get_typename() + "requested size was changed");
    }
    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();
    void *target_block = nullptr;
    void *previous_to_target_block = nullptr;

    {
        void *previous_block = nullptr;
        void *current_block = get_first_aviable_block();
        while (current_block != nullptr)
        {
            size_t current_block_size = get_aviable_block_size(current_block);
            if (current_block_size >= requested_size &&
                (fit_mode == allocator_with_fit_mode::fit_mode::first_fit ||
                 fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit &&
                 (target_block == nullptr || get_aviable_block_size(target_block) > current_block_size) ||
                 fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit && (target_block == nullptr ||
                                                                                  get_aviable_block_size(target_block) <
                                                                                  current_block_size)))
            {
                previous_to_target_block = previous_block;
                target_block = current_block;
            }
            previous_block = current_block;
            current_block = get_aviable_block_next_block_address(current_block);
        }
    }
    if (target_block == nullptr)
    {
        error_with_guard(get_typename() + "failed to allocate memory");
        lock.unlock();
        throw std::bad_alloc();
    }
    if (previous_to_target_block == nullptr)
    {
        auto blocks_sizes_difference = *reinterpret_cast<size_t*>(target_block) - requested_size;
        void **address_of_next_available_block = reinterpret_cast<void**>(reinterpret_cast<size_t *>(target_block) + 1);
        if (blocks_sizes_difference > 0 && blocks_sizes_difference < sizeof(block_pointer_t) + sizeof(block_size_t))
        {

            requested_size = get_aviable_block_size(target_block);
            void *next_free_block = get_aviable_block_next_block_address(target_block);
            void** first_available_block = reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + get_ancillary_space_size());
            *first_available_block = next_free_block;
            warning_with_guard(get_typename() + "requested size was changed");
        }
        else
        {
            auto metadata = sizeof(void*) + sizeof(size_t);
            auto size_target_block = get_aviable_block_size(target_block);
            *reinterpret_cast<size_t *>(target_block) = requested_size;
            void *next_free_block = get_aviable_block_next_block_address(target_block);

            void *new_block = reinterpret_cast<unsigned char *>(target_block) + metadata + requested_size;
            size_t *size_space_new_block = reinterpret_cast<size_t *>(new_block);
            auto **next_free_new_block = reinterpret_cast<void **>(size_space_new_block + 1);

            *size_space_new_block = size_target_block - requested_size - sizeof(size_t) - sizeof(void*);
            *next_free_new_block = next_free_block;

            void** first_available_block = reinterpret_cast<void **>
            (reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(std::mutex*) + sizeof(allocator_with_fit_mode::fit_mode));
            *first_available_block = new_block;
        }

        *address_of_next_available_block = _trusted_memory;
    }
    else
    {
        void **address_to_next_free_block = reinterpret_cast<void**>(reinterpret_cast<size_t *>(target_block) + 1);
        auto blocks_sizes_difference = get_aviable_block_size(target_block) - requested_size;
        if (blocks_sizes_difference > 0 && blocks_sizes_difference < sizeof(block_pointer_t) + sizeof(block_size_t))
        {
            warning_with_guard(get_typename() + "requested size was changes");
            requested_size = get_aviable_block_size(target_block);
            void** ptr_prev_block_to_this = reinterpret_cast<void**>(reinterpret_cast<size_t*>(previous_to_target_block) + 1);
            void* ptr_this_next = get_aviable_block_next_block_address(target_block);
            *ptr_prev_block_to_this = ptr_this_next;
        }
        else
        {
            void** ptr_prev_block_to_this = reinterpret_cast<void**>(reinterpret_cast<size_t*>(previous_to_target_block) + 1);
            void* ptr_this_next = get_aviable_block_next_block_address(target_block);
            auto size_target_block = *reinterpret_cast<size_t*>(target_block);
            size_t* size_target_block_ptr = reinterpret_cast<size_t*>(target_block);
            *size_target_block_ptr = requested_size;
            void *new_block = reinterpret_cast<char *>(target_block) + sizeof(size_t) + sizeof(void*) + requested_size;
            *reinterpret_cast<size_t*>(new_block) = size_target_block - requested_size - sizeof(size_t) - sizeof(void*);
            *reinterpret_cast<void**>(reinterpret_cast<size_t*>(new_block) + 1) = ptr_this_next;
            *ptr_prev_block_to_this = new_block;
        }
        *address_to_next_free_block = _trusted_memory;
    }
    size_t size_before = *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *));
    size_t* size_space = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *));
    *size_space = size_before - requested_size - sizeof(size_t) - sizeof(void*);
    information_with_guard(get_typename() + " available summ size" + std::to_string(*size_space));
    std::vector<allocator_test_utils::block_info> information = get_blocks_info();
    std::string result;
    for (block_info value : information)
    {
        std::string is_occupied = value.is_block_occupied ? "YES" : "NO";
        result += (is_occupied + "  " + std::to_string(value.block_size) + " | ");
    }
    debug_with_guard(get_typename()  + "blocks state: " + result);
    debug_with_guard(get_typename() + " END: deallocate");
    lock.unlock();
    return reinterpret_cast<char *>(target_block) + sizeof(size_t) + sizeof(void*);
}

void allocator_sorted_list::deallocate(void *at)
{
    std::mutex* mutex = get_mutex();
    std::unique_lock<std::mutex> lock(*mutex);
    while(!lock.owns_lock())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (at == nullptr)
    {
        return;
    }
    void *target_block = reinterpret_cast<void*>(reinterpret_cast<char *>(at) - sizeof(void*) - sizeof(size_t));
    auto size_target_block = *reinterpret_cast<size_t*>(target_block);
    debug_with_guard(get_typename() + "START: deallocate");
    std::string result;
    auto *size_space = reinterpret_cast<size_t *>(target_block);
    auto *ptr = reinterpret_cast<void *>(size_space + 1);
    auto* bytes = reinterpret_cast<unsigned char*>(ptr) + 1;

    size_t size_of_occupied_block = get_occupied_block_size(target_block);

    for (int i = 0; i < size_of_occupied_block; i++)
    {
        result += std::to_string(static_cast<int>(bytes[i])) + " ";
    }
    debug_with_guard(get_typename() + "state of block" + result);

    size_t size_before = *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *));
    size_t* summ_size = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *));
    if (*reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_block) + 1) != _trusted_memory)
    {
        error_with_guard(get_typename() + " this block doesn't belong to current allocator!");
        lock.unlock();
        throw std::logic_error("this block doesn't belong to current allocator!");
    }
    void *current_free_block = get_first_aviable_block();
    void *prev_free_block = nullptr;
    while (current_free_block != nullptr &&
           current_free_block < target_block)
    {
        prev_free_block = current_free_block;
        current_free_block = get_aviable_block_next_block_address(current_free_block);
    }

    if (prev_free_block == nullptr && current_free_block != nullptr)
    {
        void **ptr_to_first_avail_block = reinterpret_cast<void**>( reinterpret_cast<unsigned char*>(_trusted_memory)
                                                                    + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(std::mutex*) + sizeof(allocator_with_fit_mode::fit_mode));
        *ptr_to_first_avail_block = target_block;
        void** ptr_target_block_to_next = reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_block) + 1);
        *ptr_target_block_to_next = current_free_block;
        *summ_size = size_before + get_occupied_block_size(target_block);
        size_before = *summ_size;
        if (reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(target_block) + size_target_block + sizeof(size_t) + sizeof(void*)) == current_free_block )
        {
            *reinterpret_cast<size_t*>(target_block) = size_target_block + *reinterpret_cast<size_t*>(current_free_block) + sizeof(size_t) + sizeof(void*);
            *reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_block) + 1) = *reinterpret_cast<void**>(reinterpret_cast<size_t*>(current_free_block) + 1);
            *summ_size = size_before + sizeof(size_t) + sizeof(void*);
        }
    }
    else if (prev_free_block != nullptr && current_free_block != nullptr)
    {
        void** ptr_prev_to_next = reinterpret_cast<void**>(reinterpret_cast<size_t*>(prev_free_block) + 1);
        *ptr_prev_to_next = target_block;
        void** ptr_target_block_to_next = reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_block) + 1);
        *ptr_target_block_to_next = current_free_block;
        *summ_size = size_before + get_occupied_block_size(target_block);
        size_before = *summ_size;
        if (reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(target_block) + size_target_block + sizeof(size_t) + sizeof(void*)) == current_free_block )
        {
            *reinterpret_cast<size_t*>(target_block) = size_target_block + *reinterpret_cast<size_t*>(current_free_block) + sizeof(size_t) + sizeof(void*);
            *reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_block) + 1) = *reinterpret_cast<void**>(reinterpret_cast<size_t*>(current_free_block) + 1);
            *summ_size = size_before + sizeof(size_t) + sizeof(void*);
        }
        auto size_prev_block = get_aviable_block_size(prev_free_block);
        if ( reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(prev_free_block) + size_prev_block + sizeof(size_t) + sizeof(void*)) == target_block )
        {
            *reinterpret_cast<size_t*>(prev_free_block) = size_prev_block + size_target_block + sizeof(size_t) + sizeof(void*);
            *reinterpret_cast<void**>(reinterpret_cast<size_t*>(prev_free_block) + 1) = get_aviable_block_next_block_address(target_block);
            *summ_size = size_before + sizeof(size_t) + sizeof(void*);
        }
    }
    else if (prev_free_block != nullptr && current_free_block == nullptr)
    {
        void** ptr_prev_to_next = reinterpret_cast<void**>(reinterpret_cast<size_t*>(prev_free_block) + 1);
        *ptr_prev_to_next = target_block;
        void** ptr_target_block_to_next = reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_block) + 1);
        *ptr_target_block_to_next = nullptr;

        auto size_prev_block = get_aviable_block_size(prev_free_block);
        *summ_size = size_before + get_occupied_block_size(target_block);
        size_before = *summ_size;
        if(reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(prev_free_block) + size_prev_block + sizeof(size_t) + sizeof(void*)) == target_block)
        {
            *reinterpret_cast<size_t*>(prev_free_block) = size_prev_block + size_target_block + sizeof(size_t) + sizeof(void*);
            *reinterpret_cast<void**>(reinterpret_cast<size_t*>(prev_free_block) + 1) = *reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_block) + 1);
            *summ_size = size_before + sizeof(size_t) + sizeof(void*);
        }
    }
    else
    {
        void **ptr_to_first_avail_block = reinterpret_cast<void**>( reinterpret_cast<unsigned char*>(_trusted_memory)
                                                                    + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(std::mutex*) + sizeof(allocator_with_fit_mode::fit_mode));
        *ptr_to_first_avail_block = target_block;
        void** ptr_target_block_to_next = reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_block) + 1);
        *ptr_target_block_to_next = nullptr;
        *summ_size = size_before + get_occupied_block_size(target_block);
    }
    information_with_guard(get_typename() + " free summ size");
    std::vector<allocator_test_utils::block_info> data = get_blocks_info();
    std::string data_str;
    for (block_info value : data)
    {
        std::string is_oc = value.is_block_occupied ? "YES" : "NO";
        data_str += (is_oc + "  " + std::to_string(value.block_size) + " | ");
    }
    debug_with_guard(get_typename() + "state of blocks " + data_str);
    lock.unlock();
}

inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode mode)
{
    debug_with_guard(get_typename() + " START: set_fit_mode");
    *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t)) = mode;
}

size_t allocator_sorted_list::get_ancillary_space_size() const noexcept
{
    return sizeof(logger *) + sizeof(allocator *) + sizeof(size_t) +  sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode)  + sizeof(std::mutex*) + sizeof(void *);
}

allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const noexcept
{
    trace_with_guard(get_typename() + " START: get_fit_mode()");
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t));
}

void *allocator_sorted_list::get_first_aviable_block() const noexcept
{
    trace_with_guard(get_typename() + " START: get_first_aviable_block()");
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*));
}

allocator::block_size_t allocator_sorted_list::get_aviable_block_size(void *block_address) noexcept
{
    trace_with_guard(get_typename() + " START: get_aviable_block_size");
    return *reinterpret_cast<size_t *>(block_address);
}

void *allocator_sorted_list::get_aviable_block_next_block_address(void *block_address) noexcept
{
    trace_with_guard(get_typename() + " START: get_aviable_block_next_block_address");
    return *reinterpret_cast<void**>(reinterpret_cast<size_t *>(block_address) + 1);
}

allocator::block_size_t allocator_sorted_list::get_occupied_block_size(void *block_address) noexcept
{
    trace_with_guard(get_typename() + " START: get_occupied_block_size");
    return *reinterpret_cast<allocator::block_size_t *>(block_address);
}

inline allocator *allocator_sorted_list::get_allocator() const
{
    debug_with_guard(get_typename() + " START: get_allocator()");
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    debug_with_guard(get_typename() + " START: get_blocks_info()");

    std::vector<allocator_test_utils::block_info> data;
    void** ans = reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(std::mutex*) + sizeof(allocator_with_fit_mode::fit_mode));
    void* cur_block = reinterpret_cast<void **>(ans + 1);

    size_t current_size = 0;
    size_t size_trusted_memory = *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t));

    while(current_size < size_trusted_memory)
    {
        allocator_test_utils::block_info value;
        value.block_size = *reinterpret_cast<size_t*>(cur_block);
        void* ptr_next_or_memory =*reinterpret_cast<void**>(reinterpret_cast<size_t *>(cur_block) + 1);
        value.is_block_occupied = (ptr_next_or_memory == _trusted_memory) ? true : false;
        data.push_back(value);
        cur_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char *>(cur_block) + sizeof(size_t) + sizeof(void*) + value.block_size);
        current_size = current_size + sizeof(size_t) + sizeof(void* ) + value.block_size;
    }

    return data;
}


inline logger *allocator_sorted_list::get_logger() const
{
    return *reinterpret_cast<logger **>(reinterpret_cast<allocator **>(_trusted_memory) + 1);
}

std::mutex *allocator_sorted_list::get_mutex() const noexcept
{
    trace_with_guard(get_typename() + " START: get_mutex()");
    trace_with_guard(get_typename() + " END: get_mutex()");
    return *reinterpret_cast<std::mutex **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t)
                                            + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode));;
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    std::string name = "allocator_sorted_list";
    return name;
}

