#ifndef MP_OS_WORKBENCH_HASH_TABLE_H
#define MP_OS_WORKBENCH_HASH_TABLE_H

#include <associative_container.h>
#include <not_implemented.h>

template<
    typename tkey,
    typename tvalue>
class hash_table final:
    public associative_container<tkey, tvalue>
{

public:

    void insert(
        tkey const &key,
        tvalue const &value) override;

    void insert(
        tkey const &key,
        tvalue &&value) override;

    tvalue const &obtain(
        tkey const &key) override;

    tvalue dispose(
        tkey const &key) override;

};

template<
    typename tkey,
    typename tvalue>
void hash_table<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    throw not_implemented("template<typename tkey, typename tvalue> void hash_table<tkey, tvalue>::insert(tkey const &, tvalue const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void hash_table<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    throw not_implemented("template<typename tkey, typename tvalue> void hash_table<tkey, tvalue>::insert(tkey const &, tvalue &&)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue const &hash_table<tkey, tvalue>::obtain(
    tkey const &key)
{
    throw not_implemented("template<typename tkey, typename tvalue> tvalue const &hash_table<tkey, tvalue>::obtain(tkey const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue hash_table<tkey, tvalue>::dispose(
    tkey const &key)
{
    throw not_implemented("template<typename tkey, typename tvalue> tvalue hash_table<tkey, tvalue>::dispose(tkey const &)", "your code should be here...");
}

#endif //MP_OS_WORKBENCH_HASH_TABLE_H