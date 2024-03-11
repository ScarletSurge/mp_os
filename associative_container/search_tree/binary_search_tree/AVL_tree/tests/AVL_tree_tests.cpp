#include <gtest/gtest.h>
#include <AVL_tree.h>
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
    AVL_tree<tkey, tvalue> const &tree,
    std::vector<typename AVL_tree<tkey, tvalue>::iterator_data> &expected_result)
{
    std::string line;
    auto end_infix = tree.cend_infix();
    auto it = tree.cbegin_infix();
    
    for (auto const &item: expected_result)
    {
        if ((*it)->depth != item.depth || (*it)->key != item.key || (*it)->value != item.value ||
            reinterpret_cast<typename AVL_tree<tkey, tvalue>::iterator_data const *>(*it)->subtree_height != item.subtree_height)
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
    AVL_tree<tkey, tvalue> const &tree,
    std::vector<typename AVL_tree<tkey, tvalue>::iterator_data> &expected_result)
{
    std::string line;
    auto end_prefix = tree.end_prefix();
    auto it = tree.cbegin_prefix();
    
    for (auto const &item: expected_result)
    {
        if ((*it)->depth != item.depth || (*it)->key != item.key || (*it)->value != item.value ||
            reinterpret_cast<typename AVL_tree<tkey, tvalue>::iterator_data const *>(*it)->subtree_height != item.subtree_height)
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
    AVL_tree<tkey, tvalue> const &tree,
    std::vector<typename AVL_tree<tkey, tvalue>::iterator_data> &expected_result)
{
    std::string line;
    auto end_postfix = tree.end_postfix();
    auto it = tree.cbegin_postfix();
    
    for (auto const &item: expected_result)
    {
        if ((*it)->depth != item.depth || (*it)->key != item.key || (*it)->value != item.value ||
            reinterpret_cast<typename AVL_tree<tkey, tvalue>::iterator_data const *>(*it)->subtree_height != item.subtree_height)
        {
            return false;
        }
        ++it;
    }
    return true;
}

TEST(AVLTreePositiveTests, test1)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test1 started");
    
    search_tree<int, std::string> *avl = new AVL_tree<int, std::string>(nullptr, logger);
    
    avl->insert(5, "a");
    avl->insert(2, "b");
    avl->insert(15, "c");
    avl->insert(3, "d");
    avl->insert(14, "e");
    avl->insert(1, "l");
    
    std::vector<typename AVL_tree<int, std::string>::iterator_data> expected_result =
        {
            AVL_tree<int, std::string>::iterator_data(2, 1, "l", 1),
            AVL_tree<int, std::string>::iterator_data(1, 2, "b", 2),
            AVL_tree<int, std::string>::iterator_data(2, 3, "d", 1),
            AVL_tree<int, std::string>::iterator_data(0, 5, "a", 3),
            AVL_tree<int, std::string>::iterator_data(2, 14, "e", 1),
            AVL_tree<int, std::string>::iterator_data(1, 15, "c", 2)
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<AVL_tree<int, std::string> *>(avl), expected_result));
    
    logger->trace("AVLTreePositiveTests.test1 finished");
    
    delete avl;
    delete logger;
}

TEST(AVLTreePositiveTests, test2)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test2 started");
    
    search_tree<int, int> *avl = new AVL_tree<int, int>(nullptr, logger);
    
    avl->insert(1, 5);
    avl->insert(2, 12);
    avl->insert(15, 1);
    avl->insert(3, 67);
    avl->insert(4, 45);
    
    std::vector<typename AVL_tree<int, int>::iterator_data> expected_result =
        {
            AVL_tree<int, int>::iterator_data(0, 2, 12, 3),
            AVL_tree<int, int>::iterator_data(1, 1, 5, 1),
            AVL_tree<int, int>::iterator_data(1, 4, 45, 2),
            AVL_tree<int, int>::iterator_data(2, 3, 67, 1),
            AVL_tree<int, int>::iterator_data(2, 15, 1, 1)
        };
    
    EXPECT_TRUE(prefix_iterator_test(*reinterpret_cast<AVL_tree<int, int> *>(avl), expected_result));
    
    logger->trace("AVLTreePositiveTests.test2 finished");
    
    delete avl;
    delete logger;
}

