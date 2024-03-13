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
    logger_builder* builder_2 = new client_logger_builder;

    logger* constructed_logger = builder
            ->add_file_stream("file2.txt", logger::severity::information)
            ->add_file_stream("file2.txt", logger::severity::debug)
            ->add_file_stream("file2.txt", logger::severity::warning)
            ->build();

    logger* constructed_logger_2 = builder_2
            ->add_file_stream("file2.txt", logger::severity::trace)
            ->build();

    constructed_logger->warning("warning from logger_1");
    constructed_logger->debug("debug from logger_1");

    constructed_logger_2->trace("from second logger");


    delete constructed_logger;
    delete builder;

    delete constructed_logger_2;
    delete builder_2;

    return 0;

}
