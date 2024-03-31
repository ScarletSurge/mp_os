#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"
//TODO  занятом хранить размер блока и адрес откуда взяли, чтобы потом проверить на то откуда брали
allocator_sorted_list::~allocator_sorted_list()
{
   auto* parent_allocator = get_allocator();
   if(parent_allocator == nullptr)
   {
       ::operator delete(_trusted_memory);
   }
   else
   {
       parent_allocator->deallocate(_trusted_memory);
   }
}


//allocator_sorted_list::allocator_sorted_list(allocator_sorted_list const &other) : _trusted_memory(other._trusted_memory)
//{
//
//}
//
//allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list const &other)
//{
//    if(this != &other)
//    {
//        if(_trusted_memory)
//        {
//            ::operator delete(_trusted_memory);
//        }
//        _trusted_memory = other._trusted_memory;
//    }
//    return *this;
//}
//
//
//allocator_sorted_list::allocator_sorted_list(allocator_sorted_list &&other) noexcept : _trusted_memory(other._trusted_memory)
//{
//    other._trusted_memory = nullptr;
//}
//
//allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list &&other) noexcept
//{
//    if(this != &other)
//    {
//        if(_trusted_memory)
//        {
//            ::operator delete(_trusted_memory);
//        }
//        _trusted_memory = other._trusted_memory;
//        other._trusted_memory = nullptr;
//    }
//
//    return *this;
//}

allocator_sorted_list::allocator_sorted_list(
        size_t space_size,
        allocator *parent_allocator,
        logger *logger,
        allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if(logger != nullptr)
    {
        logger->debug(get_typename() + "START: constructor");
    }

    if (space_size < sizeof(block_size_t) + sizeof(block_pointer_t))
    {

        if (logger != nullptr)
        {
            logger->error("can't initialize allocator instance due to small requested size!");
        }
        throw std::logic_error("can't initialize allocator instance due to small requested size!");
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
        logger->error("failed to allocate memory");
        throw std::bad_alloc();
    }
    //1     1allocator* 2logger* 3space_size_space_adress 4sizeof_memory  5fir_mode 6mutex
    allocator **parent_allocator_space_address = reinterpret_cast<allocator **>(_trusted_memory);
    *parent_allocator_space_address = parent_allocator;
    //2
    class logger **logger_space_address = reinterpret_cast<class logger **>(parent_allocator_space_address + 1);
    *logger_space_address = logger;
    //3 размер всей памяти
    size_t *space_size_space_address = reinterpret_cast<size_t *>(logger_space_address + 1);
    *space_size_space_address = space_size;
    //перед освобождением
    size_t* size_of_memory = reinterpret_cast<size_t*>(space_size_space_address + 1);
    *size_of_memory = space_size;
    //4
    allocator_with_fit_mode::fit_mode *fit_mode_space_address = reinterpret_cast<allocator_with_fit_mode::fit_mode *>(space_size_space_address + 1);
    *fit_mode_space_address = allocate_fit_mode;
    //5
    std::mutex** mutex_space_adress = reinterpret_cast<std::mutex**>(fit_mode_space_address + 1);
    auto* mutex = new std::mutex;
    *mutex_space_adress = mutex;
    //6
    void **first_block_address_space_address = reinterpret_cast<void **>(mutex_space_adress + 1);
    *first_block_address_space_address = reinterpret_cast<void *>(first_block_address_space_address + 1);

    //положили сначала размер всей памяти
    *reinterpret_cast<size_t*>(*first_block_address_space_address) = space_size - sizeof(size_t) - sizeof(void*);
    //сказали что следующий блок нал всё аллокатор готов
    *reinterpret_cast<void**>(reinterpret_cast<size_t*>(*first_block_address_space_address) + 1) = nullptr;

    if(logger != nullptr)
    {
        logger->debug(get_typename() + "END: constructor");
    }

}

