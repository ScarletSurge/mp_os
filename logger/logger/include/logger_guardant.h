#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_GUARDANT_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_GUARDANT_H

#include "logger.h"

class logger_guardant
{

public:

    virtual ~logger_guardant() noexcept = default;

public:

    logger_guardant const *log_with_guard(std::string const &message,logger::severity severity) const;

    logger_guardant const *trace_with_guard(
        std::string const &message) const;

    logger_guardant const *debug_with_guard(
        std::string const &message) const;

    logger_guardant const *information_with_guard(
        std::string const &message) const;

    logger_guardant const *warning_with_guard(
        std::string const &message) const;

    logger_guardant const *error_with_guard(
        std::string const &message) const;

    logger_guardant const *critical_with_guard(
        std::string const &message) const;

public:

    inline virtual logger *get_logger() const = 0;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_LOGGER_GUARDANT_H