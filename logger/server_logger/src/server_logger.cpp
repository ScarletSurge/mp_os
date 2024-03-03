#include <not_implemented.h>

#include "../include/server_logger.h"

server_logger::server_logger(
    server_logger const &other)
{
    throw not_implemented("server_logger::server_logger(server_logger const &other)", "your code should be here...");
}

server_logger &server_logger::operator=(
    server_logger const &other)
{
    throw not_implemented("server_logger &server_logger::operator=(server_logger const &other)", "your code should be here...");
}

server_logger::server_logger(
    server_logger &&other) noexcept
{
    throw not_implemented("server_logger::server_logger(server_logger &&other) noexcept", "your code should be here...");
}

server_logger &server_logger::operator=(
    server_logger &&other) noexcept
{
    throw not_implemented("server_logger &server_logger::operator=(server_logger &&other) noexcept", "your code should be here...");
}

server_logger::~server_logger() noexcept
{
    throw not_implemented("server_logger::~server_logger() noexcept", "your code should be here...");
}

logger const *server_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    throw not_implemented("logger const *server_logger::log(const std::string &text, logger::severity severity) const noexcept", "your code should be here...");
}