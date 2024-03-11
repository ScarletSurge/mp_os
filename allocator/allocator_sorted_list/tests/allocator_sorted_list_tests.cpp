#include <gtest/gtest.h>
#include <logger.h>
#include <logger_builder.h>
#include <client_logger_builder.h>
#include <list>

#include "../include/allocator_sorted_list.h"

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    logger_builder *builder = new client_logger_builder();
    
    if (use_console_stream)
    {
        builder->add_console_stream(console_stream_severity);
    }
    
    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        builder->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }
    
    logger *built_logger = builder->build();
    
    delete builder;
    
    return built_logger;
}

TEST(allocatorSortedListPositiveTests, test1)
{
    //TODO: logger
    
    allocator *alloc = new allocator_sorted_list(3000, nullptr, nullptr, allocator_with_fit_mode::fit_mode::first_fit);
    
    auto first_block = reinterpret_cast<int *>(alloc->allocate(sizeof(int), 250));
    
    auto second_block = reinterpret_cast<char *>(alloc->allocate(sizeof(int), 250));
    alloc->deallocate(first_block);
    
    first_block = reinterpret_cast<int *>(alloc->allocate(sizeof(int), 245));
    
    alloc->deallocate(second_block);
    alloc->deallocate(first_block);
    
    //TODO: Проверка
    
    delete alloc;
}

TEST(allocatorSortedListPositiveTests, test2)
{
    
    //TODO: logger
    
    allocator *alloc = new allocator_sorted_list(3000, nullptr, nullptr,
        allocator_with_fit_mode::fit_mode::the_worst_fit);
    
    auto first_block = reinterpret_cast<int *>(alloc->allocate(sizeof(int), 250));
    
    auto *the_same_subject = dynamic_cast<allocator_with_fit_mode *>(alloc);
    the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::first_fit);
    auto second_block = reinterpret_cast<char *>(alloc->allocate(sizeof(char), 500));
    the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_best_fit);
    auto third_block = reinterpret_cast<double *>(alloc->allocate(sizeof(double *), 100));
    
    alloc->deallocate(first_block);
    alloc->deallocate(second_block);
    alloc->deallocate(third_block);
    
    //TODO: проверка
    
    delete alloc;
}

TEST(allocatorSortedListPositiveTests, test3)
{
    //TODO: logger
    allocator *allocator = new allocator_sorted_list(5000, nullptr, nullptr, allocator_with_fit_mode::fit_mode::first_fit);
    
    int iterations_count = 100;
    
    std::list<void *> allocated_blocks;
    srand((unsigned)time(nullptr));
    
    for (auto i = 0; i < iterations_count; i++)
    {
        switch (rand() % 2)
        {
            case 0:
            case 1:
                try
                {
                    allocated_blocks.push_front(allocator->allocate(sizeof(void *), rand() % 251 + 50));
                    std::cout << "allocation succeeded" << std::endl;
                }
                catch (std::bad_alloc const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
            case 2:
                if (allocated_blocks.empty())
                {
                    std::cout << "No blocks to deallocate" << std::endl;
                    
                    break;
                }
                
                auto it = allocated_blocks.begin();
                std::advance(it, rand() % allocated_blocks.size());
                allocator->deallocate(*it);
                allocated_blocks.erase(it);
                std::cout << "deallocation succeeded" << std::endl;
                break;
        }
    }
    
    while (!allocated_blocks.empty())
    {
        auto it = allocated_blocks.begin();
        std::advance(it, rand() % allocated_blocks.size());
        allocator->deallocate(*it);
        allocated_blocks.erase(it);
        std::cout << "deallocation succeeded" << std::endl;
    }
    
    //TODO: проверка
    
    delete allocator;
    // delete logger;
    
    
}

TEST(allocatorSortedListPositiveTests, test4)
{
    //TODO: logger
    
    allocator *alloc = new allocator_sorted_list(1000, nullptr, nullptr, allocator_with_fit_mode::fit_mode::first_fit);
    
    auto first_block = reinterpret_cast<unsigned char *>(alloc->allocate(sizeof(unsigned char), 250));
    auto second_block = reinterpret_cast<unsigned char *>(alloc->allocate(sizeof(char), 150));
    auto third_block = reinterpret_cast<unsigned char *>(alloc->allocate(sizeof(unsigned char *), 300));
    
    auto *the_same_subject = dynamic_cast<allocator_with_fit_mode *>(alloc);
    the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_worst_fit);
    auto four_block = reinterpret_cast<unsigned char *>(alloc->allocate(sizeof(unsigned char *), 50));
    
    the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_best_fit);
    auto five_block = reinterpret_cast<unsigned char *>(alloc->allocate(sizeof(unsigned char *), 50));
    
    alloc->deallocate(first_block);
    alloc->deallocate(second_block);
    alloc->deallocate(third_block);
    alloc->deallocate(four_block);
    alloc->deallocate(five_block);
    
    //TODO: проверка
    
    delete alloc;
}

