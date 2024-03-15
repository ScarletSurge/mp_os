#include <gtest/gtest.h>
#include <client_logger_builder.h>
#include <logger.h>



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
            ->transform_with_configuration("file.json", "logger")
            ->add_file_stream("file1.txt", logger::severity::trace)
            ->add_format_string("%d%d%d%d%m%t%s")
            ->add_console_stream(logger::severity::critical)
            ->build();


    constructed_logger->trace("trace");
    constructed_logger->debug("debug");
    constructed_logger->critical("critical");




    delete constructed_logger;
    delete builder;


    return 0;

}
