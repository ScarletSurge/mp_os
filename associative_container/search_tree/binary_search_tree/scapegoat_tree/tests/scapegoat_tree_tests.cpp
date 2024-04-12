#include "gtest/gtest.h"
#include <scapegoat_tree.h>
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
    scapegoat_tree<tkey, tvalue> const &tree,
    std::vector<typename scapegoat_tree<tkey, tvalue>::iterator_data> &expected_result)
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
    scapegoat_tree<tkey, tvalue> const &tree,
    std::vector<typename scapegoat_tree<tkey, tvalue>::iterator_data> &expected_result)
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
    scapegoat_tree<tkey, tvalue> const &tree,
    std::vector<typename scapegoat_tree<tkey, tvalue>::iterator_data> &expected_result)
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

TEST(scapegoatTreePositiveTests, test1)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("scapegoatTreePositiveTests.test1 started");
    
    search_tree<int, std::string> *sg = new scapegoat_tree<int, std::string>(nullptr, logger, 0.7);
    
    sg->insert(5, "a");
    sg->insert(2, "b");
    sg->insert(15, "c");
    sg->insert(3, "d");
    sg->insert(14, "e");
    sg->insert(1, "l");
    
    std::vector<typename scapegoat_tree<int, std::string>::iterator_data> expected_result =
        {
            scapegoat_tree<int, std::string>::iterator_data(2, 1, "l"),
            scapegoat_tree<int, std::string>::iterator_data(1, 2, "b"),
            scapegoat_tree<int, std::string>::iterator_data(2, 3, "d"),
            scapegoat_tree<int, std::string>::iterator_data(0, 5, "a"),
            scapegoat_tree<int, std::string>::iterator_data(2, 14, "e"),
            scapegoat_tree<int, std::string>::iterator_data(1, 15, "c")
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<scapegoat_tree<int, std::string> *>(sg), expected_result));
    
    logger->trace("scapegoatTreePositiveTests.test1 finished");
    
    delete sg;
    delete logger;
}

TEST(scapegoatTreePositiveTests, test2)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("scapegoatTreePositiveTests.test2 started");
    
    search_tree<int, int> *sg = new scapegoat_tree<int, int>(nullptr, logger, 0.5);
    
    sg->insert(1, 5);
    sg->insert(2, 12);
    sg->insert(15, 1);
    sg->insert(3, 67);
    sg->insert(4, 45);
    
    std::vector<typename scapegoat_tree<int, int>::iterator_data> expected_result =
        {
            scapegoat_tree<int, int>::iterator_data(0, 2, 12),
            scapegoat_tree<int, int>::iterator_data(1, 1, 5),
            scapegoat_tree<int, int>::iterator_data(1, 4, 45),
            scapegoat_tree<int, int>::iterator_data(2, 3, 67),
            scapegoat_tree<int, int>::iterator_data(2, 15, 1)
        };
    
    EXPECT_TRUE(prefix_iterator_test(*reinterpret_cast<scapegoat_tree<int, int> *>(sg), expected_result));
    
    logger->trace("scapegoatTreePositiveTests.test2 finished");
    
    delete sg;
    delete logger;
}

TEST(scapegoatTreePositiveTests, test3)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("scapegoatTreePositiveTests.test3 started");
    
    search_tree<std::string, int> *sg = new scapegoat_tree<std::string, int>(nullptr, logger, 0.9);
    
    sg->insert("a", 1);
    sg->insert("b", 2);
    sg->insert("c", 15);
    sg->insert("d", 3);
    sg->insert("e", 4);
    
    std::vector<typename scapegoat_tree<std::string, int>::iterator_data> expected_result =
        {
            scapegoat_tree<std::string, int>::iterator_data(4, "e", 4),
            scapegoat_tree<std::string, int>::iterator_data(3, "d", 3),
            scapegoat_tree<std::string, int>::iterator_data(2, "c", 15),
            scapegoat_tree<std::string, int>::iterator_data(1, "b", 2),
            scapegoat_tree<std::string, int>::iterator_data(0, "a", 1)
        };
    
    EXPECT_TRUE(postfix_iterator_test(*reinterpret_cast<scapegoat_tree<std::string, int> *>(sg), expected_result));
    
    logger->trace("scapegoatTreePositiveTests.test3 finished");
    
    delete sg;
    delete logger;
}