TEST(AVLTreePositiveTests, test3)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test3 started");
    
    search_tree<std::string, int> *avl = new AVL_tree<std::string, int>(nullptr, logger);
    
    avl->insert("a", 1);
    avl->insert("b", 2);
    avl->insert("c", 15);
    avl->insert("d", 3);
    avl->insert("e", 4);
    
    std::vector<typename AVL_tree<std::string, int>::iterator_data> expected_result =
        {
            AVL_tree<std::string, int>::iterator_data(1, "a", 1, 1),
            AVL_tree<std::string, int>::iterator_data(2, "c", 15, 1),
            AVL_tree<std::string, int>::iterator_data(2, "e", 4, 1),
            AVL_tree<std::string, int>::iterator_data(1, "d", 3, 2),
            AVL_tree<std::string, int>::iterator_data(0, "b", 2, 3)
        };
    
    EXPECT_TRUE(postfix_iterator_test(*reinterpret_cast<AVL_tree<std::string, int> *>(avl), expected_result));
    
    logger->trace("AVLTreePositiveTests.test3 finished");
    
    delete avl;
    delete logger;
}

TEST(AVLTreePositiveTests, test4)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test4 started");
    
    search_tree<int, std::string> *avl1 = new AVL_tree<int, std::string>(nullptr, logger);
    
    avl1->insert(6, "a");
    avl1->insert(8, "c");
    avl1->insert(15, "x");
    avl1->insert(4, "j");
    avl1->insert(1, "i");
    avl1->insert(5, "b");
    
    std::vector<typename AVL_tree<int, std::string>::iterator_data> expected_result =
        {
            AVL_tree<int, std::string>::iterator_data(2, 1, "i", 1),
            AVL_tree<int, std::string>::iterator_data(1, 4, "j", 2),
            AVL_tree<int, std::string>::iterator_data(2, 5, "b", 1),
            AVL_tree<int, std::string>::iterator_data(0, 6, "a", 3),
            AVL_tree<int, std::string>::iterator_data(1, 8, "c", 2),
            AVL_tree<int, std::string>::iterator_data(2, 15, "x", 1)
        };
    
    AVL_tree<int, std::string> avl2(std::move(*reinterpret_cast<AVL_tree<int, std::string> *>(avl1)));
    
    EXPECT_TRUE(infix_iterator_test(avl2, expected_result));
    
    logger->trace("AVLTreePositiveTests.test4 finished");
    
    delete avl1;
    delete logger;
}

TEST(AVLTreePositiveTests, test5)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test5 started");
    
    search_tree<int, std::string> *avl1 = new AVL_tree<int, std::string>(nullptr, logger);
    
    avl1->insert(6, "a");
    avl1->insert(8, "c");
    avl1->insert(15, "x");
    avl1->insert(4, "j");
    avl1->insert(1, "i");
    avl1->insert(5, "b");
    
    std::vector<typename AVL_tree<int, std::string>::iterator_data> expected_result =
        {
            AVL_tree<int, std::string>::iterator_data(2, 1, "i", 1),
            AVL_tree<int, std::string>::iterator_data(1, 4, "j", 2),
            AVL_tree<int, std::string>::iterator_data(2, 5, "b", 1),
            AVL_tree<int, std::string>::iterator_data(0, 6, "a", 3),
            AVL_tree<int, std::string>::iterator_data(1, 8, "c", 2),
            AVL_tree<int, std::string>::iterator_data(2, 15, "x", 1)
        };
    
    AVL_tree<int, std::string> avl2 = std::move(*reinterpret_cast<AVL_tree<int, std::string> *>(avl1));
    
    EXPECT_TRUE(infix_iterator_test(avl2, expected_result));
    
    logger->trace("AVLTreePositiveTests.test5 finished");
    
    delete avl1;
    delete logger;
}

