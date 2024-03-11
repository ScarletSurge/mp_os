#include "gtest/gtest.h"
#include <splay_tree.h>
#include <associative_container.h>
#include <logger_builder.h>
#include <client_logger_builder.h>
#include <iostream>

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

class key_comparer final
{

public:
    
    int operator()(
        int first,
        int second) const
    {
        return first - second;
    }
    
    int operator()(
        std::string const &first,
        std::string const &second) const
    {
        if (first > second)
        {
            return 1;
        }
        else if (first < second)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
    
};

bool compare_results(
    std::vector<typename associative_container<int, std::string>::key_value_pair> &expected,
    std::vector<typename associative_container<int, std::string>::key_value_pair> &actual)
{
    key_comparer comparer;
    
    if (expected.size() != actual.size())
    {
        return false;
    }
    
    for (size_t i = 0; i < expected.size(); ++i)
    {
        if (comparer(expected[i].key, actual[i].key))
        {
            return false;
        }
        
        if (expected[i].value != actual[i].value)
        {
            return false;
        }
    }
    
    return true;
}

template<
    typename tkey,
    typename tvalue>
bool infix_iterator_test(
    splay_tree<tkey, tvalue> const &tree,
    std::vector<typename splay_tree<tkey, tvalue>::iterator_data> &expected_result)
{
    
    std::string line;
    auto end_infix = tree.end_infix();
    auto it = tree.cbegin_infix();
    
    for (auto const &item: expected_result)
    {
        if ((*it)->depth != item.depth || (*it)->key != item.key || (*it)->value != item.value)
        {
            return false;
        }
        ++it;
    }
    return true;
}

template<
    typename tkey,
    typename tvalue>
bool prefix_iterator_test(
    splay_tree<tkey, tvalue> const &tree,
    std::vector<typename splay_tree<tkey, tvalue>::iterator_data> &expected_result)
{
    std::string line;
    auto end_prefix = tree.end_prefix();
    auto it = tree.cbegin_prefix();
    
    for (auto const &item: expected_result)
    {
        if ((*it)->depth != item.depth || (*it)->key != item.key || (*it)->value != item.value)
        {
            return false;
        }
        ++it;
    }
    return true;
}

template<
    typename tkey,
    typename tvalue>
bool postfix_iterator_test(
    splay_tree<tkey, tvalue> const &tree,
    std::vector<typename splay_tree<tkey, tvalue>::iterator_data> &expected_result)
{
    
    std::string line;
    auto end_postfix = tree.end_postfix();
    auto it = tree.cbegin_postfix();
    
    for (auto const &item: expected_result)
    {
        if ((*it)->depth != item.depth || (*it)->key != item.key || (*it)->value != item.value)
        {
            return false;
        }
        ++it;
    }
    return true;
}

TEST(splayTreePositiveTests, test1)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("splayTreePositiveTests.test1 started");
    
    search_tree<int, std::string> *splay = new splay_tree<int, std::string>(nullptr, logger);
    
    splay->insert(5, "a");
    splay->insert(2, "b");
    splay->insert(15, "c");
    splay->insert(3, "d");
    splay->insert(14, "e");
    splay->insert(1, "l");
    
    std::vector<typename splay_tree<int, std::string>::iterator_data> expected_result =
        {
            splay_tree<int, std::string>::iterator_data(0, 1, "l"),
            splay_tree<int, std::string>::iterator_data(2, 2, "b"),
            splay_tree<int, std::string>::iterator_data(3, 3, "d"),
            splay_tree<int, std::string>::iterator_data(4, 5, "a"),
            splay_tree<int, std::string>::iterator_data(1, 14, "e"),
            splay_tree<int, std::string>::iterator_data(2, 15, "c")
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<splay_tree<int, std::string> *>(splay), expected_result));
    
    logger->trace("splayTreePositiveTests.test1 finished");
    
    delete splay;
    delete logger;
}

TEST(splayTreePositiveTests, test2)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("splayTreePositiveTests.test2 started");
    
    search_tree<int, int> *splay = new splay_tree<int, int>(nullptr, logger);
    
    splay->insert(1, 5);
    splay->insert(2, 12);
    splay->insert(15, 1);
    splay->insert(3, 67);
    splay->insert(4, 45);
    
    std::vector<typename splay_tree<int, int>::iterator_data> expected_result =
        {
            splay_tree<int, int>::iterator_data(0, 4, 45),
            splay_tree<int, int>::iterator_data(3, 1, 5),
            splay_tree<int, int>::iterator_data(2, 2, 12),
            splay_tree<int, int>::iterator_data(1, 3, 67),
            splay_tree<int, int>::iterator_data(1, 15, 1)
        };
    
    EXPECT_TRUE(prefix_iterator_test(*reinterpret_cast<splay_tree<int, int> *>(splay), expected_result));
    
    logger->trace("splayTreePositiveTests.test2 finished");
    
    delete splay;
    delete logger;
}

