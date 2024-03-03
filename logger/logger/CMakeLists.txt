cmake_minimum_required(VERSION 3.21)
project(mp_os_lggr_lggr)

add_library(
        mp_os_lggr_lggr
        src/logger.cpp
        src/logger_builder.cpp
        src/logger_guardant.cpp)
target_include_directories(
        mp_os_lggr_lggr
        PUBLIC
        ./include)
set_target_properties(
        mp_os_lggr_lggr PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "logger interface library")