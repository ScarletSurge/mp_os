#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"
//TODO  занятом хранить размер блока и адрес откуда взяли, чтобы потом проверить на то откуда брали
allocator_sorted_list::~allocator_sorted_list()
{
    throw not_implemented("allocator_sorted_list::~allocator_sorted_list()", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(allocator_sorted_list const &other) : _trusted_memory(other._trusted_memory)
{

}

allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list const &other)
{
    if(this != &other)
    {
        if(_trusted_memory)
        {
            ::operator delete(_trusted_memory);
        }
        _trusted_memory = other._trusted_memory;
    }
    return *this;
}


allocator_sorted_list::allocator_sorted_list(allocator_sorted_list &&other) noexcept : _trusted_memory(other._trusted_memory)
{
    other._trusted_memory = nullptr;
}

allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list &&other) noexcept
{
    if(this != &other)
    {
        if(_trusted_memory)
        {
            ::operator delete(_trusted_memory);
        }
        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }

    return *this;
}

allocator_sorted_list::allocator_sorted_list(
        size_t space_size,
        allocator *parent_allocator,
        logger *logger,
        allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (space_size < sizeof(block_size_t) + sizeof(block_pointer_t))
    {
        if (logger != nullptr)
        {
            logger->error("can't initialize allocator instance");
        }
        throw std::logic_error("can't initialize allocator instance");
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
    //1
    allocator **parent_allocator_space_address = reinterpret_cast<allocator **>(_trusted_memory);
    *parent_allocator_space_address = parent_allocator;
    //2
    class logger **logger_space_address = reinterpret_cast<class logger **>(parent_allocator_space_address + 1);
    *logger_space_address = logger;
    //3
    size_t *space_size_space_address = reinterpret_cast<size_t *>(logger_space_address + 1);
    *space_size_space_address = space_size;
    //4
    allocator_with_fit_mode::fit_mode *fit_mode_space_address = reinterpret_cast<allocator_with_fit_mode::fit_mode *>(space_size_space_address + 1);
    *fit_mode_space_address = allocate_fit_mode;
    //5
    void **first_block_address_space_address = reinterpret_cast<void **>(fit_mode_space_address + 1);
    *first_block_address_space_address = reinterpret_cast<void *>(first_block_address_space_address + 1);

    //указаетель на след свободный блок теперь nullptr
    *reinterpret_cast<void **>(*first_block_address_space_address) = nullptr;
    //размер первого блока
    *reinterpret_cast<size_t *>(reinterpret_cast<void **>(*first_block_address_space_address) + 1) = space_size - sizeof(block_size_t);

}

[[nodiscard]] void *allocator_sorted_list::allocate(size_t value_size, size_t values_count)
{
    auto requested_size = value_size * values_count;

    if (requested_size < sizeof(block_pointer_t))
    {
        requested_size = sizeof(block_pointer_t);
        warning_with_guard("requested space size was changed");
    }

    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();
    //сюда сохраняем результаты поиска
    void *target_block = nullptr;
    void *previous_to_target_block = nullptr;
    void *next_to_target_block = nullptr;

    {
        //по этим итерируемся и ищем
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
                //если что-то нашли
                previous_to_target_block = previous_block;
                target_block = current_block;
                next_to_target_block = get_aviable_block_next_block_address(current_block);
            }

            //продолжаем поиск ничего не нашли
            previous_block = current_block;
            current_block = get_aviable_block_next_block_address(current_block);
        }
    }

    //не смогли найти подходящего размера блок
    if (target_block == nullptr)
    {
        error_with_guard("can't allocate block!");
        throw std::bad_alloc();
    }


    if(previous_to_target_block == nullptr)
    {
        auto block_sizes_difference = get_aviable_block_size(target_block) - requested_size;
        void* adress_of_next_available_block = get_aviable_block_next_block_adress(target_block);
        auto metadata = sizeof(block_pointer_t) + sizeof(block_size_t);

        if(block_sizes_difference > 0 && block_sizes_difference < metadata)
        {
            requested_size = get_aviable_block_size(target_block);
            auto* adress_next_available_block = get_aviable_block_next_block_adress(target_block);
            void** first_available_block = reinterpret_cast<void**>(_trusted_memory) + get_ancillary_space_size();
            *first_available_block = adress_next_available_block;
            warning_with_guard("requested size was changed");

        }
        //если заняли таргет блок
        else
        {
            auto target_block_size = get_aviable_block_size(target_block);
            *reinterpret_cast<size_t*>(target_block) = requested_size; //установили размер этого блока
            void* adres_next_available_block = get_aviable_block_next_block_adress(target_block);

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


}

void allocator_sorted_list::deallocate(
        void *at)
{
    throw not_implemented("void allocator_sorted_list::deallocate(void *)", "your code should be here...");
}

inline void allocator_sorted_list::set_fit_mode(
        allocator_with_fit_mode::fit_mode mode)
{
    throw not_implemented("inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

inline allocator *allocator_sorted_list::get_allocator() const
{
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept", "your code should be here...");
}
//[a*][logger*][size][ptr_first_free*][синхронизация]|[  блоки памяти  ]
inline logger *allocator_sorted_list::get_logger() const
{
    return *reinterpret_cast<logger **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *));
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    return "allocator_sorted_list";
}

//сколько всей памяти занимают метаданные
size_t allocator_sorted_list::get_ancillary_space_size() const noexcept
{
    return sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(void*);
}

allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const noexcept
{
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t));
}

void *allocator_sorted_list::get_first_aviable_block() const noexcept
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode));
}

allocator::block_size_t allocator_sorted_list::get_aviable_block_size(
        void *block_address) noexcept
{
    return *reinterpret_cast<size_t *>(block_address);
}

void *allocator_sorted_list::get_aviable_block_next_block_adress(void *block_address) noexcept
{
    return *reinterpret_cast<void**>((reinterpret_cast<size_t*>(block_address) + 1));
}

allocator::block_size_t allocator_sorted_list::get_occupied_block_size(
        void *block_address) noexcept
{
    return *reinterpret_cast<allocator::block_size_t *>(block_address);
}


