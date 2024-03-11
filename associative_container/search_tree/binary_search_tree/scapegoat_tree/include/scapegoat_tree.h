#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SCAPEGOAT_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SCAPEGOAT_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class scapegoat_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:
    
    struct scapegoat_node final:
        binary_search_tree<tkey, tvalue>::node
    {
        
        // TODO: think about it!
        
    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {
    
    public:
        
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value);
    
    };

private:
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
    public:
        
        explicit insertion_template_method(
            scapegoat_tree<tkey, tvalue> *tree);
    
    private:
        
        // TODO: think about it!
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method
    {
    
    public:
        
        explicit obtaining_template_method(
            scapegoat_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
    public:
        
        explicit disposal_template_method(
            scapegoat_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };

public:
    
    explicit scapegoat_tree(
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        double alpha = 0.5);

public:
    
    ~scapegoat_tree() noexcept final;
    
    scapegoat_tree(
        scapegoat_tree<tkey, tvalue> const &other);
    
    scapegoat_tree<tkey, tvalue> &operator=(
        scapegoat_tree<tkey, tvalue> const &other);
    
    scapegoat_tree(
        scapegoat_tree<tkey, tvalue> &&other) noexcept;
    
    scapegoat_tree<tkey, tvalue> &operator=(
        scapegoat_tree<tkey, tvalue> &&other) noexcept;

public:
    
    void setup_alpha(
        double alpha);
    
};

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value)
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue>::iterator_data::iterator_data(unsigned int, tkey const &, tvalue const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    scapegoat_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(scapegoat_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    scapegoat_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(scapegoat_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    scapegoat_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(scapegoat_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue>::scapegoat_tree(
    allocator *allocator,
    logger *logger,
    double alpha)
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue>::scapegoat_tree(allocator *, logger *, double)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue>::~scapegoat_tree() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue>::~scapegoat_tree() noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue>::scapegoat_tree(
    scapegoat_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue>::scapegoat_tree(scapegoat_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue> &scapegoat_tree<tkey, tvalue>::operator=(
    scapegoat_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue> &scapegoat_tree<tkey, tvalue>::operator=(scapegoat_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue>::scapegoat_tree(
    scapegoat_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue>::scapegoat_tree(scapegoat_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue> &scapegoat_tree<tkey, tvalue>::operator=(
    scapegoat_tree<tkey, tvalue> &&other) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> scapegoat_tree<tkey, tvalue> &scapegoat_tree<tkey, tvalue>::operator=(scapegoat_tree<tkey, tvalue> &&) noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void scapegoat_tree<tkey, tvalue>::setup_alpha(
    double alpha)
{
    throw not_implemented("", "your code should be here...");
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SCAPEGOAT_TREE_H