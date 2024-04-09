#include <not_implemented.h>

#include "../include/allocator_buddies_system.h"

allocator_buddies_system::~allocator_buddies_system()
{
    allocator* parent_allocator = get_allocator();
    allocator::destruct(get_mutex());
    debug_with_guard(get_typename() + " "+ "START: ~allocator_sorted_list()")->
    debug_with_guard(get_typename() + " " + "END: ~allocator_sorted_list()");
    if(parent_allocator != nullptr)
    {
        parent_allocator->deallocate(_trusted_memory);
    }
    else
    {
        ::operator delete(_trusted_memory);
    }

}


allocator_buddies_system::allocator_buddies_system(allocator_buddies_system &&other) noexcept : _trusted_memory(other._trusted_memory)
{
    other._trusted_memory = nullptr;
}

allocator_buddies_system &allocator_buddies_system::operator=(allocator_buddies_system &&other) noexcept
{
     if(this != &other)
     {
         allocator* parent_allocator = get_allocator();
        if(parent_allocator != nullptr)
        {
            parent_allocator->deallocate(_trusted_memory);

        }
        else
        {
            ::operator delete(_trusted_memory);
        }
         _trusted_memory = other._trusted_memory;
         other._trusted_memory = nullptr;

     }
     return *this;

}

allocator_buddies_system::allocator_buddies_system(
    size_t power_of_two_space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->debug(get_typename() + " START: constructor");
    }
    //в одном байте: старший бит - занятость (1 занят, 0 - свободен), оставшиеся 7 бит - размер
    if ((1 << power_of_two_space_size) < (sizeof(unsigned char) + sizeof(void*) + sizeof(void*)))
    {
        if (logger != nullptr)
        {
            logger->error("too small size\n");
        }
        throw std::logic_error("too small size");
    }

    size_t common_size = (1 << power_of_two_space_size) + get_ancillary_space_size();

    try
    {
        _trusted_memory = parent_allocator == nullptr
                          ? ::operator new(common_size)
                          : parent_allocator->allocate(common_size, 1);
    }
    catch (std::bad_alloc const &ex)
    {
        logger->error("can't get memory");
        throw std::bad_alloc();
    }
    //1
    allocator** parent_allocator_space_address = reinterpret_cast<allocator**>(_trusted_memory);
    *parent_allocator_space_address = parent_allocator;
    //2
    class logger** logger_space_address = reinterpret_cast<class logger**>(parent_allocator_space_address + 1);
    *logger_space_address = logger;
    //power of two!! 3
    size_t* space_size_power_two_space_address = reinterpret_cast<size_t*>(logger_space_address + 1);
    *space_size_power_two_space_address = power_of_two_space_size;
    //power of two 4
    size_t* size_of_free_memory = reinterpret_cast<size_t*>(space_size_power_two_space_address + 1);
    *size_of_free_memory = 1 << power_of_two_space_size;
    //5
    allocator_with_fit_mode::fit_mode* fit_mode_space_address = reinterpret_cast<allocator_with_fit_mode::fit_mode *>(size_of_free_memory + 1);
    *fit_mode_space_address = allocate_fit_mode;
    //6
    std::mutex** mutex_space_address = reinterpret_cast<std::mutex**>(fit_mode_space_address + 1);
    *mutex_space_address = new std::mutex;
    //7
    void** first_block_address_space_address = reinterpret_cast<void**>(mutex_space_address + 1);
    *first_block_address_space_address = first_block_address_space_address + 1;
    //meta block
    unsigned char* state_and_power_size = reinterpret_cast<unsigned char*>(*first_block_address_space_address);
    *state_and_power_size |= power_of_two_space_size;

//    std::cout << std::to_string(*state_and_power_size) << std::endl;

    void** previous_available_block_address = reinterpret_cast<void**>(state_and_power_size + 1);
    *previous_available_block_address = nullptr;

    void** next_available_block_address = reinterpret_cast<void**>(previous_available_block_address + 1);
    *next_available_block_address = nullptr;


    if(logger != nullptr)
    {
        logger->debug(get_typename() + " END: constructor. allocator has created");
    }

}

