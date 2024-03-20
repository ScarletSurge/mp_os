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
            ->transform_with_configuration("file.json", "logger")
            ->add_file_stream("file1.txt", logger::severity::trace)
            ->add_format_string("%m %")
            ->add_console_stream(logger::severity::critical)
            ->build();


    constructed_logger->trace("this is a simple");
    constructed_logger->debug("test");
    constructed_logger->critical("that logger works");




    delete constructed_logger;
    delete builder;


    return 0;

}
