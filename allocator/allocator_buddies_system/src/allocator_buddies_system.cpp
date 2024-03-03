#include <not_implemented.h>

#include "../include/allocator_buddies_system.h"

allocator_buddies_system::~allocator_buddies_system()
{
    throw not_implemented("allocator_buddies_system::~allocator_buddies_system()", "your code should be here...");
}

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system const &other)
{
    throw not_implemented("allocator_buddies_system::allocator_buddies_system(allocator_buddies_system const &)", "your code should be here...");
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system const &other)
{
    throw not_implemented("allocator_buddies_system &allocator_buddies_system::operator=(allocator_buddies_system const &)", "your code should be here...");
}

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system &&other) noexcept
{
    throw not_implemented("allocator_buddies_system::allocator_buddies_system(allocator_buddies_system &&) noexcept", "your code should be here...");
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system &&other) noexcept
{
    throw not_implemented("allocator_buddies_system &allocator_buddies_system::operator=(allocator_buddies_system &&) noexcept", "your code should be here...");
}

allocator_buddies_system::allocator_buddies_system(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    throw not_implemented("allocator_buddies_system::allocator_buddies_system(size_t, allocator *, logger *, allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

[[nodiscard]] void *allocator_buddies_system::allocate(
    size_t value_size,
    size_t values_count)
{
    throw not_implemented("[[nodiscard]] void *allocator_buddies_system::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_buddies_system::deallocate(
    void *at)
{
    throw not_implemented("void allocator_buddies_system::deallocate(void *)", "your code should be here...");
}

inline void allocator_buddies_system::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    throw not_implemented("inline void allocator_buddies_system::set_fit_mode(allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

inline allocator *allocator_buddies_system::get_allocator() const
{
    throw not_implemented("inline allocator *allocator_buddies_system::get_allocator() const", "your code should be here...");
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_buddies_system::get_logger() const
{
    throw not_implemented("inline logger *allocator_buddies_system::get_logger() const", "your code should be here...");
}

inline std::string allocator_buddies_system::get_typename() const noexcept
{
    throw not_implemented("inline std::string allocator_buddies_system::get_typename() const noexcept", "your code should be here...");
}