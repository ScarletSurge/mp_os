#include <gtest/gtest.h>
#include <cmath>
#include <allocator.h>
#include <allocator_buddies_system.h>
#include <client_logger_builder.h>
#include <logger.h>
#include <logger_builder.h>

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    logger_builder *logger_builder_instance = new client_logger_builder;
    
    if (use_console_stream)
    {
        logger_builder_instance->add_console_stream(console_stream_severity);
    }
    
    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        logger_builder_instance->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }
    
    logger *logger_instance = logger_builder_instance->build();
    
    delete logger_builder_instance;
    
    return logger_instance;
}

TEST(positiveTests, test1)
{
    logger *logger_instance = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "allocator_buddies_system_positiveTests_test1.txt",
                logger::severity::information
            }
        });
    allocator *allocator_instance = new allocator_buddies_system(12, nullptr, logger_instance, allocator_with_fit_mode::fit_mode::first_fit);
    
    auto actual_blocks_state = dynamic_cast<allocator_test_utils *>(allocator_instance)->get_blocks_info();
    std::vector<allocator_test_utils::block_info> expected_blocks_state
        {
            { .block_size = 4096, .is_block_occupied = false }
        };
    
    ASSERT_EQ(actual_blocks_state.size(), expected_blocks_state.size());
    for (int i = 0; i < actual_blocks_state.size(); i++)
    {
        ASSERT_EQ(actual_blocks_state[i], expected_blocks_state[i]);
    }
    
    delete allocator_instance;
    delete logger_instance;
}

TEST(positiveTests, test2)
{
    logger *logger_instance = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "allocator_buddies_system_positiveTests_test1.txt",
                logger::severity::information
            }
        });
    allocator *allocator_instance = new allocator_buddies_system(8, nullptr, logger_instance, allocator_with_fit_mode::fit_mode::first_fit);
    
    void *first_block = allocator_instance->allocate(sizeof(unsigned char), 40);
    
    auto actual_blocks_state = dynamic_cast<allocator_test_utils *>(allocator_instance)->get_blocks_info();
    std::vector<allocator_test_utils::block_info> expected_blocks_state
        {
            { .block_size = 64, .is_block_occupied = true },
            { .block_size = 64, .is_block_occupied = false },
            { .block_size = 128, .is_block_occupied = false }
        };
    
    ASSERT_EQ(actual_blocks_state.size(), expected_blocks_state.size());
    for (int i = 0; i < actual_blocks_state.size(); i++)
    {
        ASSERT_EQ(actual_blocks_state[i], expected_blocks_state[i]);
    }
    
    allocator_instance->deallocate(first_block);
    
    delete allocator_instance;
    delete logger_instance;
}

TEST(positiveTests, test3)
{
    allocator *allocator_instance = new allocator_buddies_system(8, nullptr, nullptr, allocator_with_fit_mode::fit_mode::first_fit);
    
    void *first_block = allocator_instance->allocate(sizeof(unsigned char), 0);
    void *second_block = allocator_instance->allocate(sizeof(unsigned char), 0);
    allocator_instance->deallocate(first_block);
    
    auto actual_blocks_state = dynamic_cast<allocator_test_utils *>(allocator_instance)->get_blocks_info();
    ASSERT_EQ(actual_blocks_state.size(), 3);
    ASSERT_EQ(actual_blocks_state[0].block_size, 1 << (static_cast<int>(std::floor(std::log2(sizeof(allocator::block_pointer_t) * 2 + 1))) + 1));
    ASSERT_EQ(actual_blocks_state[0].is_block_occupied, false);
    ASSERT_EQ(actual_blocks_state[0].block_size, actual_blocks_state[1].block_size);
    ASSERT_EQ(actual_blocks_state[1].is_block_occupied, true);
    
    allocator_instance->deallocate(second_block);
    
    delete allocator_instance;
}

TEST(falsePositiveTests, test1)
{
    ASSERT_THROW(new allocator_buddies_system(static_cast<int>(std::floor(std::log2(sizeof(allocator::block_pointer_t) * 2 + 1))) - 1), std::logic_error);
}

int main(
    int argc,
    char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}