[[nodiscard]] void *allocator_sorted_list::allocate(size_t value_size, size_t values_count)
{
    debug_with_guard(get_typename() + "START: allocate");

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
        warning_with_guard("requested space size was changed");
    }

    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();
    void *target_block = nullptr;
    void *previous_to_target_block = nullptr;
    void *next_to_target_block = nullptr;

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
                next_to_target_block = get_aviable_block_next_block_address(current_block);
            }

            previous_block = current_block;
            current_block = get_aviable_block_next_block_address(current_block);
        }
    }

    if (target_block == nullptr)
    {
        error_with_guard("can't allocate block!");
        lock.unlock();
        throw std::bad_alloc();
    }


    if(previous_to_target_block == nullptr)
    {
        auto block_sizes_difference = get_aviable_block_size(target_block) - requested_size;
        void* adress_of_next_available_block = get_aviable_block_next_block_address(target_block);
        auto metadata = sizeof(block_pointer_t) + sizeof(block_size_t);

        if(block_sizes_difference > 0 && block_sizes_difference < metadata)
        {
            requested_size = get_aviable_block_size(target_block);
            auto* adress_next_available_block = get_aviable_block_next_block_address(target_block);
            void** first_available_block = reinterpret_cast<void**>(_trusted_memory) + get_ancillary_space_size();
            *first_available_block = adress_next_available_block;
            warning_with_guard("requested size was changed");

        }
        //если заняли таргет блок
        else
        {
            auto target_block_size = get_aviable_block_size(target_block);
            *reinterpret_cast<size_t*>(target_block) = requested_size; //установили размер этого блока
            void* adres_next_available_block = get_aviable_block_next_block_address(target_block);

            //теперь обрабатываем следующий свободный блок, тк таргет блок мы уже заняли, который идёт псоле таргета
            void* new_available_block = reinterpret_cast<unsigned char*>(target_block) + sizeof(size_t) + sizeof(void*) + requested_size;
            size_t* size_new_available_block = reinterpret_cast<size_t*>(new_available_block);
            void** adress_new_available_block = reinterpret_cast<void**>(size_new_available_block + 1);
            auto meta = sizeof(size_t) + sizeof(void*);

            //устанавливаем его данные
            *size_new_available_block = target_block_size - requested_size - meta;
            *adress_new_available_block = adres_next_available_block;

            void** first_available_block = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size());
            *first_available_block = new_available_block;
        }
        //занятый блок указывает на начало памяти, чтобы при освобождении проверить, что он вернётся туда откуда пришёл
        adress_of_next_available_block = _trusted_memory;

    }
    else
    {
        void* adress_of_next_available_block = get_aviable_block_next_block_address(target_block);
        auto block_sizes_difference = get_aviable_block_size(target_block) - requested_size;
        auto metadata = sizeof(block_pointer_t) + sizeof(block_size_t);
        if(block_sizes_difference > 0 && block_sizes_difference < metadata)
        {
            warning_with_guard(get_typename() + "requested size was changed");
            requested_size = get_aviable_block_size(target_block);
            void** address_previous_block_to_this = reinterpret_cast<void**>(reinterpret_cast<size_t*>(previous_to_target_block) + 1);
            void* address_current_block = get_aviable_block_next_block_address(target_block);
            *address_previous_block_to_this = address_current_block;

        }
        else
        {
            void** address_previous_block_to_current = reinterpret_cast<void**>(reinterpret_cast<size_t*>(previous_to_target_block) + 1);
            void* address_current_block = get_aviable_block_next_block_address(target_block);
            auto target_block_size = get_aviable_block_size(target_block);
            auto* size_of_target_block_ptr = reinterpret_cast<size_t*>(target_block);
            *size_of_target_block_ptr = requested_size;
            void* new_block_address = reinterpret_cast<unsigned char*>(target_block) + metadata + requested_size;
            *reinterpret_cast<size_t*>(new_block_address) = target_block_size - requested_size - metadata;
            *reinterpret_cast<void**>(reinterpret_cast<size_t*>(new_block_address) + 1) = address_current_block;
            *address_previous_block_to_current = new_block_address;

        }
        adress_of_next_available_block = _trusted_memory;
    }

    size_t initial_memory_size = *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*));
    auto* available_space_size = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t));
    auto metadata = sizeof(void*) + sizeof(size_t);
    *available_space_size = initial_memory_size - requested_size - metadata;
    information_with_guard("available memory:" + std::to_string(*available_space_size));
    std::vector<allocator_test_utils::block_info> information = get_blocks_info();
    std::string data_string;
    for(block_info value : information)
    {
        std::string is_block_occupied = value.is_block_occupied ? "YES" : "NO";
        data_string += (is_block_occupied + " " + std::to_string(value.block_size) + " | ");

    }

    debug_with_guard(get_typename() + "END: allocate");
    lock.unlock();
    return reinterpret_cast<unsigned char*>(target_block) + metadata;
}

