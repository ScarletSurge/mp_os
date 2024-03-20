#include <not_implemented.h>

#include <allocator_global_heap.h>
#include <sstream>

template<typename T>
std::string convert_to_string(T value)
{
    std::ostringstream oss;
    oss << value;

    return oss.str();
}

std::string allocator_global_heap::get_memory_state(void *at) const
{
    std::string state;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(at);
    size_t size_of_block = *reinterpret_cast<size_t*>(reinterpret_cast<void**>(at) - 1);

    for(int i = 0; i < size_of_block; ++i)
    {
        state += convert_to_string(static_cast<int>(bytes[i])) + " ";
    }

    return state;
}



//метаданные аллокатора
allocator_global_heap::allocator_global_heap(logger* log_allocator) : _log_allocator(log_allocator) {}


//allocator::allocator_exception::allocator_exception(const std::string &msg) : _msg(msg) {}

allocator_global_heap::~allocator_global_heap()
{
//    throw not_implemented("allocator_global_heap::~allocator_global_heap()", "your code should be here...");
}

allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) noexcept
{
    throw not_implemented("allocator_global_heap::allocator_global_heap(allocator_global_heap &&) noexcept", "your code should be here...");
}

allocator_global_heap &allocator_global_heap::operator=(allocator_global_heap &&other) noexcept
{
    throw not_implemented("allocator_global_heap &allocator_global_heap::operator=(allocator_global_heap &&) noexcept", "your code should be here...");
}

[[nodiscard]] void *allocator_global_heap::allocate(size_t value_size, size_t values_count) //sizeof(char) * sizeof(string)
{
    auto requested_size = value_size * values_count;
    if(requested_size < sizeof(size_t))
    {
        requested_size = sizeof(size_t);
        warning_with_guard("requested space size was changed");
    }
    try
    {

        debug_with_guard("starts allocate method");
        auto* block_of_memory = ::operator new(requested_size + sizeof(size_t)); //размер всей памфти плюс метаданные

        size_t* block_size = reinterpret_cast<size_t*>(block_of_memory);
        *block_size = requested_size; //сохраняем информацию о размере выделенного блока в начале выделенного блока
        debug_with_guard("memory allocated");
        //возвращаем указатель на начало памяти после инфы о размере блока
        return reinterpret_cast<void*>(block_size + 1);
    }

    catch (const std::bad_alloc &ex)
    {
        std::string message = "failed to allocate memory";
        error_with_guard(message);
//        throw allocator::allocator_exception(message);
    }

}


void allocator_global_heap::deallocate(void *at) //блок который нало освободить
{
    debug_with_guard("starts deallocate method");
    std::string state = get_memory_state(at);
    debug_with_guard("state of block before deallocation: " + state);

    auto* block = reinterpret_cast<void*>(reinterpret_cast<size_t*>(at) - 1);
    ::operator delete(block);
    debug_with_guard("end of the deallocate method");


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