TEST(splayTreePositiveTests, test3)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("splayTreePositiveTests.test3 started");
    
    search_tree<std::string, int> *splay = new splay_tree<std::string, int>(nullptr, logger);
    
    splay->insert("a", 1);
    splay->insert("b", 2);
    splay->insert("c", 15);
    splay->insert("d", 3);
    splay->insert("e", 4);
    
    std::vector<typename splay_tree<std::string, int>::iterator_data> expected_result =
        {
            splay_tree<std::string, int>::iterator_data(4, "a", 1),
            splay_tree<std::string, int>::iterator_data(3, "b", 2),
            splay_tree<std::string, int>::iterator_data(2, "c", 15),
            splay_tree<std::string, int>::iterator_data(1, "d", 3),
            splay_tree<std::string, int>::iterator_data(0, "e", 4)
        };
    
    EXPECT_TRUE(postfix_iterator_test(*reinterpret_cast<splay_tree<std::string, int> *>(splay), expected_result));
    
    logger->trace("splayTreePositiveTests.test3 finished");
    
    delete splay;
    delete logger;
}

TEST(splayTreePositiveTests, test4)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("splayTreePositiveTests.test4 started");
    
    search_tree<int, std::string> *splay1 = new splay_tree<int, std::string>(nullptr, logger);
    
    splay1->insert(6, "a");
    splay1->insert(8, "c");
    splay1->insert(15, "x");
    splay1->insert(4, "j");
    splay1->insert(1, "i");
    splay1->insert(5, "b");
    
    std::vector<typename splay_tree<int, std::string>::iterator_data> expected_result =
        {
            splay_tree<int, std::string>::iterator_data(2, 1, "i"),
            splay_tree<int, std::string>::iterator_data(1, 4, "j"),
            splay_tree<int, std::string>::iterator_data(0, 5, "b"),
            splay_tree<int, std::string>::iterator_data(1, 6, "a"),
            splay_tree<int, std::string>::iterator_data(3, 8, "c"),
            splay_tree<int, std::string>::iterator_data(2, 15, "x")
        };
    
    splay_tree<int, std::string> splay2(std::move(*reinterpret_cast<splay_tree<int, std::string> *>(splay1)));
    
    EXPECT_TRUE(infix_iterator_test(splay2, expected_result));
    
    logger->trace("splayTreePositiveTests.test4 finished");
    
    delete splay1;
    delete logger;
}

TEST(splayTreePositiveTests, test5)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("splayTreePositiveTests.test5 started");
    
    search_tree<int, std::string> *splay1 = new splay_tree<int, std::string>(nullptr, logger);
    
    splay1->insert(6, "a");
    splay1->insert(8, "c");
    splay1->insert(15, "x");
    splay1->insert(4, "j");
    splay1->insert(1, "i");
    splay1->insert(5, "b");
    
    std::vector<typename splay_tree<int, std::string>::iterator_data> expected_result =
        {
            splay_tree<int, std::string>::iterator_data(2, 1, "i"),
            splay_tree<int, std::string>::iterator_data(1, 4, "j"),
            splay_tree<int, std::string>::iterator_data(0, 5, "b"),
            splay_tree<int, std::string>::iterator_data(1, 6, "a"),
            splay_tree<int, std::string>::iterator_data(3, 8, "c"),
            splay_tree<int, std::string>::iterator_data(2, 15, "x")
        };
    
    splay_tree<int, std::string> splay2 = std::move(*reinterpret_cast<splay_tree<int, std::string> *>(splay1));
    
    EXPECT_TRUE(infix_iterator_test(splay2, expected_result));
    
    logger->trace("splayTreePositiveTests.test5 finished");
    
    delete splay1;
    delete logger;
}

TEST(splayTreePositiveTests, test6)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("splayTreePositiveTests.test6 started");
    
    search_tree<int, std::string> *splay1 = new splay_tree<int, std::string>(nullptr, logger);
    
    splay1->insert(6, "a");
    splay1->insert(8, "c");
    splay1->insert(15, "x");
    splay1->insert(4, "j");
    splay1->insert(1, "i");
    splay1->insert(5, "b");
    
    splay1->dispose(5);
    
    std::vector<typename splay_tree<int, std::string>::iterator_data> expected_result =
        {
            splay_tree<int, std::string>::iterator_data(1, 1, "i"),
            splay_tree<int, std::string>::iterator_data(0, 4, "j"),
            splay_tree<int, std::string>::iterator_data(1, 6, "a"),
            splay_tree<int, std::string>::iterator_data(3, 8, "c"),
            splay_tree<int, std::string>::iterator_data(2, 15, "x"),
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<splay_tree<int, std::string> *>(splay1), expected_result));
    
    logger->trace("splayTreePositiveTests.test6 finished");
    
    delete splay1;
    delete logger;
}

