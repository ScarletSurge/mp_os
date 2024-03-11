#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_TEST_UTILS_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_TEST_UTILS_H

#include <cstddef>
#include <vector>

class allocator_test_utils
{

public:
    
    struct block_info final
    {
        
        size_t block_size;
        
        bool is_block_occupied;
        
        bool operator==(
            block_info const &other) const noexcept;
        
        bool operator!=(
            block_info const &other) const noexcept;
        
    };

public:
    
    virtual ~allocator_test_utils() noexcept = default;

public:
    
    virtual std::vector<block_info> get_blocks_info() const noexcept = 0;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_TEST_UTILS_H