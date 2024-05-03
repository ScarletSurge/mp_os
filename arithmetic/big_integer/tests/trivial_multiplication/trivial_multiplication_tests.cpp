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

    big_integer bigint_1("2423545763");
    big_integer bigint_2("3657687978");
    big_integer::multiply(bigint_1, bigint_2, nullptr, big_integer::multiplication_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "8864574201457937214");

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
    big_integer::multiply(bigint_1, bigint_2, nullptr, big_integer::multiplication_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "0");

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
    big_integer bigint_2("-0000001");
    big_integer::multiply(bigint_1, bigint_2, nullptr, big_integer::multiplication_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "-1123");

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
    big_integer::multiply(bigint_1, bigint_2, nullptr, big_integer::multiplication_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "6792799554126344920806");

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

    std::stringstream iss("8062112134235893450865580976575 5224253464575690753458936456445353");

    big_integer bigint_1("0");
    big_integer bigint_2("0");
    iss >> bigint_1 >> bigint_2;
    big_integer::multiply(bigint_1, bigint_2, nullptr, big_integer::multiplication_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "42118517249079582762848120969952324453639154832768688602860605975");

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
    big_integer::multiply(bigint_1, bigint_2, nullptr, big_integer::multiplication_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "290651176357489495451049958587923972328418314663424320128873904703658883667429195585130334492391519870913575716570325570910803505581125240577700");

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

    big_integer bigint_1("999999999999999999999999999977777");
    big_integer bigint_2("-0000000000000000000000000000000000000000000000000059");
    big_integer::multiply(bigint_1, bigint_2, nullptr, big_integer::multiplication_rule::trivial);

    EXPECT_TRUE((std::ostringstream() << bigint_1).str() == "-58999999999999999999999999998688843");

    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}