[[nodiscard]] void *allocator_buddies_system::allocate(size_t value_size, size_t values_count)
{
    debug_with_guard(get_typename() + " " + "START: allocate");
    std::mutex* mutex = get_mutex();
    std::lock_guard<std::mutex> lock(*mutex);
    size_t metadata_of_block = sizeof(char) + sizeof(void*) + sizeof(void*);
    size_t requested_size = value_size * values_count + metadata_of_block;
    short requested_size_power_of_two = get_power_of_two_up(requested_size);

    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();
    size_t* available_size = get_available_size();
//    std::cout<< *available_size << std::endl;

    void* previous_to_target_block = nullptr;
    void* target_block = nullptr;
    void* next_to_target_block = nullptr;

    {
        void *current_block = get_first_available_block();
        void *previous_block = nullptr;

        while (current_block != nullptr)
        {
            unsigned char state_and_power_of_block = *reinterpret_cast<unsigned char*>(current_block);
            size_t extracted_power_size_of_current_block = static_cast<int>(state_and_power_of_block & 127);

            if((1 << extracted_power_size_of_current_block) >= requested_size && (fit_mode == allocator_with_fit_mode::fit_mode::first_fit ||
            fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit && (target_block == nullptr ||
                    static_cast<int>((*get_power_two_of_block(target_block)) & 127) > extracted_power_size_of_current_block) ||
                    fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit &&
                            (target_block == nullptr || static_cast<int>((*get_power_two_of_block(target_block)) & 127) < extracted_power_size_of_current_block)))
            {
                previous_to_target_block = previous_block;
                target_block = current_block;
                next_to_target_block = get_next_available_block(current_block);
                if(fit_mode == allocator_with_fit_mode::fit_mode::first_fit)
                {
                    break;
                }
            }
            previous_block = current_block;
            current_block = get_next_available_block(current_block);

        }

    }
    //если ничего не нашли
    if(target_block == nullptr)
    {
        error_with_guard(get_typename() + "failed to allocate memory");
        throw std::bad_alloc();
    }
    //нашли блок
    size_t target_block_power = static_cast<int>((*get_power_two_of_block(target_block)) & 127);
    unsigned char target_block_power_char = (*reinterpret_cast<unsigned char*>(target_block) & 127);
    if((1 << target_block_power) > requested_size)
    {
        void* next_available_block_address = get_next_available_block(target_block);
        unsigned char* space_state_and_power_of_target_block = reinterpret_cast<unsigned char*>(target_block);
        //дробление
        while(((1 << target_block_power) >> 1) > requested_size)
        {
            target_block_power--;
            //находим указатель на двойника
            void* buddie = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(target_block) + (1 << target_block_power));

            //meta budy 1) полложили размер и занятость у него будет 0 автоматический и указатель на предыдущий и следующий
            *reinterpret_cast<unsigned char*>(buddie) |= target_block_power_char;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(buddie) + sizeof(unsigned char)) = target_block;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(buddie) + sizeof(unsigned char) + sizeof(void*)) = get_next_available_block(target_block);

            *space_state_and_power_of_target_block |= target_block_power_char;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(target_block) + sizeof(unsigned char) + sizeof(void*)) = buddie;

        }

    }

    if(previous_to_target_block == nullptr)
    {//если звнят прям который перед метаданными алллокатора
        void** first_available_block = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*));
        *first_available_block = get_next_available_block(target_block);
    }
    else
    {
        //если он свободен
        void** address_to_next_block_in_previous = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous_to_target_block) + sizeof(unsigned char) + sizeof(void*));
        *address_to_next_block_in_previous = target_block;
    }
    //короче всё закончили устанавливаем что наш таргет

    unsigned char* state_and_power_trgt_blck = reinterpret_cast<unsigned char*>(target_block);

    *state_and_power_trgt_blck |= (1 << 7); //поставили единичку в старший бит теперь таргет блок занят
//    std::cout << static_cast<int>(*state_and_power_trgt_blck) << std::endl;
    *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(target_block) + sizeof(unsigned char)) = _trusted_memory;
    (*available_size) -= (1 << target_block_power);

    debug_with_guard(get_typename() + " " + "END: allocate")->
            information_with_guard(get_typename() + "available size: " + std::to_string(*available_size));

    get_blocks_info();
    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(target_block) + sizeof(unsigned char) + sizeof(void*) + sizeof(void*));

}

void allocator_buddies_system::deallocate(void *at)
{
    debug_with_guard(get_typename() + " " + "START: deallocate");
    std::mutex* mutex = get_mutex();
    std::lock_guard<std::mutex> lock(*mutex);

    size_t* available_size = get_available_size();

    void* target_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(at) - sizeof(unsigned char) - sizeof(void*) - sizeof(void*));

    size_t target_block_power = static_cast<int>((*get_power_two_of_block(target_block)) & 127);
    void* next_target_block = get_first_available_block();
    void* previous_target_block = nullptr;
    while(next_target_block != nullptr && next_target_block <= target_block)
    {
        previous_target_block = next_target_block;
        next_target_block = get_next_available_block(next_target_block);
    }
    //инвертируем состояние бита
    *reinterpret_cast<unsigned char*>(target_block) ^= (1 << 7);
    *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(target_block) + sizeof(unsigned char)) = previous_target_block;
    *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(target_block) + sizeof(unsigned char) + sizeof(void*)) = next_target_block;

    if(previous_target_block == nullptr)
    {
        void** first_free_block = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*));
        *first_free_block = target_block;
    }

    else
    {
        *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous_target_block) + sizeof(unsigned char) + sizeof(void*)) = target_block;
    }

    if(next_target_block != nullptr)
    {
        *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(next_target_block) + sizeof(unsigned char)) = target_block;
    }

    void* buddy = get_buddy(target_block);

    while(reinterpret_cast<unsigned char*>(buddy) < reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size() +
                                                            (*reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) +
                                                            sizeof(logger*))) && buddy != nullptr && is_free_block(buddy) && static_cast<int>((*get_power_two_of_block(buddy)) & 127) == target_block_power)
    {
        void* next_block_buddy = get_next_available_block(buddy);
        *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(target_block)  + sizeof(unsigned char) + sizeof(void*)) = next_block_buddy;
        *reinterpret_cast<unsigned char*>(target_block) += 1;

        if(next_block_buddy != nullptr)
        {
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(next_block_buddy) + sizeof(unsigned char)) = target_block;
        }

        buddy = get_buddy(target_block);
    }

    (*available_size) += (1 << target_block_power);
    get_blocks_info();
    debug_with_guard(get_typename() + " " + "END: deallocate");
}