TEST(AVLTreePositiveTests, test6)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test6 started");
    
    search_tree<int, std::string> *avl1 = new AVL_tree<int, std::string>(nullptr, logger);
    
    avl1->insert(6, "a");
    avl1->insert(8, "c");
    avl1->insert(15, "x");
    avl1->insert(4, "j");
    avl1->insert(1, "i");
    avl1->insert(5, "b");
    
    avl1->dispose(5);
    
    std::vector<typename AVL_tree<int, std::string>::iterator_data> expected_result =
        {
            AVL_tree<int, std::string>::iterator_data(2, 1, "i", 1),
            AVL_tree<int, std::string>::iterator_data(1, 4, "j", 2),
            AVL_tree<int, std::string>::iterator_data(0, 6, "a", 3),
            AVL_tree<int, std::string>::iterator_data(1, 8, "c", 2),
            AVL_tree<int, std::string>::iterator_data(2, 15, "x", 1)
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<AVL_tree<int, std::string> *>(avl1), expected_result));
    
    logger->trace("AVLTreePositiveTests.test6 finished");
    
    delete avl1;
    delete logger;
}

TEST(AVLTreePositiveTests, test7)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test7 started");
    
    search_tree<int, std::string> *avl1 = new AVL_tree<int, std::string>(nullptr, logger);
    
    avl1->insert(6, "a");
    avl1->insert(8, "c");
    avl1->insert(15, "x");
    avl1->insert(4, "j");
    avl1->insert(3, "i");
    avl1->insert(2, "l");
    avl1->insert(5, "b");
    
    avl1->dispose(3);
    
    std::vector<typename AVL_tree<int, std::string>::iterator_data> expected_result =
        {
            AVL_tree<int, std::string>::iterator_data(2, 2, "l", 1),
            AVL_tree<int, std::string>::iterator_data(1, 4, "j", 2),
            AVL_tree<int, std::string>::iterator_data(1, 5, "b", 1),
            AVL_tree<int, std::string>::iterator_data(0, 6, "a", 3),
            AVL_tree<int, std::string>::iterator_data(1, 8, "c", 2),
            AVL_tree<int, std::string>::iterator_data(2, 15, "x", 1)
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<AVL_tree<int, std::string> *>(avl1), expected_result));
    
    logger->trace("AVLTreePositiveTests.test7 finished");
    
    delete avl1;
    delete logger;
    
}

TEST(AVLTreePositiveTests, test8)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test8 started");
    
    search_tree<int, std::string> *avl1 = new AVL_tree<int, std::string>(nullptr, logger);
    
    avl1->insert(6, "a");
    avl1->insert(8, "c");
    avl1->insert(15, "x");
    avl1->insert(11, "j");
    avl1->insert(19, "i");
    avl1->insert(12, "l");
    avl1->insert(17, "b");
    avl1->insert(18, "e");
    
    avl1->dispose(15);
    
    std::vector<typename AVL_tree<int, std::string>::iterator_data> expected_result =
        {
            AVL_tree<int, std::string>::iterator_data(2, 6, "a", 1),
            AVL_tree<int, std::string>::iterator_data(1, 8, "c", 2),
            AVL_tree<int, std::string>::iterator_data(0, 11, "j", 4),
            AVL_tree<int, std::string>::iterator_data(2, 12, "l", 2),
            AVL_tree<int, std::string>::iterator_data(3, 17, "b", 1),
            AVL_tree<int, std::string>::iterator_data(1, 18, "e", 3),
            AVL_tree<int, std::string>::iterator_data(2, 19, "i", 1)
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<AVL_tree<int, std::string> *>(avl1), expected_result));
    
    logger->trace("AVLTreePositiveTests.test8 finished");
    
    delete avl1;
    delete logger;
}

