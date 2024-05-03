cmake_minimum_required(VERSION 3.21)
project(mp_os_arthmtc_bg_intgr)

add_subdirectory(tests)

add_library(
        mp_os_arthmtc_bg_intgr
        src/big_integer.cpp)
target_include_directories(
        mp_os_arthmtc_bg_intgr
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_arthmtc_bg_intgr
        PUBLIC
        mp_os_cmmn)
target_link_libraries(
        mp_os_arthmtc_bg_intgr
        PUBLIC
        mp_os_allctr_allctr)
set_target_properties(
        mp_os_arthmtc_bg_intgr PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "big integer implementation library")