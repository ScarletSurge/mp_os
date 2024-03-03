#include <not_implemented.h>

#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger)
{
    throw not_implemented("allocator_global_heap::allocator_global_heap(logger *)", "your code should be here...");
}

allocator_global_heap::~allocator_global_heap()
{
    throw not_implemented("allocator_global_heap::~allocator_global_heap()", "your code should be here...");
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept
{
    throw not_implemented("allocator_global_heap::allocator_global_heap(allocator_global_heap &&) noexcept", "your code should be here...");
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    throw not_implemented("allocator_global_heap &allocator_global_heap::operator=(allocator_global_heap &&) noexcept", "your code should be here...");
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    throw not_implemented("[[nodiscard]] void *allocator_global_heap::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_global_heap::deallocate(
    void *at)
{
    throw not_implemented("void allocator_global_heap::deallocate(void *)", "your code should be here...");
}

inline logger *allocator_global_heap::get_logger() const
{
    throw not_implemented("inline logger *allocator_global_heap::get_logger() const", "your code should be here...");
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    throw not_implemented("inline std::string allocator_global_heap::get_typename() const noexcept", "your code should be here...");
}