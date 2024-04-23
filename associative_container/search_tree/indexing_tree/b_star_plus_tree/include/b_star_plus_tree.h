#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_STAR_PLUS_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_STAR_PLUS_TREE_H

#include <search_tree.h>

template<
    typename tkey,
    typename tvalue>
class b_star_plus_tree final:
    public search_tree<tkey, tvalue>
{

public:

    class infix_iterator final
    {

    public:

        bool operator==(
            infix_iterator const &other) const noexcept;

        bool operator!=(
            infix_iterator const &other) const noexcept;

        infix_iterator &operator++();

        infix_iterator operator++(
            int not_used);

        std::tuple<size_t, tkey const &, tvalue &> operator*() const;

    };

    class infix_const_iterator final
    {

    public:

        bool operator==(
            infix_const_iterator const &other) const noexcept;

        bool operator!=(
            infix_const_iterator const &other) const noexcept;

        infix_const_iterator &operator++();

        infix_const_iterator operator++(
            int not_used);

        std::tuple<size_t, tkey const &, tvalue const &> operator*() const;

    };

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

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) override;

public:

    explicit b_star_plus_tree(
        size_t t,
        std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr);

    b_star_plus_tree(
        b_star_plus_tree<tkey, tvalue> const &other);

    b_star_plus_tree<tkey, tvalue> &operator=(
        b_star_plus_tree<tkey, tvalue> const &other);

    b_star_plus_tree(
        b_star_plus_tree<tkey, tvalue> &&other) noexcept;

    b_star_plus_tree<tkey, tvalue> &operator=(
        b_star_plus_tree<tkey, tvalue> &&other) noexcept;

    ~b_star_plus_tree();

public:

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

};

template<
    typename tkey,
    typename tvalue>
bool b_star_plus_tree<tkey, tvalue>::infix_iterator::operator==(
    typename b_star_plus_tree::infix_iterator const &other) const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> bool b_star_plus_tree<tkey, tvalue>::infix_iterator::operator==(typename b_star_plus_tree::infix_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool b_star_plus_tree<tkey, tvalue>::infix_iterator::operator!=(
    typename b_star_plus_tree::infix_iterator const &other) const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> bool b_star_plus_tree<tkey, tvalue>::infix_iterator::operator!=(typename b_star_plus_tree::infix_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_star_plus_tree<tkey, tvalue>::infix_iterator &b_star_plus_tree<tkey, tvalue>::infix_iterator::operator++()
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_star_plus_tree<tkey, tvalue>::infix_iterator &b_star_plus_tree<tkey, tvalue>::infix_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_star_plus_tree<tkey, tvalue>::infix_iterator b_star_plus_tree<tkey, tvalue>::infix_iterator::operator++(
    int not_used)
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_star_plus_tree<tkey, tvalue>::infix_iterator b_star_plus_tree<tkey, tvalue>::infix_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
std::tuple<size_t, tkey const &, tvalue &> b_star_plus_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    throw not_implemented("template<typename tkey, typename tvalue> std::tuple<size_t, tkey const &, tvalue &> b_star_plus_tree<tkey, tvalue>::infix_iterator::operator*() const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator==(
    b_star_plus_tree::infix_const_iterator const &other) const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> bool b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator==(b_star_plus_tree::infix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    b_star_plus_tree::infix_const_iterator const &other) const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> bool b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator!=(b_star_plus_tree::infix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_star_plus_tree<tkey, tvalue>::infix_const_iterator &b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_star_plus_tree<tkey, tvalue>::infix_const_iterator &b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_star_plus_tree<tkey, tvalue>::infix_const_iterator b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_star_plus_tree<tkey, tvalue>::infix_const_iterator b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
std::tuple<size_t, tkey const &, tvalue const &> b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    throw not_implemented("template<typename tkey, typename tvalue> std::tuple<size_t, tkey const &, tvalue const &> b_star_plus_tree<tkey, tvalue>::infix_const_iterator::operator*() const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void b_star_plus_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    throw not_implemented("template<typename tkey, typename tvalue> void b_star_plus_tree<tkey, tvalue>::insert(tkey const &, tvalue const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void b_star_plus_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    throw not_implemented("template<typename tkey, typename tvalue> void b_star_plus_tree<tkey, tvalue>::insert(tkey const &, tvalue &&)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue const &b_star_plus_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    throw not_implemented("template<typename tkey, typename tvalue> tvalue const &b_star_plus_tree<tkey, tvalue>::obtain(tkey const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue b_star_plus_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    throw not_implemented("template<typename tkey, typename tvalue> tvalue b_star_plus_tree<tkey, tvalue>::dispose(tkey const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> b_star_plus_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    throw not_implemented("template<typename tkey, typename tvalue> std::vector<typename associative_container<tkey, tvalue>::key_value_pair> b_star_plus_tree<tkey, tvalue>::obtain_between(tkey const &, tkey const &, bool, bool)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
b_star_plus_tree<tkey, tvalue>::b_star_plus_tree(
    size_t t,
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger)
{
    throw not_implemented("template<typename tkey, typename tvalue> b_star_plus_tree<tkey, tvalue>::b_star_plus_tree(size_t, std::function<int(tkey const &, tkey const &)>, allocator *, logger *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
b_star_plus_tree<tkey, tvalue>::b_star_plus_tree(
    b_star_plus_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> b_star_plus_tree<tkey, tvalue>::b_star_plus_tree(b_star_plus_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
b_star_plus_tree<tkey, tvalue> &b_star_plus_tree<tkey, tvalue>::operator=(b_star_plus_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> b_star_plus_tree<tkey, tvalue> &b_star_plus_tree<tkey, tvalue>::operator=(b_star_plus_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
b_star_plus_tree<tkey, tvalue>::b_star_plus_tree(
    b_star_plus_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> b_star_plus_tree<tkey, tvalue>::b_star_plus_tree(b_star_plus_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
b_star_plus_tree<tkey, tvalue> &b_star_plus_tree<tkey, tvalue>::operator=(
    b_star_plus_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> b_star_plus_tree<tkey, tvalue> &b_star_plus_tree<tkey, tvalue>::operator=(b_star_plus_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
b_star_plus_tree<tkey, tvalue>::~b_star_plus_tree()
{
    throw not_implemented("template<typename tkey, typename tvalue> b_star_plus_tree<tkey, tvalue>::~b_star_plus_tree()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_star_plus_tree<tkey, tvalue>::infix_iterator b_star_plus_tree<tkey, tvalue>::begin_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_star_plus_tree<tkey, tvalue>::infix_iterator b_star_plus_tree<tkey, tvalue>::begin_infix() const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_star_plus_tree<tkey, tvalue>::infix_iterator b_star_plus_tree<tkey, tvalue>::end_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_star_plus_tree<tkey, tvalue>::infix_iterator b_star_plus_tree<tkey, tvalue>::end_infix() const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_star_plus_tree<tkey, tvalue>::infix_const_iterator b_star_plus_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_star_plus_tree<tkey, tvalue>::infix_const_iterator b_star_plus_tree<tkey, tvalue>::cbegin_infix() const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_star_plus_tree<tkey, tvalue>::infix_const_iterator b_star_plus_tree<tkey, tvalue>::cend_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_star_plus_tree<tkey, tvalue>::infix_const_iterator b_star_plus_tree<tkey, tvalue>::cend_infix() const noexcept", "your code should be here...");
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_STAR_PLUS_TREE_H