void allocator_sorted_list::deallocate(void *deallocated_block)
{
    debug_with_guard(get_typename() + "START: deallocate");
    std::mutex* mutex = get_mutex();
    std::unique_lock<std::mutex> lock(*mutex);
    while(!lock.owns_lock())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (deallocated_block == nullptr)
    {
        return;
    }
    void *target_block = reinterpret_cast<void*>(reinterpret_cast<char *>(deallocated_block) - sizeof(void*) - sizeof(size_t));
    auto size_target_block = *reinterpret_cast<size_t*>(target_block);
    debug_with_guard(get_typename() + "START: deallocate");
    std::string result;
    auto *size_space = reinterpret_cast<size_t *>(target_block);
    auto *ptr = reinterpret_cast<void *>(size_space + 1);
    auto* bytes = reinterpret_cast<unsigned char*>(ptr) + 1;

    size_t size_object = get_occupied_block_size(target_block);

    for (int i = 0; i < size_object; i++)
    {
        result += std::to_string(static_cast<int>(bytes[i])) + " ";
    }

    debug_with_guard("state of block: " + result);
    size_t size_before = *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *));
    size_t* summ_size = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *));
    if (*reinterpret_cast<void**>(reinterpret_cast<size_t*>(target_block) + 1) != _trusted_memory)
    {
        error_with_guard("block doesn't belong to this allocator!");
        lock.unlock();
        throw std::logic_error("block doesn't belong this allocator!");
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
    information_with_guard("deallocate memory. free sum size" + std::to_string(*size_space));
    std::vector<allocator_test_utils::block_info> data = get_blocks_info();
    std::string data_str;
    for (block_info value : data)
    {
        std::string is_oc = value.is_block_occupied ? "YES" : "NO";
        data_str += (is_oc + "  " + std::to_string(value.block_size) + " | ");
    }

    debug_with_guard(get_typename() + "END: deallocate");
    lock.unlock();
}

inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode mode)
{
    debug_with_guard(get_typename() + "START: set_fit_mode");
    debug_with_guard(get_typename() + "END: set_fit_mode");
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(size_t)) = mode;
}

inline allocator *allocator_sorted_list::get_allocator() const
{
    debug_with_guard(get_typename() + "START: get_allocator");
    debug_with_guard(get_typename() + "END: get_allocator");
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

inline std::mutex* allocator_sorted_list::get_mutex() const noexcept
{
    debug_with_guard(get_typename() + "START: get_mutex");
    debug_with_guard(get_typename() + "END: get_mutex");
    return *reinterpret_cast<std::mutex**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode));
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    debug_with_guard(get_typename() + "START: get_blocks_info()");

    std::vector<allocator_test_utils::block_info> information;
    void** result = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size());
    void* current_block_address = reinterpret_cast<void**>(result + 1);

    size_t current_size = 0;
    size_t size_of_trusted_memory = *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t));
    auto metadata = sizeof(void*) + sizeof(size_t);

    while(current_size < size_of_trusted_memory)
    {
        allocator_test_utils::block_info value;
        value.block_size = *reinterpret_cast<size_t*>(current_block_address);
        void* address_next = *reinterpret_cast<void**>(reinterpret_cast<size_t*>(current_block_address) + 1);
        value.is_block_occupied = (address_next == _trusted_memory) ? true : false;
        information.push_back(value);
        current_block_address = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current_block_address) + metadata + value.block_size);
        current_size = current_size + metadata + value.block_size;

    }

    debug_with_guard(get_typename() + "END: get_blocks_info()");
    return information;
}

inline logger *allocator_sorted_list::get_logger() const
{
    debug_with_guard(get_typename() + "START: get_logger");
    debug_with_guard(get_typename() + "END: get_logger");
    return *reinterpret_cast<logger **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *));
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    return "allocator_sorted_list";
}

//сколько всей памяти занимают метаданные
size_t allocator_sorted_list::get_ancillary_space_size() const noexcept
{
    debug_with_guard(get_typename() + "START: get_ancillary_space_size()");
    debug_with_guard(get_typename() + "END: get_ancillary_space_size()");
    return sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*) + sizeof(void*);
}

allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const noexcept
{
    debug_with_guard(get_typename() + "START: get_fit_mode()");
    debug_with_guard(get_typename() + "END: get_fit_mode()");
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t));
}

void *allocator_sorted_list::get_first_aviable_block() const noexcept
{
    debug_with_guard(get_typename() + "START: get_first_aviable_block()");
    debug_with_guard(get_typename() + "END: get_first_aviable_block()");
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*));
}

allocator::block_size_t allocator_sorted_list::get_aviable_block_size(void *block_address) noexcept
{

    return *reinterpret_cast<size_t *>(block_address);
}

void *allocator_sorted_list::get_aviable_block_next_block_address(void* block_address) noexcept
{

    return *reinterpret_cast<void**>((reinterpret_cast<size_t*>(block_address) + 1));
}

allocator::block_size_t allocator_sorted_list::get_occupied_block_size(void *block_address) noexcept
{

    return *reinterpret_cast<allocator::block_size_t *>(block_address);
}


