cmake_minimum_required(VERSION 3.21)
project(mp_os_lggr_clnt_lggr)

add_subdirectory(tests)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_library(
        mp_os_lggr_clnt_lggr
        src/client_logger.cpp
        src/client_logger_builder.cpp)
target_include_directories(
        mp_os_lggr_clnt_lggr
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_lggr_clnt_lggr
        PUBLIC
        mp_os_cmmn)
target_link_libraries(
        mp_os_lggr_clnt_lggr
        PUBLIC
        mp_os_lggr_lggr)
target_link_libraries(
        mp_os_lggr_clnt_lggr
        PUBLIC
        nlohmann_json::nlohmann_json)
set_target_properties(
        mp_os_lggr_clnt_lggr PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "client logger implementation library")