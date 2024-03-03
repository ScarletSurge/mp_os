#include <not_implemented.h>

#include "../include/server_logger_builder.h"

server_logger_builder::server_logger_builder()
{
    throw not_implemented("server_logger_builder::server_logger_builder()", "your code should be here...");
}

server_logger_builder::server_logger_builder(
    server_logger_builder const &other)
{
    throw not_implemented("server_logger_builder::server_logger_builder(server_logger_builder const &other)", "your code should be here...");
}

server_logger_builder &server_logger_builder::operator=(
    server_logger_builder const &other)
{
    throw not_implemented("server_logger_builder &server_logger_builder::operator=(server_logger_builder const &other)", "your code should be here...");
}

server_logger_builder::server_logger_builder(
    server_logger_builder &&other) noexcept
{
    throw not_implemented("server_logger_builder::server_logger_builder(server_logger_builder &&other) noexcept", "your code should be here...");
}

server_logger_builder &server_logger_builder::operator=(
    server_logger_builder &&other) noexcept
{
    throw not_implemented("server_logger_builder &server_logger_builder::operator=(server_logger_builder &&other) noexcept", "your code should be here...");
}

server_logger_builder::~server_logger_builder() noexcept
{
    throw not_implemented("server_logger_builder::~server_logger_builder() noexcept", "your code should be here...");
}

logger_builder *server_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    throw not_implemented("logger_builder *server_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity)", "your code should be here...");
}

logger_builder *server_logger_builder::add_console_stream(
    logger::severity severity)
{
    throw not_implemented("logger_builder *server_logger_builder::add_console_stream(logger::severity severity)", "your code should be here...");
}

logger_builder* server_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    throw not_implemented("logger_builder* server_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path)", "your code should be here...");
}

logger_builder *server_logger_builder::clear()
{
    throw not_implemented("logger_builder *server_logger_builder::clear()", "your code should be here...");
}

logger *server_logger_builder::build() const
{
    throw not_implemented("logger *server_logger_builder::build() const", "your code should be here...");
}