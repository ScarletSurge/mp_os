#include <gtest/gtest.h>
#include <logger.h>
#include <client_logger_builder.h>



//int main(
//        int argc,
//        char *argv[])
//{
//    testing::InitGoogleTest(&argc, argv);
//
//    return RUN_ALL_TESTS();
//}
int main()
{
    logger_builder* builder = new client_logger_builder();

    logger* constructed_logger = builder
            ->transform_with_configuration("file.json", "logger") //trace debug
            ->add_format_string("%m %d")
            ->add_console_stream(logger::severity::critical)
            ->build();


    constructed_logger->trace("[logger_1] this message must be in file2.txt");
    constructed_logger->debug("[logger_1] this message must be in file1.txt");
    constructed_logger->critical("[logger_1] it must be at console");

    logger_builder* builder_2 = new client_logger_builder();

    logger* constructed_logger_2 = builder_2
            ->add_file_stream("file4.txt", logger::severity::information)
            ->add_file_stream("file1.txt", logger::severity::critical)
            ->add_file_stream("file2.txt", logger::severity::warning)
            ->build();

    constructed_logger_2->information("[logger_2] this message must be in file4.txt");
    constructed_logger_2->critical("[logger_2] this message must be in file1.txt");
    constructed_logger_2->warning("[logger_2] this message must be in file2.txt");

    logger_builder* builder_3 = new client_logger_builder();

    logger* constructed_logger_3 = builder_3
            ->add_format_string("%d %t")
            ->add_file_stream("file4.txt", logger::severity::critical)
            ->build();

    constructed_logger_3->critical("[logger_3] this message must be at file4.txt with cur data and time");


    delete constructed_logger;
    delete builder;

    delete constructed_logger_2;
    delete builder_2;

    delete constructed_logger_3;
    delete builder_3;

    return 0;

}
