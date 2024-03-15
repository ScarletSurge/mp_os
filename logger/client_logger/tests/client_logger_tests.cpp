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
            ->build();


    constructed_logger->trace("aaaaa");
    constructed_logger->debug("bbbbb");




    delete constructed_logger;
    delete builder;


    return 0;

}
