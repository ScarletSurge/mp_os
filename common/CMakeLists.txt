cmake_minimum_required(VERSION 3.21)
project(mp_os_cmmn)

add_library(
        mp_os_cmmn
        src/not_implemented.cpp
        src/operation_not_supported.cpp)
target_include_directories(
        mp_os_cmmn
        PUBLIC
        ./include)
set_target_properties(
        mp_os_cmmn PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "common utils and exception classes library")