TEST(scapegoatTreePositiveTests, test4)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("scapegoatTreePositiveTests.test4 started");
    
    search_tree<int, std::string> *sg1 = new scapegoat_tree<int, std::string>(nullptr, logger, 0.65);
    
    sg1->insert(6, "a");
    sg1->insert(8, "c");
    sg1->insert(15, "x");
    sg1->insert(4, "j");
    sg1->insert(1, "i");
    sg1->insert(5, "b");
    
    std::vector<typename scapegoat_tree<int, std::string>::iterator_data> expected_result =
        {
            scapegoat_tree<int, std::string>::iterator_data(2, 1, "i"),
            scapegoat_tree<int, std::string>::iterator_data(1, 4, "j"),
            scapegoat_tree<int, std::string>::iterator_data(2, 5, "b"),
            scapegoat_tree<int, std::string>::iterator_data(0, 6, "a"),
            scapegoat_tree<int, std::string>::iterator_data(2, 8, "c"),
            scapegoat_tree<int, std::string>::iterator_data(1, 15, "x"),
        };
    
    scapegoat_tree<int, std::string> sg2(std::move(*reinterpret_cast<scapegoat_tree<int, std::string> *>(sg1)));
    
    EXPECT_TRUE(infix_iterator_test(sg2, expected_result));
    
    logger->trace("scapegoatTreePositiveTests.test4 finished");
    
    delete sg1;
    delete logger;
}

TEST(scapegoatTreePositiveTests, test5)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("scapegoatTreePositiveTests.test5 started");
    
    search_tree<int, std::string> *sg1 = new scapegoat_tree<int, std::string>(nullptr, logger, 0.65);
    
    sg1->insert(6, "a");
    sg1->insert(8, "c");
    sg1->insert(15, "x");
    sg1->insert(4, "j");
    sg1->insert(1, "i");
    sg1->insert(5, "b");
    
    std::vector<typename scapegoat_tree<int, std::string>::iterator_data> expected_result =
        {
            scapegoat_tree<int, std::string>::iterator_data(2, 1, "i"),
            scapegoat_tree<int, std::string>::iterator_data(1, 4, "j"),
            scapegoat_tree<int, std::string>::iterator_data(2, 5, "b"),
            scapegoat_tree<int, std::string>::iterator_data(0, 6, "a"),
            scapegoat_tree<int, std::string>::iterator_data(2, 8, "c"),
            scapegoat_tree<int, std::string>::iterator_data(1, 15, "x"),
        };
    
    scapegoat_tree<int, std::string> sg2 = std::move(
        *reinterpret_cast<scapegoat_tree<int, std::string> *>(sg1));
    
    EXPECT_TRUE(infix_iterator_test(sg2, expected_result));
    
    logger->trace("scapegoatTreePositiveTests.test5 finished");
    
    delete sg1;
    delete logger;
}

TEST(scapegoatTreePositiveTests, test6)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("scapegoatTreePositiveTests.test6 started");
    
    search_tree<int, std::string> *sg1 = new scapegoat_tree<int, std::string>(nullptr, logger, 0.5);
    
    sg1->insert(6, "a");
    sg1->insert(8, "c");
    sg1->insert(15, "x");
    sg1->insert(4, "j");
    sg1->insert(1, "i");
    sg1->insert(5, "b");
    
    sg1->dispose(5);
    
    std::vector<typename scapegoat_tree<int, std::string>::iterator_data> expected_result =
        {
            scapegoat_tree<int, std::string>::iterator_data(2, 1, "i"),
            scapegoat_tree<int, std::string>::iterator_data(1, 4, "j"),
            scapegoat_tree<int, std::string>::iterator_data(0, 6, "a"),
            scapegoat_tree<int, std::string>::iterator_data(2, 8, "c"),
            scapegoat_tree<int, std::string>::iterator_data(1, 15, "x"),
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<scapegoat_tree<int, std::string> *>(sg1), expected_result));
    
    logger->trace("scapegoatTreePositiveTests.test6 finished");
    
    delete sg1;
    delete logger;
}

