cmake_minimum_required(VERSION 3.21)
project(mp_os_assctv_cntnr_srch_tr)

add_subdirectory(binary_search_tree)
#add_subdirectory(indexing_tree)
add_library(
        mp_os_assctv_cntnr_srch_tr
        include/search_tree.h)
target_include_directories(
        mp_os_assctv_cntnr_srch_tr
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr
        PUBLIC
        mp_os_cmmn)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr
        PUBLIC
        mp_os_lggr_lggr)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr
        PUBLIC
        mp_os_allctr_allctr)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr
        PUBLIC
        mp_os_assctv_cntnr)
set_target_properties(
        mp_os_assctv_cntnr_srch_tr PROPERTIES
        VERSION 1.0
        DESCRIPTION "search tree interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)