#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>
#include <cmath>

class allocator_buddies_system final:
        private allocator_guardant,
        public allocator_test_utils,
        public allocator_with_fit_mode,
        private logger_guardant,
        private typename_holder
{

private:

    void *_trusted_memory;

public:

    ~allocator_buddies_system() override;

    allocator_buddies_system(allocator_buddies_system const &other) = delete;

    allocator_buddies_system &operator=(allocator_buddies_system const &other) = delete;

    allocator_buddies_system(allocator_buddies_system &&other) noexcept;

    allocator_buddies_system &operator=(allocator_buddies_system &&other) noexcept;

public:

    explicit allocator_buddies_system(
            size_t space_size_power_of_two,
            allocator *parent_allocator = nullptr,
            logger *logger = nullptr,
            allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:

    [[nodiscard]] void *allocate(
            size_t value_size,
            size_t values_count) override;

    void deallocate(
            void *at) override;

public:

    inline void set_fit_mode(
            allocator_with_fit_mode::fit_mode mode) override;

private:

    inline allocator *get_allocator() const override;

public:

    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:

    inline logger *get_logger() const override;

private:

    inline std::string get_typename() const noexcept override;

private:

    size_t get_ancillary_space_size() const noexcept;

    std::mutex* get_mutex() const noexcept;

    short get_power_of_two_up(size_t value);

    allocator_with_fit_mode::fit_mode get_fit_mode() const noexcept;

    void* get_first_available_block() const noexcept;

    size_t* get_available_size() const noexcept;

    unsigned char* get_power_two_of_block(void* block_address) const;

    void* get_next_available_block(void* current_block) const;

    void* get_next_block(void* current_block) const noexcept;

    void* get_buddy(void* target_block) const noexcept;

    bool is_free_block(void* target_block) const noexcept;


};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
