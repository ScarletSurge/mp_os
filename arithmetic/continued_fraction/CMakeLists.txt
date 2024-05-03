cmake_minimum_required(VERSION 3.21)
project(mp_os_arthmtc_cntnd_frctn)

add_subdirectory(tests)

add_library(
        mp_os_arthmtc_cntnd_frctn
        src/continued_fraction.cpp)
target_include_directories(
        mp_os_arthmtc_cntnd_frctn
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_arthmtc_cntnd_frctn
        PUBLIC
        mp_os_cmmn)
target_link_libraries(
        mp_os_arthmtc_cntnd_frctn
        PUBLIC
        mp_os_allctr_allctr)
target_link_libraries(
        mp_os_arthmtc_cntnd_frctn
        PUBLIC
        mp_os_arthmtc_bg_intgr)
target_link_libraries(
        mp_os_arthmtc_cntnd_frctn
        PUBLIC
        mp_os_arthmtc_frctn)
set_target_properties(
        mp_os_arthmtc_cntnd_frctn PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "complex number implementation library")