TEST(splayTreePositiveTests, test7)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("splayTreePositiveTests.test7 started");
    
    search_tree<int, std::string> *splay1 = new splay_tree<int, std::string>(nullptr, logger);
    
    splay1->insert(6, "a");
    splay1->insert(8, "c");
    splay1->insert(15, "x");
    splay1->insert(4, "j");
    splay1->insert(3, "i");
    splay1->insert(2, "l");
    splay1->insert(5, "b");
    
    splay1->dispose(3);
    
    std::vector<typename splay_tree<int, std::string>::iterator_data> expected_result =
        {
            splay_tree<int, std::string>::iterator_data(0, 2, "l"),
            splay_tree<int, std::string>::iterator_data(2, 4, "j"),
            splay_tree<int, std::string>::iterator_data(1, 5, "b"),
            splay_tree<int, std::string>::iterator_data(2, 6, "a"),
            splay_tree<int, std::string>::iterator_data(4, 8, "c"),
            splay_tree<int, std::string>::iterator_data(3, 15, "x")
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<splay_tree<int, std::string> *>(splay1),
        expected_result));
    
    logger->trace("splayTreePositiveTests.test7 finished");
    
    delete splay1;
    delete logger;
    
}

TEST(splayTreePositiveTests, test8)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("splayTreePositiveTests.test8 started");
    
    search_tree<int, std::string> *splay1 = new splay_tree<int, std::string>(nullptr, logger);
    
    splay1->insert(6, "a");
    splay1->insert(8, "c");
    splay1->insert(15, "x");
    splay1->insert(11, "j");
    splay1->insert(19, "i");
    splay1->insert(12, "l");
    splay1->insert(17, "b");
    splay1->insert(18, "e");
    
    splay1->dispose(12);
    
    std::vector<typename splay_tree<int, std::string>::iterator_data> expected_result =
        {
            splay_tree<int, std::string>::iterator_data(2, 6, "a"),
            splay_tree<int, std::string>::iterator_data(1, 8, "c"),
            splay_tree<int, std::string>::iterator_data(0, 11, "j"),
            splay_tree<int, std::string>::iterator_data(2, 15, "x"),
            splay_tree<int, std::string>::iterator_data(1, 17, "b"),
            splay_tree<int, std::string>::iterator_data(2, 18, "e"),
            splay_tree<int, std::string>::iterator_data(3, 19, "i"),
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<splay_tree<int, std::string> *>(splay1), expected_result));
    
    logger->trace("splayTreePositiveTests.test8 finished");
    
    delete splay1;
    delete logger;
    
}

TEST(splayTreePositiveTests, test9)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("splayTreePositiveTests.test9 started");
    
    search_tree<int, std::string> *splay1 = new splay_tree<int, std::string>(nullptr, logger);
    
    splay1->insert(6, "l");
    splay1->insert(8, "c");
    splay1->insert(15, "l");
    splay1->insert(11, "o");
    splay1->insert(9, "h");
    splay1->insert(2, "e");
    splay1->insert(4, "b");
    splay1->insert(18, "e");
    
    std::vector<std::string> vector;
    
    vector.push_back(splay1->obtain(9));
    vector.push_back(splay1->obtain(2));
    vector.push_back(splay1->obtain(15));
    vector.push_back(splay1->obtain(6));
    vector.push_back(splay1->obtain(11));
    
    std::string actual_result;
    
    for (auto const &item: vector)
    {
        actual_result.append(item + " ");
    }
    
    EXPECT_EQ(actual_result, "h e l l o ");
    
    logger->trace("splayTreePositiveTests.test9 finished");
    
    delete splay1;
    delete logger;
    
}

TEST(splayTreePositiveTests, test10)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "splay_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    
    logger->trace("splayTreePositiveTests.test10 started");
    
    search_tree<int, std::string> *splay = new splay_tree<int, std::string>(nullptr, logger);
    
    splay->insert(6, "l");
    splay->insert(8, "c");
    splay->insert(15, "l");
    splay->insert(11, "o");
    splay->insert(9, "h");
    splay->insert(2, "e");
    splay->insert(4, "b");
    splay->insert(18, "e");
    
    std::vector<associative_container<int, std::string>::key_value_pair> actual_result = splay->obtain_between(2, 10, true, false);
    
    std::vector<associative_container<int, std::string>::key_value_pair> expected_result =
        {
            { 2, "e" },
            { 4, "b" },
            { 6, "l" },
            { 8, "c" },
            { 9, "h" }
        };
    
    EXPECT_TRUE(compare_results(expected_result, actual_result));
    
    logger->trace("splayTreePositiveTests.test10 finished");
    
    delete splay;
    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}