inline void allocator_buddies_system::set_fit_mode(allocator_with_fit_mode::fit_mode mode)
{
    *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t)) = mode;
}

inline allocator *allocator_buddies_system::get_allocator() const
{
    return *reinterpret_cast<allocator**>(_trusted_memory);
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept
{
   debug_with_guard(get_typename() + "START: get_blocks_info");
   std::vector<allocator_test_utils::block_info> information_about_blocks;

   void** first_current_block = reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*));
   void* current_block = reinterpret_cast<void*>(first_current_block + 1);

   size_t current_size = 0;
   size_t size_of_trusted_memory = 1 << *(reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*)));

   while(current_size < size_of_trusted_memory)
   {
       allocator_test_utils::block_info block_info;

       unsigned char occupation = *reinterpret_cast<unsigned char*>(current_block);
       occupation = (occupation & (1 << 7)) ? 1 : 0;
       size_t power_size_current_block = static_cast<int>(*reinterpret_cast<unsigned char*>(current_block) & 127);
       block_info.is_block_occupied = occupation;
       block_info.block_size = (1 << (power_size_current_block));

       std::cout << (block_info.block_size) << std::endl;

       information_about_blocks.push_back(block_info);
       current_block = get_next_block(current_block);

       current_size += block_info.block_size;
   }

   std::string data_string;

   for(block_info value : information_about_blocks)
    {
        std::string is_occupied = value.is_block_occupied ? "occup" : "avail";
        data_string += (is_occupied + " " + std::to_string(value.block_size) + " | ");
    }

    debug_with_guard(get_typename() + " state of blocks: " + data_string);
    debug_with_guard(get_typename() + "END: get_block_info");

    return information_about_blocks;
}


inline logger *allocator_buddies_system::get_logger() const
{
    return *reinterpret_cast<logger**>(reinterpret_cast<allocator**>(_trusted_memory) + 1);
}

inline std::string allocator_buddies_system::get_typename() const noexcept
{
    return "allocator_buddies_system";
}

size_t allocator_buddies_system::get_ancillary_space_size() const noexcept
{
    return sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*) + sizeof(void*);
}

std::mutex* allocator_buddies_system::get_mutex() const noexcept
{
    return *reinterpret_cast<std::mutex **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t)
                                            + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode));
}

short allocator_buddies_system::get_power_of_two_up(size_t value)
{
    return static_cast<short>(std::ceil(log2(value)));
}

allocator_with_fit_mode::fit_mode allocator_buddies_system::get_fit_mode() const noexcept
{
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(size_t) + sizeof(size_t));
}

void* allocator_buddies_system::get_first_available_block() const noexcept
{
     return *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t) + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex*));
}
//size of free memory
size_t* allocator_buddies_system::get_available_size() const noexcept
{
    return reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(size_t));
}

unsigned char* allocator_buddies_system::get_power_two_of_block(void *block_address) const
{
//    unsigned char state_and_power = *reinterpret_cast<unsigned char*>(block_address);
//    unsigned char extracted_power = state_and_power & 127;
//    return extracted_power;
        return reinterpret_cast<unsigned char*>(block_address);
}

void* allocator_buddies_system::get_next_available_block(void *current_block) const
{
    return *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(current_block) + sizeof(unsigned char) + sizeof(void*));
}

void* allocator_buddies_system::get_next_block(void *current_block) const noexcept
{
    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(current_block) + (1 << (*reinterpret_cast<unsigned char*>(current_block)) & 127));
}

void* allocator_buddies_system::get_buddy(void *target_block) const noexcept
{
    size_t target_block_size = 1 << static_cast<int>(*get_power_two_of_block(target_block) & 127);
    void* start_allocated_memory = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(_trusted_memory) + get_ancillary_space_size());
    auto address = reinterpret_cast<unsigned char*>(target_block) - reinterpret_cast<unsigned char*>(start_allocated_memory);
    auto buddy = address ^ target_block_size;

    auto result = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(start_allocated_memory) + buddy);

    return result;
}

bool allocator_buddies_system::is_free_block(void *target_block) const noexcept
{
    unsigned char occupation = (*reinterpret_cast<unsigned char*>(target_block)) & 128;
    occupation = (occupation & (1 << 7)) ? 1 : 0;
    return occupation == 0;
}
