cmake_minimum_required(VERSION 3.21)
project(mp_os_assctv_cntnr_srch_tr_bnr_srch_tr)

add_subdirectory(AVL_tree)
add_subdirectory(red_black_tree)
add_subdirectory(scapegoat_tree)
add_subdirectory(splay_tree)
add_subdirectory(tests)
add_library(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr
        include/binary_search_tree.h)
target_include_directories(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr
        PUBLIC
        mp_os_assctv_cntnr)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr
        PUBLIC
        mp_os_assctv_cntnr_srch_tr)
set_target_properties(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr PROPERTIES
        VERSION 1.0
        DESCRIPTION "binary search tree implementation library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)