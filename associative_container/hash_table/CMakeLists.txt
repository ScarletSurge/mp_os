cmake_minimum_required(VERSION 3.21)
project(mp_os_assctv_cntnr_hsh_tbl)

add_subdirectory(tests)
add_library(
        mp_os_assctv_cntnr_hsh_tbl
        include/hash_table.h)
target_include_directories(
        mp_os_assctv_cntnr_hsh_tbl
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_assctv_cntnr_hsh_tbl
        PUBLIC
        mp_os_cmmn)
target_link_libraries(
        mp_os_assctv_cntnr_hsh_tbl
        PUBLIC
        mp_os_lggr_lggr)
target_link_libraries(
        mp_os_assctv_cntnr_hsh_tbl
        PUBLIC
        mp_os_allctr_allctr)
target_link_libraries(
        mp_os_assctv_cntnr_hsh_tbl
        PUBLIC
        mp_os_assctv_cntnr)
set_target_properties(
        mp_os_assctv_cntnr_hsh_tbl PROPERTIES
        VERSION 1.0
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        DESCRIPTION "hash table implementation library")