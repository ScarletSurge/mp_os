#include <not_implemented.h>
#include "../include/allocator_boundary_tags.h"


allocator_boundary_tags::~allocator_boundary_tags()
{
    debug_with_guard(get_typename() + " START: destructor")->debug_with_guard(get_typename() + " END: destructor");
    allocator::destruct(get_mutex());
    deallocate_with_guard(_trusted_memory);
}

allocator_boundary_tags::allocator_boundary_tags(allocator_boundary_tags &&other) noexcept : _trusted_memory(other._trusted_memory)
{
    other._trusted_memory = nullptr;
}

allocator_boundary_tags &allocator_boundary_tags::operator=(allocator_boundary_tags &&other) noexcept
{
    if(this != &other)
    {
        deallocate_with_guard(_trusted_memory);
        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }
    return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
        size_t space_size,
        allocator *parent_allocator,
        logger *logger,
        allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->debug(get_typename() + "START: constructor");
    }
    size_t common_size = space_size + get_ancillary_space_size();
    try
    {
        _trusted_memory = parent_allocator == nullptr
                          ? ::operator new(common_size)
                          : parent_allocator->allocate(common_size, 1);
    }
    catch (std::bad_alloc const &ex)
    {
        logger->error("failed to allocate memory");
        throw std::bad_alloc();
    }

    allocator** parent_allocator_space_address = reinterpret_cast<allocator **>(_trusted_memory);
    *parent_allocator_space_address = parent_allocator;

    class logger** logger_space_address = reinterpret_cast<class logger**>(parent_allocator_space_address + 1);
    *logger_space_address = logger;

    size_t *space_size_space_address = reinterpret_cast<size_t *>(logger_space_address + 1);
    *space_size_space_address = space_size;

    size_t *free_memory = reinterpret_cast<size_t *>(space_size_space_address + 1);
    *free_memory = space_size;

    allocator_with_fit_mode::fit_mode *fit_mode_space_address = reinterpret_cast<allocator_with_fit_mode::fit_mode *>(free_memory + 1);
    *fit_mode_space_address = allocate_fit_mode;

    std::mutex** mutex_space_address = reinterpret_cast<std::mutex**>(fit_mode_space_address + 1);
    *mutex_space_address = new std::mutex;

    void **first_block_address_space_address = reinterpret_cast<void **>(mutex_space_address + 1);
    *first_block_address_space_address = nullptr;
    if (logger != nullptr)
    {
        logger->debug(get_typename() + "END: constructor");
    }

}

