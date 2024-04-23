cmake_minimum_required(VERSION 3.21)
project(mp_os_assctv_cntnr)

add_subdirectory(hash_table)
add_subdirectory(search_tree)
add_library(
        mp_os_assctv_cntnr
        include/associative_container.h)
target_include_directories(
        mp_os_assctv_cntnr
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_assctv_cntnr
        PUBLIC
        mp_os_cmmn)
set_target_properties(
        mp_os_assctv_cntnr PROPERTIES
        VERSION 1.0
        DESCRIPTION "associative container interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)