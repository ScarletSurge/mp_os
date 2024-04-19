#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_H

#include <iostream>

class logger
{

public:

    enum class severity
    {
        trace,
        debug,
        information,
        warning,
        error,
        critical
    };

public:

    virtual ~logger() noexcept = default;

public:

    virtual logger const *log(
        std::string const &message,
        logger::severity severity) const noexcept = 0;

public:

    logger const *trace(
        std::string const &message) const noexcept;

    logger const *debug(
        std::string const &message) const noexcept;

    logger const *information(
        std::string const &message) const noexcept;

    logger const *warning(
        std::string const &message) const noexcept;

    logger const *error(
        std::string const &message) const noexcept;

    logger const *critical(
        std::string const &message) const noexcept;

protected:

    static std::string severity_to_string(
        logger::severity severity);

    static std::string current_date_to_string() noexcept;

    static std::string current_time_to_string() noexcept;

    static std::string current_datetime_to_string() noexcept;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_H