[[nodiscard]] void *allocator_boundary_tags::allocate(
        size_t value_size,
        size_t values_count)
{
    debug_with_guard(get_typename() + " " + "START: allocate");
    std::mutex* mutex = get_mutex();
    std::lock_guard<std::mutex> lock(*mutex);

    size_t* free_size = get_free_size();
    auto requested_size = value_size * values_count + get_metadata_size_of_block();
    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();

    void *target_block = nullptr;
    void *previous_to_target_block = nullptr;
    void *next_to_target_block = nullptr;
    size_t current_size = 0;

    {
        void *previous_block = nullptr;
        void *current_block = get_first_occupied_block();
        if (current_block == nullptr)
        {
            unsigned char* start_of_memory = reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size();
            unsigned char* end_of_memory = reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size() + get_full_size();
            size_t available_size = end_of_memory - start_of_memory;

            if (available_size >= requested_size)
            {
                target_block = reinterpret_cast<void*>(start_of_memory);
                current_size = available_size;
            }
        }
        else
        {
            while (current_block != nullptr)
            {
                unsigned char* start_of_memory = nullptr;
                unsigned char* end_of_memory = reinterpret_cast<unsigned char*>(current_block);

                if (previous_block == nullptr)
                {
                    start_of_memory = reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size();
                }
                else
                {
                    start_of_memory = reinterpret_cast<unsigned char*>(previous_block) + get_metadata_size_of_block() + get_size_of_block(previous_block);
                }
                size_t current_block_size = end_of_memory - start_of_memory;
                if (current_block_size >= requested_size &&
                    (fit_mode == allocator_with_fit_mode::fit_mode::first_fit ||
                     fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit &&
                     (target_block == nullptr || current_size > current_block_size) ||
                     fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit && (target_block == nullptr ||
                                                                                      current_size <
                                                                                      current_block_size)))
                {
                    previous_to_target_block = previous_block;
                    target_block = reinterpret_cast<void*>(start_of_memory);
                    current_size = current_block_size;
                    next_to_target_block = get_next_occupied_block(current_block);
                }
                previous_block = current_block;
                current_block = get_next_occupied_block(current_block);
            }
            unsigned char* start_of_memory = reinterpret_cast<unsigned char*>(previous_block) + get_metadata_size_of_block() + get_size_of_block(previous_block);
            unsigned char* end_of_memory = reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size() + get_full_size();
            size_t current_block_size = end_of_memory - start_of_memory;

            if (current_block_size >= requested_size &&
                (fit_mode == allocator_with_fit_mode::fit_mode::first_fit ||
                 fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit &&
                 (target_block == nullptr || current_size > current_block_size) ||
                 fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit && (target_block == nullptr ||
                                                                                  current_size <
                                                                                  current_block_size)))
            {
                previous_to_target_block = previous_block;
                target_block = reinterpret_cast<void*>(start_of_memory);
                current_size = current_block_size;
                next_to_target_block = nullptr;
            }

        }
    }
    if (target_block == nullptr)
    {
        error_with_guard(get_typename() + "failed to allocate memory");
        throw std::bad_alloc();
    }

    if (get_first_occupied_block() == nullptr)
    {
        size_t* size_block = reinterpret_cast<size_t*>(target_block);
        size_t difference = current_size - requested_size;
        if (difference < get_metadata_size_of_block())
        {
            requested_size = current_size;
        }
        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*)) = target_block;

        *size_block = requested_size - get_metadata_size_of_block();
        void** ptr_to_previous = reinterpret_cast<void**>(size_block + 1);
        void** ptr_to_next = reinterpret_cast<void**>(ptr_to_previous + 1);
        void** ptr_to_trusted_memory = reinterpret_cast<void**>(ptr_to_next + 1);

        *ptr_to_previous = nullptr;
        *ptr_to_next = nullptr;
        *ptr_to_trusted_memory = _trusted_memory;
    }
    else
    {
        size_t difference = current_size - requested_size;
        if (difference < get_metadata_size_of_block())
        {
            requested_size = current_size;
        }
        size_t* size_block = reinterpret_cast<size_t*>(target_block);
        *size_block = requested_size - get_metadata_size_of_block();

        void** ptr_to_previous = reinterpret_cast<void**>(size_block + 1);
        void** ptr_to_next = reinterpret_cast<void**>(ptr_to_previous + 1);
        void** ptr_to_trusted_memory = reinterpret_cast<void**>(ptr_to_next + 1);

        *ptr_to_previous = previous_to_target_block;
        *ptr_to_next = next_to_target_block;
        *ptr_to_trusted_memory = _trusted_memory;

        if(next_to_target_block != nullptr)
        {
            void** ptr_next_to_prev = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(next_to_target_block) + sizeof(size_t));
            *ptr_next_to_prev = target_block;
        }
        if (previous_to_target_block != nullptr)
        {
            void** ptr_prev_to_next = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous_to_target_block) + sizeof(size_t) + sizeof(void*));
            *ptr_prev_to_next = target_block;
        }

    }
    *free_size -=  requested_size;
    std::vector<allocator_test_utils::block_info> data = get_blocks_info();
    std::string data_str;
    for (block_info value : data)
    {
        std::string is_oc = value.is_block_occupied ? "YES" : "NO";
        data_str += (is_oc + "  " + std::to_string(value.block_size) + " | ");
    }
    debug_with_guard(get_typename() + "END: allocate");
    debug_with_guard(get_typename() + "state blocks: " + data_str);
    debug_with_guard("free size: " + std::to_string(*free_size));
    return reinterpret_cast<unsigned char*>(target_block) + get_metadata_size_of_block();
}