TEST(AVLTreePositiveTests, test9)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test9 started");
    
    search_tree<int, std::string> *avl1 = new AVL_tree<int, std::string>(nullptr, logger);
    
    avl1->insert(6, "a");
    avl1->insert(8, "c");
    avl1->insert(15, "x");
    avl1->insert(11, "j");
    avl1->insert(19, "i");
    avl1->insert(12, "l");
    avl1->insert(17, "b");
    avl1->insert(18, "e");
    
    avl1->dispose(11);
    
    std::vector<typename AVL_tree<int, std::string>::iterator_data> expected_result =
        {
            AVL_tree<int, std::string>::iterator_data(2, 6, "a", 1),
            AVL_tree<int, std::string>::iterator_data(1, 8, "c", 2),
            AVL_tree<int, std::string>::iterator_data(2, 12, "l", 1),
            AVL_tree<int, std::string>::iterator_data(0, 15, "x", 3),
            AVL_tree<int, std::string>::iterator_data(2, 17, "b", 1),
            AVL_tree<int, std::string>::iterator_data(1, 18, "e", 2),
            AVL_tree<int, std::string>::iterator_data(2, 19, "i", 1)
        };
    
    EXPECT_TRUE(infix_iterator_test(*reinterpret_cast<AVL_tree<int, std::string> *>(avl1), expected_result));
    
    logger->trace("AVLTreePositiveTests.test9 finished");
    
    delete avl1;
    delete logger;
    
}

TEST(AVLTreePositiveTests, test10)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            },
        });
    
    logger->trace("AVLTreePositiveTests.test10 started");
    
    search_tree<int, std::string> *avl1 = new AVL_tree<int, std::string>(nullptr, logger);
    
    avl1->insert(6, "l");
    avl1->insert(8, "c");
    avl1->insert(15, "l");
    avl1->insert(11, "o");
    avl1->insert(9, "h");
    avl1->insert(2, "e");
    avl1->insert(4, "b");
    avl1->insert(18, "e");
    
    std::vector<std::string> vector;
    
    vector.push_back(avl1->obtain(9));
    vector.push_back(avl1->obtain(2));
    vector.push_back(avl1->obtain(15));
    vector.push_back(avl1->obtain(6));
    vector.push_back(avl1->obtain(11));
    
    std::string actual_result;
    
    for (auto const &item: vector)
    {
        actual_result.append(item + " ");
    }
    
    EXPECT_EQ(actual_result, "h e l l o ");
    
    logger->trace("AVLTreePositiveTests.test10 finished");
    
    delete avl1;
    delete logger;
    
}

TEST(AVLTreePositiveTests, test11)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "AVL_tree_tests_logs.txt",
                logger::severity::trace
            }
        });
    
    logger->trace("AVLTreePositiveTests.test11 started");
    
    search_tree<int, std::string> *avl = new AVL_tree<int, std::string>(nullptr, logger);
    
    avl->insert(6, "l");
    avl->insert(8, "c");
    avl->insert(15, "l");
    avl->insert(11, "o");
    avl->insert(9, "h");
    avl->insert(2, "e");
    avl->insert(4, "b");
    avl->insert(18, "e");
    
    std::vector<associative_container<int, std::string>::key_value_pair> actual_result = avl->obtain_between(2, 10, true, false);
    
    std::vector<associative_container<int, std::string>::key_value_pair> expected_result =
        {
            { 2, "e" },
            { 4, "b" },
            { 6, "l" },
            { 8, "c" },
            { 9, "h" }
        };
    
    EXPECT_TRUE(compare_results(expected_result, actual_result));
    
    logger->trace("AVLTreePositiveTests.test11 finished");
    
    delete avl;
    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}