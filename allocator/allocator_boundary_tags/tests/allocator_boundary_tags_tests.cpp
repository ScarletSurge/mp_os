#include <gtest/gtest.h>
#include <allocator.h>
#include <allocator_boundary_tags.h>
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
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "allocator_boundary_tags_tests_logs_positive_test_plain_usage.txt",
                logger::severity::information
            }
        });
    allocator *subject = new allocator_boundary_tags(sizeof(int) * 40, nullptr, logger, allocator_with_fit_mode::fit_mode::first_fit);
    
    auto const *first_block = reinterpret_cast<int const *>(subject->allocate(sizeof(int), 10));
    auto const *second_block = reinterpret_cast<int const *>(subject->allocate(sizeof(int), 10));
    auto const *third_block = reinterpret_cast<int const *>(subject->allocate(sizeof(int), 10));
    
    ASSERT_EQ(first_block + 10, second_block);
    ASSERT_EQ(second_block + 10, third_block);
    
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(second_block)));
    
    auto *the_same_subject = dynamic_cast<allocator_with_fit_mode *>(subject);
    the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_worst_fit);
    auto const *fourth_block = reinterpret_cast<int const *>(subject->allocate(sizeof(int), 1));
    the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_best_fit);
    auto const *fifth_block = reinterpret_cast<int const *>(subject->allocate(sizeof(int), 1));
    
    ASSERT_EQ(first_block + 10, fifth_block);
    ASSERT_EQ(third_block + 10, fourth_block);
    
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(first_block)));
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(third_block)));
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(fourth_block)));
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(fifth_block)));
    
    delete subject;
    delete logger;
}

TEST(positiveTests, test2)
{
    logger *logger_instance = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "allocator_boundary_tags_tests_logs_false_positive_test_1.txt",
                logger::severity::information
            }
        });
    allocator *allocator_instance = new allocator_boundary_tags(sizeof(unsigned char) * 3000, nullptr, logger_instance, allocator_with_fit_mode::fit_mode::first_fit);
    
    char *first_block = reinterpret_cast<char *>(allocator_instance->allocate(sizeof(char), 1000));
    char *second_block = reinterpret_cast<char *>(allocator_instance->allocate(sizeof(char), 0));
    allocator_instance->deallocate(first_block);
    first_block = reinterpret_cast<char *>(allocator_instance->allocate(sizeof(char), 999));
    auto actual_blocks_state = dynamic_cast<allocator_test_utils *>(allocator_instance)->get_blocks_info();
    std::vector<allocator_test_utils::block_info> expected_blocks_state
        {
            { .block_size = 1000 + sizeof(allocator::block_size_t) + sizeof(allocator::block_pointer_t) * 2, .is_block_occupied = true },
            { .block_size = 1000 + sizeof(allocator::block_size_t) + sizeof(allocator::block_pointer_t) * 2, .is_block_occupied = true },
            { .block_size = 3000 - (1000 + sizeof(allocator::block_size_t) + sizeof(allocator::block_pointer_t)) * 2, .is_block_occupied = false }
        };
    
    ASSERT_EQ(actual_blocks_state.size(), expected_blocks_state.size());
    for (int i = 0; i < actual_blocks_state.size(); i++)
    {
        ASSERT_EQ(actual_blocks_state[i], expected_blocks_state[i]);
    }
    
    allocator_instance->deallocate(first_block);
    allocator_instance->deallocate(second_block);
    
    delete allocator_instance;
    delete logger_instance;
}

TEST(falsePositiveTests, test1)
{
    logger *logger_instance = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "allocator_boundary_tags_tests_logs_false_positive_test_2.txt",
                logger::severity::information
            }
        });
    allocator *allocator_instance = new allocator_boundary_tags(3000, nullptr, logger_instance, allocator_with_fit_mode::fit_mode::first_fit);
    
    ASSERT_THROW(static_cast<void>(allocator_instance->allocate(sizeof(char), 3000)), std::bad_alloc);
    
    delete allocator_instance;
    delete logger_instance;
}

int main(
    int argc,
    char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}