TEST(allocatorSortedListPositiveTests, test5)
{
    allocator *alloc = new allocator_sorted_list(3000, nullptr, nullptr, allocator_with_fit_mode::fit_mode::first_fit);
    
    auto first_block = reinterpret_cast<int *>(alloc->allocate(sizeof(int), 250));
    auto second_block = reinterpret_cast<char *>(alloc->allocate(sizeof(char), 500));
    auto third_block = reinterpret_cast<double *>(alloc->allocate(sizeof(double *), 250));
    alloc->deallocate(first_block);
    first_block = reinterpret_cast<int *>(alloc->allocate(sizeof(int), 245));
    
    //TODO: logger
    allocator *allocator = new allocator_sorted_list(5000, nullptr, nullptr, allocator_with_fit_mode::fit_mode::first_fit);
    auto *the_same_subject = dynamic_cast<allocator_with_fit_mode *>(alloc);
    int iterations_count = 100;
    
    std::list<void *> allocated_blocks;
    srand((unsigned)time(nullptr));
    
    for (auto i = 0; i < iterations_count; i++)
    {
        switch (rand() % 2)
        {
            case 0:
            case 1:
                try
                {
                    switch (rand() % 2)
                    {
                        case 0:
                            the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::first_fit);
                        case 1:
                            the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_best_fit);
                        case 2:
                            the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_worst_fit);
                    }
                    
                    allocated_blocks.push_front(allocator->allocate(sizeof(void *), rand() % 251 + 50));
                    std::cout << "allocation succeeded" << std::endl;
                }
                catch (std::bad_alloc const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
            case 2:
                if (allocated_blocks.empty())
                {
                    std::cout << "No blocks to deallocate" << std::endl;
                    
                    break;
                }
                
                auto it = allocated_blocks.begin();
                std::advance(it, rand() % allocated_blocks.size());
                allocator->deallocate(*it);
                allocated_blocks.erase(it);
                std::cout << "deallocation succeeded" << std::endl;
                break;
        }
    }
    
    while (!allocated_blocks.empty())
    {
        auto it = allocated_blocks.begin();
        std::advance(it, rand() % allocated_blocks.size());
        allocator->deallocate(*it);
        allocated_blocks.erase(it);
        std::cout << "deallocation succeeded" << std::endl;
    }
    
    //TODO: проверка
    
    delete allocator;
    // delete logger;
    
    delete alloc;
}


//TODO: Тесты на особенность аллокатора?

TEST(allocatorSortedListNegativeTests, test1)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "allocator_sorted_list_tests_logs_negative_test_1.txt",
                logger::severity::information
            }
        });
    allocator *alloc = new allocator_sorted_list(3000, nullptr, logger, allocator_with_fit_mode::fit_mode::first_fit);
    
    ASSERT_THROW(alloc->allocate(sizeof(char), 3100), std::bad_alloc);
    
    delete alloc;
    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}