//#include <not_implemented.h>

#include "../include/client_logger_builder.h"
#include <nlohmann/json.hpp>

client_logger_builder::client_logger_builder()
{
    this->format_string = "%d %t %s %m";
}

client_logger_builder::~client_logger_builder() noexcept
= default;

logger_builder* client_logger_builder::add_format_string(const std::string &format_string)
{
    this->format_string = format_string;

    return this;
}

logger_builder *client_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity)
{
    if(_builder_streams.find(stream_file_path) != _builder_streams.end())
    {
        _builder_streams[stream_file_path].insert(severity);
    }

    else
    {
        _builder_streams.insert({stream_file_path, {severity}});
    }

    return this;

}



logger_builder *client_logger_builder::add_console_stream(logger::severity severity)
{

    if(_builder_streams.find("console") != _builder_streams.end())
    {
        _builder_streams["console"].insert(severity);
    }

    else
    {
        _builder_streams.insert({"console", {severity}});
    }

    return this;
}

logger_builder *client_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path)
{
    std::ifstream file(configuration_file_path);
    auto information = nlohmann::json::parse(file); //парсим в json формат
    auto pairs = information[configuration_path];

    logger::severity severity;

    for (auto &elem : pairs)
    {//проверяем есть ли в мапе элемент с ключём равным значению файла из текущего элемента пары, если есть до добавляем сиверити
        if (_builder_streams.find(elem.value("file", "not found!")) != _builder_streams.end())
        {
            severity = elem["severity"];
            _builder_streams[elem.value("file", "not found!")].insert(severity);
        }
        else
        {
            _builder_streams.insert({elem.value("file", "not found!"), {elem["severity"]}});

        }

    }

    return this;
}

logger_builder *client_logger_builder::clear()
{
    _builder_streams.clear();

    return this;
}

logger *client_logger_builder::build() const
{
   return new client_logger(_builder_streams, this->format_string);
}