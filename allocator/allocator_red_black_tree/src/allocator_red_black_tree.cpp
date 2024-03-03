#include <not_implemented.h>

#include "../include/allocator_red_black_tree.h"

allocator_red_black_tree::~allocator_red_black_tree()
{
    throw not_implemented("allocator_red_black_tree::~allocator_red_black_tree()", "your code should be here...");
}

allocator_red_black_tree::allocator_red_black_tree(
    allocator_red_black_tree const &other)
{
    throw not_implemented("allocator_red_black_tree::allocator_red_black_tree(allocator_red_black_tree const &)", "your code should be here...");
}

allocator_red_black_tree &allocator_red_black_tree::operator=(
    allocator_red_black_tree const &other)
{
    throw not_implemented("allocator_red_black_tree &allocator_red_black_tree::operator=(allocator_red_black_tree const &)", "your code should be here...");
}

allocator_red_black_tree::allocator_red_black_tree(
    allocator_red_black_tree &&other) noexcept
{
    throw not_implemented("allocator_red_black_tree::allocator_red_black_tree(allocator_red_black_tree &&) noexcept", "your code should be here...");
}

allocator_red_black_tree &allocator_red_black_tree::operator=(
    allocator_red_black_tree &&other) noexcept
{
    throw not_implemented("allocator_red_black_tree &allocator_red_black_tree::operator=(allocator_red_black_tree &&) noexcept", "your code should be here...");
}

allocator_red_black_tree::allocator_red_black_tree(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    throw not_implemented("allocator_red_black_tree::allocator_red_black_tree(size_t, allocator *, logger *, allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

[[nodiscard]] void *allocator_red_black_tree::allocate(
    size_t value_size,
    size_t values_count)
{
    throw not_implemented("[[nodiscard]] void *allocator_red_black_tree::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_red_black_tree::deallocate(
    void *at)
{
    throw not_implemented("void allocator_red_black_tree::deallocate(void *)", "your code should be here...");
}

inline void allocator_red_black_tree::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    throw not_implemented("inline void allocator_red_black_tree::set_fit_mode(allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

inline allocator *allocator_red_black_tree::get_allocator() const
{
    throw not_implemented("inline allocator *allocator_red_black_tree::get_allocator() const", "your code should be here...");
}

std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_red_black_tree::get_logger() const
{
    throw not_implemented("inline logger *allocator_red_black_tree::get_logger() const", "your code should be here...");
}

inline std::string allocator_red_black_tree::get_typename() const noexcept
{
    throw not_implemented("inline std::string allocator_red_black_tree::get_typename() const noexcept", "your code should be here...");
}