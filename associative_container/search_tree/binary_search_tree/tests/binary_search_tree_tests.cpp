#include <gtest/gtest.h>
#include <binary_search_tree.h>
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
        int second)
    {
        return first - second;
    }
    
    int operator()(
        std::string const &first,
        std::string const &second)
    {
        if (first > second)
        {
            return 1;
        }
        else if (first == second)
        {
            return 0;
        }
        return -1;
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
    binary_search_tree<tkey, tvalue> const &tree,
    std::vector<typename binary_search_tree<tkey, tvalue>::iterator_data> &expected_result)
{
    auto end_infix = tree.cend_infix();
    auto it = tree.cbegin_infix();
    
    for (auto const &item: expected_result)
    {
        if ((*it)->depth != item.depth || (*it)->get_key() != item.get_key() || (*it)->get_value() != item.get_value())
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
    binary_search_tree<tkey, tvalue> const &tree,
    std::vector<typename binary_search_tree<tkey, tvalue>::iterator_data> &expected_result)
{
    auto end_prefix = tree.cend_prefix();
    auto it = tree.cbegin_prefix();
    
    for (auto const &item: expected_result)
    {
        if ((*it)->depth != item.depth || (*it)->get_key() != item.get_key() || (*it)->get_value() != item.get_value())
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
    binary_search_tree<tkey, tvalue> const &tree,
    std::vector<typename binary_search_tree<tkey, tvalue>::iterator_data> &expected_result)
{
    std::string line;
    auto end_postfix = tree.end_postfix();
    auto it = tree.cbegin_postfix();
    
    for (auto const &item: expected_result)
    {
        if ((*it)->depth != item.depth || (*it)->get_key() != item.get_key() || (*it)->get_value() != item.get_value())
        {
            return false;
        }
        
        ++it;
    }
    
    return true;
}

TEST(binarySearchTreePositiveTests, test1)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test1 started");
    
    search_tree<int, std::string> *bst = new binary_search_tree<int, std::string>(key_comparer(), nullptr, logger);
    
    bst->insert(5, "a");
    bst->insert(2, "b");
    bst->insert(15, "c");
    bst->insert(3, "d");
    bst->insert(14, "e");
    bst->insert(1, "l");
    
    std::vector<typename binary_search_tree<int, std::string>::iterator_data> expected_result =
        {
            binary_search_tree<int, std::string>::iterator_data(2, 1, "l"),
            binary_search_tree<int, std::string>::iterator_data(1, 2, "b"),
            binary_search_tree<int, std::string>::iterator_data(2, 3, "d"),
            binary_search_tree<int, std::string>::iterator_data(0, 5, "a"),
            binary_search_tree<int, std::string>::iterator_data(2, 14, "e"),
            binary_search_tree<int, std::string>::iterator_data(1, 15, "c")
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<binary_search_tree<int, std::string> const *>(bst), expected_result));
    
    logger->trace("binarySearchTreePositiveTests.test1 finished");
    
    delete bst;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test2)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test2 started");
    
    search_tree<int, int> *bst = new binary_search_tree<int, int>(key_comparer(), nullptr, logger);
    
    bst->insert(1, 5);
    bst->insert(2, 12);
    bst->insert(15, 1);
    bst->insert(3, 67);
    bst->insert(4, 45);
    
    std::vector<typename binary_search_tree<int, int>::iterator_data> expected_result =
        {
            binary_search_tree<int, int>::iterator_data(0, 1, 5),
            binary_search_tree<int, int>::iterator_data(1, 2, 12),
            binary_search_tree<int, int>::iterator_data(2, 15, 1),
            binary_search_tree<int, int>::iterator_data(3, 3, 67),
            binary_search_tree<int, int>::iterator_data(4, 4, 45)
        };
    
    EXPECT_TRUE(prefix_iterator_test(*reinterpret_cast<binary_search_tree<int, int> const *>(bst), expected_result));
    
    logger->trace("binarySearchTreePositiveTests.test2 finished");
    
    delete bst;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test3)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test3 started");
    
    search_tree<std::string, int> *bst = new binary_search_tree<std::string, int>(key_comparer(), nullptr, logger);
    
    bst->insert("a", 1);
    bst->insert("b", 2);
    bst->insert("c", 15);
    bst->insert("d", 3);
    bst->insert("e", 4);
    
    std::vector<typename binary_search_tree<std::string, int>::iterator_data> expected_result =
        {
            binary_search_tree<std::string, int>::iterator_data(4, "e", 4),
            binary_search_tree<std::string, int>::iterator_data(3, "d", 3),
            binary_search_tree<std::string, int>::iterator_data(2, "c", 15),
            binary_search_tree<std::string, int>::iterator_data(1, "b", 2),
            binary_search_tree<std::string, int>::iterator_data(0, "a", 1)
        };
    
    EXPECT_TRUE(postfix_iterator_test(*reinterpret_cast<binary_search_tree<std::string, int> const *>(bst), expected_result));
    
    logger->trace("binarySearchTreePositiveTests.test3 finished");
    
    delete bst;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test4)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test4 started");
    
    search_tree<int, std::string> *bst1 = new binary_search_tree<int, std::string>(key_comparer(), nullptr, logger);
    
    bst1->insert(6, "a");
    bst1->insert(8, "c");
    bst1->insert(15, "x");
    bst1->insert(4, "j");
    bst1->insert(1, "i");
    bst1->insert(5, "b");
    
    std::vector<typename binary_search_tree<int, std::string>::iterator_data> expected_result =
        {
            binary_search_tree<int, std::string>::iterator_data(2, 1, "i"),
            binary_search_tree<int, std::string>::iterator_data(1, 4, "j"),
            binary_search_tree<int, std::string>::iterator_data(2, 5, "b"),
            binary_search_tree<int, std::string>::iterator_data(0, 6, "a"),
            binary_search_tree<int, std::string>::iterator_data(1, 8, "c"),
            binary_search_tree<int, std::string>::iterator_data(2, 15, "x")
        };
    
    binary_search_tree<int, std::string> bst2(std::move(*reinterpret_cast<binary_search_tree<int, std::string> *>(bst1)));
    
    EXPECT_TRUE(infix_iterator_test(bst2, expected_result));
    
    logger->trace("binarySearchTreePositiveTests.test4 finished");
    
    delete bst1;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test5)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test5 started");
    
    search_tree<int, std::string> *bst1 = new binary_search_tree<int, std::string>(key_comparer(), nullptr, logger);
    
    bst1->insert(6, "a");
    bst1->insert(8, "c");
    bst1->insert(15, "x");
    bst1->insert(4, "j");
    bst1->insert(1, "i");
    bst1->insert(5, "b");
    
    std::vector<typename binary_search_tree<int, std::string>::iterator_data> expected_result =
        {
            binary_search_tree<int, std::string>::iterator_data(2, 1, "i"),
            binary_search_tree<int, std::string>::iterator_data(1, 4, "j"),
            binary_search_tree<int, std::string>::iterator_data(2, 5, "b"),
            binary_search_tree<int, std::string>::iterator_data(0, 6, "a"),
            binary_search_tree<int, std::string>::iterator_data(1, 8, "c"),
            binary_search_tree<int, std::string>::iterator_data(2, 15, "x"),
        };
    
    binary_search_tree<int, std::string> bst2 = std::move(*reinterpret_cast<binary_search_tree<int, std::string> *>(bst1));
    EXPECT_TRUE(infix_iterator_test(bst2, expected_result));
    
    expected_result.clear();
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<binary_search_tree<int, std::string> const *>(bst1), expected_result));
    
    logger->trace("binarySearchTreePositiveTests.test5 finished");
    
    delete bst1;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test6)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test6 started");
    
    search_tree<int, std::string> *bst1 = new binary_search_tree<int, std::string>(key_comparer(), nullptr, logger);
    
    bst1->insert(6, "a");
    bst1->insert(8, "c");
    bst1->insert(15, "x");
    bst1->insert(4, "j");
    bst1->insert(1, "i");
    bst1->insert(5, "b");
    
    bst1->dispose(5);
    
    std::vector<typename binary_search_tree<int, std::string>::iterator_data> expected_result =
        {
            binary_search_tree<int, std::string>::iterator_data(2, 1, "i"),
            binary_search_tree<int, std::string>::iterator_data(1, 4, "j"),
            binary_search_tree<int, std::string>::iterator_data(0, 6, "a"),
            binary_search_tree<int, std::string>::iterator_data(1, 8, "c"),
            binary_search_tree<int, std::string>::iterator_data(2, 15, "x")
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<binary_search_tree<int, std::string> *>(bst1), expected_result));
    
    logger->trace("binarySearchTreePositiveTests.test6 finished");
    
    delete bst1;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test7)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test7 started");
    
    search_tree<int, std::string> *bst1 = new binary_search_tree<int, std::string>(key_comparer(), nullptr, logger);
    
    bst1->insert(6, "a");
    bst1->insert(8, "c");
    bst1->insert(15, "x");
    bst1->insert(4, "j");
    bst1->insert(3, "i");
    bst1->insert(2, "l");
    bst1->insert(5, "b");
    
    bst1->dispose(3);
    
    std::vector<typename binary_search_tree<int, std::string>::iterator_data> expected_result =
        {
            binary_search_tree<int, std::string>::iterator_data(2, 2, "l"),
            binary_search_tree<int, std::string>::iterator_data(1, 4, "j"),
            binary_search_tree<int, std::string>::iterator_data(2, 5, "b"),
            binary_search_tree<int, std::string>::iterator_data(0, 6, "a"),
            binary_search_tree<int, std::string>::iterator_data(1, 8, "c"),
            binary_search_tree<int, std::string>::iterator_data(2, 15, "x")
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<binary_search_tree<int, std::string> *>(bst1), expected_result));
    
    logger->trace("binarySearchTreePositiveTests.test7 finished");
    
    delete bst1;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test8)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test8 started");
    
    search_tree<int, std::string> *bst1 = new binary_search_tree<int, std::string>(key_comparer(), nullptr, logger);
    
    bst1->insert(6, "a");
    bst1->insert(8, "c");
    bst1->insert(15, "x");
    bst1->insert(11, "j");
    bst1->insert(19, "i");
    bst1->insert(12, "l");
    bst1->insert(17, "b");
    bst1->insert(18, "e");
    
    bst1->dispose(15);
    
    std::vector<typename binary_search_tree<int, std::string>::iterator_data> expected_result =
        {
            binary_search_tree<int, std::string>::iterator_data(0, 6, "a"),
            binary_search_tree<int, std::string>::iterator_data(1, 8, "c"),
            binary_search_tree<int, std::string>::iterator_data(3, 11, "j"),
            binary_search_tree<int, std::string>::iterator_data(2, 12, "l"),
            binary_search_tree<int, std::string>::iterator_data(4, 17, "b"),
            binary_search_tree<int, std::string>::iterator_data(5, 18, "e"),
            binary_search_tree<int, std::string>::iterator_data(3, 19, "i")
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<binary_search_tree<int, std::string> *>(bst1), expected_result));
    
    logger->trace("binarySearchTreePositiveTests.test8 finished");
    
    delete bst1;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test9)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test9 started");
    
    search_tree<int, std::string> *bst1 = new binary_search_tree<int, std::string>(key_comparer(), nullptr, logger);
    
    bst1->insert(6, "a");
    bst1->insert(8, "c");
    bst1->insert(15, "x");
    bst1->insert(11, "j");
    bst1->insert(19, "i");
    bst1->insert(12, "l");
    bst1->insert(17, "b");
    bst1->insert(18, "e");
    
    bst1->dispose(6);
    
    std::vector<typename binary_search_tree<int, std::string>::iterator_data> expected_result =
        {
            binary_search_tree<int, std::string>::iterator_data(0, 8, "c"),
            binary_search_tree<int, std::string>::iterator_data(2, 11, "j"),
            binary_search_tree<int, std::string>::iterator_data(3, 12, "l"),
            binary_search_tree<int, std::string>::iterator_data(1, 15, "x"),
            binary_search_tree<int, std::string>::iterator_data(3, 17, "b"),
            binary_search_tree<int, std::string>::iterator_data(4, 18, "e"),
            binary_search_tree<int, std::string>::iterator_data(2, 19, "i")
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<binary_search_tree<int, std::string> *>(bst1), expected_result));
    
    logger->trace("binarySearchTreePositiveTests.test9 finished");
    
    delete bst1;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test10)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    logger->trace("binarySearchTreePositiveTests.test10 started");
    
    search_tree<int, std::string> *bst1 = new binary_search_tree<int, std::string>(key_comparer(), nullptr, logger);
    
    bst1->insert(6, "l");
    bst1->insert(8, "c");
    bst1->insert(15, "l");
    bst1->insert(11, "o");
    bst1->insert(9, "h");
    bst1->insert(2, "e");
    bst1->insert(4, "b");
    bst1->insert(18, "e");
    
    std::vector<std::string> vector;
    
    vector.push_back(bst1->obtain(9));
    vector.push_back(bst1->obtain(2));
    vector.push_back(bst1->obtain(15));
    vector.push_back(bst1->obtain(6));
    vector.push_back(bst1->obtain(11));
    
    std::string actual_result;
    
    for (auto const &item: vector)
    {
        actual_result.append(item + " ");
    }
    
    EXPECT_EQ(actual_result, "h e l l o ");
    
    logger->trace("binarySearchTreePositiveTests.test10 finished");
    
    delete bst1;
    delete logger;
}

TEST(binarySearchTreePositiveTests, test11)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "binary_search_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    
    logger->trace("binarySearchTreePositiveTests.test11 started");
    
    search_tree<int, std::string> *bst1 = new binary_search_tree<int, std::string>(key_comparer(), nullptr, logger);
    
    bst1->insert(6, "l");
    bst1->insert(8, "c");
    bst1->insert(15, "l");
    bst1->insert(11, "o");
    bst1->insert(9, "h");
    bst1->insert(2, "e");
    bst1->insert(4, "b");
    bst1->insert(18, "e");
    
    std::vector<associative_container<int, std::string>::key_value_pair> actual_result = bst1->obtain_between(2, 10, true, false);
    
    std::vector<associative_container<int, std::string>::key_value_pair> expected_result =
        {
            { 2, "e" },
            { 4, "b" },
            { 6, "l" },
            { 8, "c" },
            { 9, "h" }
        };
    
    EXPECT_TRUE(compare_results(expected_result, actual_result));
    
    logger->trace("binarySearchTreePositiveTests.test11 finished");
    
    delete bst1;
    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}