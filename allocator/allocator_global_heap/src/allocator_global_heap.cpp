#include <not_implemented.h>
#include <allocator_global_heap.h>
#include <string>

std::string allocator_global_heap::get_memory_state(void *at) const
{
    debug_with_guard(" START: get_memory_state");
    std::string state;
    auto* bytes = reinterpret_cast<unsigned char*>(at);
    size_t size_of_block = *reinterpret_cast<size_t*>(reinterpret_cast<void**>(at) - 1);

    for(int i = 0; i < size_of_block; i++)
    {
        state += std::to_string(static_cast<int>(bytes[i])) + " ";

    }

    debug_with_guard(" END: get_memory_state");
    return state;

}



//метаданные аллокатора
allocator_global_heap::allocator_global_heap(logger* log_allocator) : _log_allocator(log_allocator) { };


allocator_global_heap::~allocator_global_heap() = default;


[[nodiscard]] void *allocator_global_heap::allocate(size_t value_size, size_t values_count)
{
    debug_with_guard(" START: allocate");
    auto requested_size = value_size * values_count;

    if(requested_size < sizeof(size_t) + sizeof(allocator*))
    {
        requested_size = sizeof(size_t) + sizeof(allocator*);
        warning_with_guard(" requested space size was changed");
    }
    try
    {

        auto* block_of_memory = ::operator new(requested_size + sizeof(size_t) + sizeof(allocator*));

        auto** allocator_ptr = reinterpret_cast<allocator**>(block_of_memory);
        auto* block_size = reinterpret_cast<size_t*>(allocator_ptr + 1);
        *block_size = requested_size;
        *allocator_ptr = this;

        debug_with_guard(" END: allocate");
        return reinterpret_cast<unsigned char*>(block_of_memory) + sizeof(allocator*) + sizeof(size_t);
    }
    catch(const std::bad_alloc &ex)
    {
        error_with_guard("failed to allocate memory");
        throw std::bad_alloc();
    }



}


void allocator_global_heap::deallocate(void *at) //блок который нало освободить
{
    debug_with_guard(" START: deallocate");
    std::string state = get_memory_state(at);
    debug_with_guard(" state of block before deallocation: " + state);

    auto* size_of_bloc = reinterpret_cast<size_t*>(at) - 1;
    try
    {
        auto* alloc_ptr = *(reinterpret_cast<allocator**>(size_of_bloc) - 1);
        if(alloc_ptr != this)
        {
            error_with_guard(" this block doesn't belong!");
            throw std::logic_error(" doesn't belong!");
        }
    }

    catch(const std::logic_error &ex)
    {
        error_with_guard(" this block doesn't belong!");
        throw std::logic_error("doesn't belong");
    }

    auto* start_block = reinterpret_cast<unsigned char*>(at) - sizeof(allocator*) - sizeof(size_t);
    ::operator delete(start_block);
    debug_with_guard(" END: deallocate");


}

inline logger *allocator_global_heap::get_logger() const
{
    return _log_allocator;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    throw not_implemented("inline std::string allocator_global_heap::get_typename() const noexcept", "your code should be here...");
}

allocator_global_heap::allocator_global_heap()
= default;