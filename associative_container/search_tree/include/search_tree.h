#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H

#include <iostream>
#include <functional>
#include <stack>
#include <vector>

#include <allocator.h>
#include <allocator_guardant.h>
#include <associative_container.h>
#include <logger.h>
#include <logger_guardant.h>
#include <not_implemented.h>

template<
    typename tkey,
    typename tvalue>
class search_tree:
    public associative_container<tkey, tvalue>,
    protected allocator_guardant,
    protected logger_guardant
{

public:
    
    struct __attribute__((unused)) common_node
    {
    
    public:
        
        typename associative_container<tkey, tvalue>::key_value_pair *keys_and_values;
        
        common_node **subtrees;
        
        size_t virtual_size;
    
    public:
    
        common_node();
        
        virtual ~common_node() noexcept;
        
    };

protected:
    
    std::function<int(tkey const &, tkey const &)> _keys_comparer;

private:
    
    logger *_logger;
    
    allocator *_allocator;

protected:
    
    explicit search_tree(
        std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
        logger *logger = nullptr,
        allocator *allocator = nullptr);
    
public:
    
    virtual std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) = 0;

protected:
    
    [[nodiscard]] inline allocator *get_allocator() const final;
    
    [[nodiscard]] inline logger *get_logger() const final;
    
};

//region search_tree<tkey, tvalue>::node implementation

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::common_node::common_node()
{
    throw not_implemented("template<typename tkey, typename tvalue> search_tree<tkey, tvalue>::common_node::common_node()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::common_node::~common_node() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> search_tree<tkey, tvalue>::common_node::~common_node() noexcept", "your code should be here...");
}

// endregion search_tree<tkey, tvalue>::node implementation

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::search_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    logger *logger,
    allocator *allocator)
{
    throw not_implemented("template<typename tkey, typename tvalue> search_tree<tkey, tvalue>::search_tree(std::function<int(tkey const &, tkey const &)>, logger *, allocator *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *search_tree<tkey, tvalue>::get_allocator() const
{
    throw not_implemented("template<typename tkey, typename tvalue> [[nodiscard]] inline allocator *search_tree<tkey, tvalue>::get_allocator() const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *search_tree<tkey, tvalue>::get_logger() const
{
    throw not_implemented("template<typename tkey, typename tvalue> [[nodiscard]] inline logger *search_tree<tkey, tvalue>::get_logger() const", "your code should be here...");
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H