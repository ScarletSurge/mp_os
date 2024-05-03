#include <gtest/gtest.h>

#include <sstream>

#include <big_integer.h>
#include <client_logger.h>
#include <operation_not_supported.h>

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

TEST(positive_tests, test1)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
                                       {
                                           {
                                               "bigint_logs.txt",
                                               logger::severity::information
                                           },
                                       });

    big_integer bigint_1("2");
    big_integer bigint_2("3");
    big_integer::divide(bigint_1, bigint_2, nullptr, big_integer::division_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "0");

    delete logger;
}

TEST(positive_tests, test2)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
                                       {
                                           {
                                               "bigint_logs.txt",
                                               logger::severity::information
                                           },
                                       });

    big_integer bigint_1("20944325634363");
    big_integer bigint_2("0");

    EXPECT_THROW(big_integer::divide(bigint_1, bigint_2, nullptr, big_integer::division_rule::trivial), std::logic_error);

    delete logger;
}

TEST(positive_tests, test3)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
                                       {
                                           {
                                               "bigint_logs.txt",
                                               logger::severity::information
                                           },
                                       });

    big_integer bigint_1("001123");
    big_integer bigint_2("-000000");

    EXPECT_THROW(big_integer::divide(bigint_1, bigint_2, nullptr, big_integer::division_rule::trivial), std::logic_error);

    delete logger;
}

TEST(positive_tests, test4)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
                                       {
                                           {
                                               "bigint_logs.txt",
                                               logger::severity::information
                                           },
                                       });

    big_integer bigint_1("-28958888309635818");
    big_integer bigint_2("-234567");
    big_integer::divide(bigint_1, bigint_2, nullptr, big_integer::division_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "123456787654");

    delete logger;
}

TEST(positive_tests, test5)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
                                       {
                                           {
                                               "bigint_logs.txt",
                                               logger::severity::information
                                           },
                                       });

    std::stringstream iss("806211213 52");

    big_integer bigint_1("0");
    big_integer bigint_2("0");
    iss >> bigint_1 >> bigint_2;
    big_integer result_of_division = big_integer::divide(bigint_1, bigint_2, nullptr, big_integer::division_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "15504061");

    delete logger;
}

TEST(positive_tests, test6)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
                                       {
                                           {
                                               "bigint_logs.txt",
                                               logger::severity::information
                                           },
                                       });

    big_integer bigint_1("123424353464389587244387927589346894576464343235445645674563532464675467425");
    big_integer bigint_2("2354893245937465784937542389428935349086840957804985309763636567574564");
    big_integer::divide(bigint_1, bigint_2, nullptr, big_integer::division_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "52411");

    delete logger;
}

TEST(positive_tests, test7)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
                                       {
                                           {
                                               "bigint_logs.txt",
                                               logger::severity::information
                                           },
                                       });

    big_integer bigint_1("12342435346438958724438792758934689457646434323544564567456353246467546742553890454890356745895343687456894678934854493068450697557345353");
    big_integer bigint_2("42389428935349086840957804985309763636567574564");
    big_integer::divide(bigint_1, bigint_2, nullptr, big_integer::division_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "291167766502899124008723943300693817184031156313950883875886123638773043440828935681031181");

    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}