TEST(scapegoatTreePositiveTests, test7)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("scapegoatTreePositiveTests.test7 started");
    
    search_tree<int, std::string> *sg1 = new scapegoat_tree<int, std::string>(nullptr, logger, 0.7);
    
    sg1->insert(6, "a");
    sg1->insert(8, "c");
    sg1->insert(15, "x");
    sg1->insert(4, "j");
    sg1->insert(3, "i");
    sg1->insert(2, "l");
    sg1->insert(5, "b");
    
    sg1->dispose(3);
    
    std::vector<typename scapegoat_tree<int, std::string>::iterator_data> expected_result =
        {
            scapegoat_tree<int, std::string>::iterator_data(2, 2, "l"),
            scapegoat_tree<int, std::string>::iterator_data(1, 4, "j"),
            scapegoat_tree<int, std::string>::iterator_data(2, 5, "b"),
            scapegoat_tree<int, std::string>::iterator_data(0, 6, "a"),
            scapegoat_tree<int, std::string>::iterator_data(1, 8, "c"),
            scapegoat_tree<int, std::string>::iterator_data(2, 15, "x"),
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<scapegoat_tree<int, std::string> *>(sg1), expected_result));
    
    logger->trace("scapegoatTreePositiveTests.test7 finished");
    
    delete sg1;
    delete logger;
    
}

TEST(scapegoatTreePositiveTests, test8)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("scapegoatTreePositiveTests.test8 started");
    
    search_tree<int, std::string> *sg1 = new scapegoat_tree<int, std::string>(nullptr, logger);
    
    sg1->insert(6, "a");
    sg1->insert(8, "c");
    sg1->insert(15, "x");
    sg1->insert(11, "j");
    sg1->insert(19, "i");
    sg1->insert(12, "l");
    sg1->insert(17, "b");
    sg1->insert(18, "e");
    
    sg1->dispose(12);
    sg1->dispose(15);
    
    std::vector<typename scapegoat_tree<int, std::string>::iterator_data> expected_result =
        {
            scapegoat_tree<int, std::string>::iterator_data(2, 6, "a"),
            scapegoat_tree<int, std::string>::iterator_data(1, 8, "c"),
            scapegoat_tree<int, std::string>::iterator_data(2, 11, "j"),
            scapegoat_tree<int, std::string>::iterator_data(0, 17, "b"),
            scapegoat_tree<int, std::string>::iterator_data(2, 18, "e"),
            scapegoat_tree<int, std::string>::iterator_data(1, 19, "i"),
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<scapegoat_tree<int, std::string> *>(sg1), expected_result));
    
    logger->trace("scapegoatTreePositiveTests.test8 finished");
    
    delete sg1;
    delete logger;
}

TEST(scapegoatTreePositiveTests, test9)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("scapegoatTreePositiveTests.test9 started");
    
    search_tree<int, std::string> *sg1 = new scapegoat_tree<int, std::string>(nullptr, logger);
    
    sg1->insert(6, "l");
    sg1->insert(8, "c");
    sg1->insert(15, "l");
    sg1->insert(11, "o");
    sg1->insert(9, "h");
    sg1->insert(2, "e");
    sg1->insert(4, "b");
    sg1->insert(18, "e");
    
    std::vector<std::string> vector;
    
    vector.push_back(sg1->obtain(9));
    vector.push_back(sg1->obtain(2));
    vector.push_back(sg1->obtain(15));
    vector.push_back(sg1->obtain(6));
    vector.push_back(sg1->obtain(11));
    
    std::string actual_result;
    
    for (auto const &item: vector)
    {
        actual_result.append(item + " ");
    }
    
    EXPECT_EQ(actual_result, "h e l l o ");
    
    logger->trace("scapegoatTreePositiveTests.test9 finished");
    
    delete sg1;
    delete logger;
    
}

TEST(scapegoatTreePositiveTests, test10)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "scapegoat_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    
    logger->trace("scapegoatTreePositiveTests.test10 started");
    
    search_tree<int, std::string> *sg = new scapegoat_tree<int, std::string>(nullptr, logger);
    
    sg->insert(6, "l");
    sg->insert(8, "c");
    sg->insert(15, "l");
    sg->insert(11, "o");
    sg->insert(9, "h");
    sg->insert(2, "e");
    sg->insert(4, "b");
    sg->insert(18, "e");
    
    std::vector<associative_container<int, std::string>::key_value_pair> actual_result = sg->obtain_between(2, 10, true, false);
    
    std::vector<associative_container<int, std::string>::key_value_pair> expected_result =
        {
            { 2, "e" },
            { 4, "b" },
            { 6, "l" },
            { 8, "c" },
            { 9, "h" }
        };
    
    EXPECT_TRUE(compare_results(expected_result, actual_result));
    
    logger->trace("scapegoatTreePositiveTests.test10 finished");
    
    delete sg;
    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}