cmake_minimum_required(VERSION 3.21)
project(mp_os_allctr_allctr)

add_library(
        mp_os_allctr_allctr
        src/allocator_guardant.cpp
        src/allocator_test_utils.cpp)
target_include_directories(
        mp_os_allctr_allctr
        PUBLIC
        ./include)
set_target_properties(
        mp_os_allctr_allctr PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "allocator interface library")