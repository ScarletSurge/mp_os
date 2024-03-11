#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_GUARDANT_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_GUARDANT_H

#include "allocator.h"

class allocator_guardant
{

public:
    
    virtual ~allocator_guardant() noexcept = default;

public:
    
    [[nodiscard]] void *allocate_with_guard(
        size_t value_size,
        size_t values_count = 1) const;
    
    void deallocate_with_guard(
        void *at) const;

public:
    
    [[nodiscard]] inline virtual allocator *get_allocator() const = 0;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_GUARDANT_H