void allocator_boundary_tags::deallocate(
        void *at)
{
    debug_with_guard(get_typename() + " " + "START: deallocate");
    std::mutex* mutex = get_mutex();
    std::lock_guard<std::mutex> lock(*mutex);

    size_t* free_size = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t));
    void* ptr_to_start = *reinterpret_cast<void**>(reinterpret_cast<char*>(at) - sizeof(void*));
    if (ptr_to_start != _trusted_memory)
    {
        error_with_guard(get_typename() + "block doesnt belong to this allocator");
        throw std::logic_error("block doesnt belong to this instance");
    }

    void *target_block = reinterpret_cast<void*>(reinterpret_cast<char *>(at) - sizeof(void*) - sizeof(void*) - sizeof(void*) - sizeof(size_t));
    size_t size_object = *reinterpret_cast<size_t*>(target_block);
    std::string result = get_state(target_block, size_object);
    debug_with_guard("state block: " + result);
    std::cout << result << std::endl;


    void* prev_occup_block = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(target_block) + sizeof(size_t));
    void* current_occup = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(target_block) + sizeof(size_t) + sizeof(void*));
    if (get_first_occupied_block() == target_block)
    {
        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*)) = current_occup;
    }
    if (prev_occup_block != nullptr)
    {
        size_t* size_ = reinterpret_cast<size_t*>(prev_occup_block);
        void** ptr_prev_ = reinterpret_cast<void**>(size_ + 1);
        void** ptr_prev_to_next = reinterpret_cast<void**>(ptr_prev_ + 1);
        *ptr_prev_to_next = current_occup;
    }
    if (current_occup != nullptr)
    {
        void** ptr_next_to_prev = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(current_occup) + sizeof(size_t));
        *ptr_next_to_prev = prev_occup_block;
    }
    std::vector<allocator_test_utils::block_info> data = get_blocks_info();
    std::string data_str;
    for (block_info value : data)
    {
        std::string is_oc = value.is_block_occupied ? "YES" : "NO";
        data_str += (is_oc + "  " + std::to_string(value.block_size) + " | ");
    }
    debug_with_guard(get_typename() + " END: deallocate");
    debug_with_guard(get_typename() + "state blocks: " + data_str);
    (*free_size) += (size_object + get_metadata_size_of_block());
    debug_with_guard(get_typename() + "free size: " + std::to_string(*free_size));
}

inline void allocator_boundary_tags::set_fit_mode(allocator_with_fit_mode::fit_mode mode)
{
    *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t)) = mode;

}

