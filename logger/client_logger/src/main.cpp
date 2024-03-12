#include <iostream>
#include <../include/client_logger.h>



int main()
{
    logger_builder* builder = new client_logger_builder();

    logger* constructed_logger = builder
            ->add_file_stream("file1.txt", logger::severity::information)
            ->add_file_stream("file1.txt", logger::severity::debug)
            ->add_file_stream("file2.txt", logger::severity::warning)
            ->build();

    constructed_logger->warning("del by zero");


    delete constructed_logger;
    delete builder;

    return 0;

}
