#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"

allocator_sorted_list::~allocator_sorted_list()
{
    throw not_implemented("allocator_sorted_list::~allocator_sorted_list()", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list const &other)
{
    throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list const &)", "your code should be here...");
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list const &other)
{
    throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list const &)", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list &&other) noexcept
{
    throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list &&other) noexcept
{
    throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if(space_size < sizeof(block_size_t) + sizeof(block_pointer_t))
    {
        if(logger != nullptr)
        {
            logger->error("requested size was changed"); //TODO
        }

        throw std::logic_error("can't initialize allocator instance");
    }

    //размер памяти сколько запросил пользователь + ращмер метаданных
    auto common_size = space_size + get_ancillary_space_size();

    try
    {
        _trusted_memory = parent_allocator == nullptr ? ::operator new(common_size) : parent_allocator->allocate(common_size, 1);
    }

    catch(std::bad_alloc const &ex)
    {
        logger->error("failed to allocate memory");
        throw std::bad_alloc();
    }
    //1
    allocator** parent_allocator_space_address = reinterpret_cast<allocator**>(_trusted_memory);
    *parent_allocator_space_address = parent_allocator;
    //2
    //проинициализировали логгер
    class logger** logger_space_address = reinterpret_cast<class logger**>(parent_allocator_space_address + 1);
    //3
//    *space_size_space_addres = space_size;
    void ** first_block_address_space_address = reinterpret_cast<void**>(parent_allocator_space_address + 1);
    *first_block_address_space_address = reinterpret_cast<void*>(first_block_address_space_address + 1);

    *reinterpret_cast<void**>(*first_block_address_space_address) = nullptr;

    *reinterpret_cast<size_t*>(reinterpret_cast<void**>(*first_block_address_space_address) + 1) = space_size - sizeof(block_pointer_t);
    //4
    size_t* space_size_space_address = reinterpret_cast<size_t*>(logger_space_address + 1);
            *space_size_space_address = space_size;

    void** first_block_address_space_address = reinterpret_cast<void**>(space_size_space_address + 1);
    //--------\
    //[meta]   [ ]
    *first_block_address_space_address  = reinterpret_cast<void*>(first_block_address_space_address + 1);

    //заполнить информацией свободный блок

    //всё пока следующего элемента посоле этого блока нет поэтому нал
    *reinterpret_cast<void**>(*first_block_address_space_address) = nullptr;

    //указали размер блока
    *reinterpret_cast<size_t*>(reinterpret_cast<void**>(*first_block_address_space_address) + 1) = space_size - sizeof(block_size_t);



}

[[nodiscard]] void *allocator_sorted_list::allocate(
    size_t value_size,
    size_t values_count)
{
    auto requested_size = value_size * values_count;

    if(requested_size < sizeof(block_pointer_t))
    {
        requested_size = sizeof(block_pointer_t);
        warning_with_guard("requested space size was changed");
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
//[a*][logger*][size][ptr_first_free*][объект синхронизации]|[    ]
inline logger *allocator_sorted_list::get_logger() const
{
    return *reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*));
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    throw not_implemented("inline std::string allocator_sorted_list::get_typename() const noexcept", "your code should be here...");
}

//сколько всей памяти занимают метаданные
size_t allocator_sorted_list::get_ancillary_space_size() const noexcept
{
    return sizeof(logger*) + sizeof(allocator*) + sizeof(size_t) + sizeof(void*);
}