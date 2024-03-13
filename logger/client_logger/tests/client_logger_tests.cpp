#include <gtest/gtest.h>
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
            ->add_file_stream("file1.txt", logger::severity::information)
            ->add_file_stream("file1.txt", logger::severity::debug)
            ->add_file_stream("file2.txt", logger::severity::warning)
            ->build();

    constructed_logger->warning("i dont know");


    delete constructed_logger;
    delete builder;

    return 0;

}
