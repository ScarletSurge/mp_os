#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_H

#include <cstddef>

class allocator
{

public:
    
    typedef size_t block_size_t;
    
    typedef void *block_pointer_t;

public:
    
    virtual ~allocator() noexcept = default;

public:
    
    template<
        typename T,
        typename ...args>
    inline static void construct(
        T *at,
        args... constructor_arguments);
    
    template<
        typename T>
    inline static void destruct(
        T *at);

public:
    
    [[nodiscard]] virtual void *allocate(
        size_t value_size,
        size_t values_count) = 0;
    
    virtual void deallocate(
        void *at) = 0;
    
};

template<
    typename T,
    typename ...args>
inline void allocator::construct(
    T *at,
    args... constructor_arguments)
{
    new(at) T(constructor_arguments...);
}

template<
    typename T>
inline void allocator::destruct(
    T *at)
{
    at->~T();
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_H