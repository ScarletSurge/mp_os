cmake_minimum_required(VERSION 3.21)
project(mp_os_allctr_allctr_bdds_sstm)

add_subdirectory(tests)
add_library(
        mp_os_allctr_allctr_bdds_sstm
        src/allocator_buddies_system.cpp)
target_include_directories(
        mp_os_allctr_allctr_bdds_sstm
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_allctr_allctr_bdds_sstm
        PUBLIC
        mp_os_cmmn)
target_link_libraries(
        mp_os_allctr_allctr_bdds_sstm
        PUBLIC
        mp_os_lggr_lggr)
target_link_libraries(
        mp_os_allctr_allctr_bdds_sstm
        PUBLIC
        mp_os_allctr_allctr)
set_target_properties(
        mp_os_allctr_allctr_bdds_sstm PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "buddies system allocator implementation library")