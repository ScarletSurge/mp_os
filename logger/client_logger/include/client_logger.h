#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include <client_logger_builder.h>
//#include <../../../logger/client_logger/include/client_logger_builder.h>
#include <map>
#include <set>

class client_logger final : public logger
{
    friend class client_logger_builder;

private:
    std::map<std::string, std::pair<std::ofstream*, std::set<logger::severity>>> _streams;


private:

    static std::map<std::string, std::pair<std::ofstream*, size_t >> _global_streams;

private:

    std::string _format_string;

public:

    explicit client_logger(std::map<std::string, std::set<logger::severity>> const &, std::string const& format_string);

    client_logger(client_logger const &other) = delete;

    client_logger &operator=(client_logger const &other) = delete;

    client_logger(client_logger &&other) = delete;

    client_logger &operator=(client_logger &&other) = delete;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(const std::string &message, logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H