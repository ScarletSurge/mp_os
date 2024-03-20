#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_H

#include <cstddef>
#include <string>
#include <iostream>
#include <fstream>

class allocator
{

public:

    typedef size_t block_size_t;
    typedef void *block_pointer_t;

public:

    virtual ~allocator() noexcept = default;

public:

    template<typename T, typename ...args>
    inline static void construct(T *at, args... constructor_arguments);

    template<typename T>
    inline static void destruct(T* at);

//public:
//
//class allocator_exception final : public std::exception
//{
//private:
//
//    std::string _msg;
//
//public:
//
//    explicit allocator_exception(const std::string &msg);
//};

public:

    [[nodiscard]] virtual void *allocate(size_t value_size,size_t values_count) = 0;

    virtual void deallocate(void *at) = 0;

};

//allocator::allocator_exception::allocator_exception(const std::string &msg) : _msg(msg) {}

template<typename T, typename ...args>
inline void allocator::construct(T *at, args... constructor_arguments)
{
    new (at) T(constructor_arguments...);
}

template<typename T>
inline void allocator::destruct(T* at)
{
    at->~T();
}



#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_H