inline allocator *allocator_boundary_tags::get_allocator() const
{
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
    debug_with_guard(get_typename() + "START: get_blocks_info");
    std::vector<allocator_test_utils::block_info> data;
    void** ans = reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(std::mutex*) + sizeof(allocator_with_fit_mode::fit_mode));
    void* first_block = reinterpret_cast<void **>(ans + 1);

    void* current_occup = get_first_occupied_block();
    if (current_occup == nullptr)
    {
        allocator_test_utils::block_info obj;
        obj.is_block_occupied = 0;
        obj.block_size = get_full_size();
        data.push_back(obj);
        return data;
    }
    else
    {
        void* prev_block_occup = nullptr;
        if (current_occup == first_block)
        {
            allocator_test_utils::block_info obj;
            obj.block_size = *reinterpret_cast<size_t*>(current_occup);
            obj.is_block_occupied = 1;
            prev_block_occup = current_occup;
            current_occup = get_next_occupied_block(current_occup);
            data.push_back(obj);
        }
        else
        {
            allocator_test_utils::block_info obj1;
            unsigned char* start = reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size();
            unsigned char* end = reinterpret_cast<unsigned char*>(current_occup);
            if (end - start > 0)
            {
                obj1.is_block_occupied = 0;
                obj1.block_size = end - start;
                data.push_back(obj1);
            }
            allocator_test_utils::block_info obj2;
            obj2.block_size = *reinterpret_cast<size_t*>(current_occup);
            obj2.is_block_occupied = 1;
            data.push_back(obj2);
            prev_block_occup = current_occup;
            current_occup = get_next_occupied_block(current_occup);
        }
        while (current_occup != nullptr)
        {
            allocator_test_utils::block_info obj1;
            unsigned char* start = reinterpret_cast<unsigned char*>(prev_block_occup) + get_metadata_size_of_block() + *reinterpret_cast<size_t*>(prev_block_occup);
            unsigned char* end = reinterpret_cast<unsigned char*>(current_occup);
            if (end - start > 0)
            {
                obj1.is_block_occupied = 0;
                obj1.block_size = end - start;
                data.push_back(obj1);
            }
            allocator_test_utils::block_info obj2;
            obj2.block_size = *reinterpret_cast<size_t*>(current_occup);
            obj2.is_block_occupied = 1;
            data.push_back(obj2);
            prev_block_occup = current_occup;
            current_occup = get_next_occupied_block(current_occup);
        }
        unsigned char* end =  reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size() + get_full_size();
        unsigned char* start = reinterpret_cast<unsigned char*>(prev_block_occup) + get_metadata_size_of_block() + *reinterpret_cast<size_t*>(prev_block_occup);
        if (start != end)
        {
            allocator_test_utils::block_info obj;
            obj.block_size = end - start;
            obj.is_block_occupied = 0;
            data.push_back(obj);
        }
    }
    debug_with_guard(get_typename() + "END: blocks_info");
    return data;
}

inline logger *allocator_boundary_tags::get_logger() const
{
    return *reinterpret_cast<logger **>(reinterpret_cast<allocator **>(_trusted_memory) + 1);
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return "allocator_boundary_tags";
}

size_t allocator_boundary_tags::get_ancillary_space_size() const
{
    return sizeof(size_t) + sizeof(allocator*) + sizeof(logger*) + sizeof(void*) + sizeof(size_t) + sizeof(std::mutex*) + sizeof(allocator_with_fit_mode::fit_mode);
}

size_t allocator_boundary_tags::get_metadata_size_of_block() const
{
    return sizeof(size_t) + 3 * sizeof(void*);
}


void* allocator_boundary_tags::get_first_occupied_block() const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*));
}

allocator_with_fit_mode::fit_mode allocator_boundary_tags::get_fit_mode()
{
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t));
}

size_t allocator_boundary_tags::get_full_size() const
{
    return *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*));
}

void* allocator_boundary_tags::get_next_occupied_block(void* target_block) const noexcept
{
    return  *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(target_block) + sizeof(size_t) + sizeof(void*));
}

std::mutex* allocator_boundary_tags::get_mutex() const noexcept
{
    return *reinterpret_cast<std::mutex**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode));
}

size_t* allocator_boundary_tags::get_free_size() const noexcept
{
    return reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t));
}

size_t allocator_boundary_tags::get_size_of_block(void *target_block) const noexcept
{
    return *reinterpret_cast<size_t*>(target_block);
}

std::string allocator_boundary_tags::get_state(void *at, size_t size_object) const noexcept
{
    std::string result;
    auto* bytes = reinterpret_cast<unsigned char*>(at) + sizeof(size_t) + 3 * sizeof(void*);
    for (int i = 0; i < size_object; i++)
    {
        result += std::to_string(static_cast<int>(bytes[i])) + " ";
    }
    return result;
}