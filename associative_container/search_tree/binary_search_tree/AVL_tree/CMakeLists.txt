cmake_minimum_required(VERSION 3.21)
project(mp_os_assctv_cntnr_srch_tr_bnr_srch_tr_AVL_tr)

add_subdirectory(tests)
add_library(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr_AVL_tr
        include/AVL_tree.h)
target_include_directories(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr_AVL_tr
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr_AVL_tr
        PUBLIC
        mp_os_assctv_cntnr)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr_AVL_tr
        PUBLIC
        mp_os_assctv_cntnr_srch_tr)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr_AVL_tr
        PUBLIC
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr)
set_target_properties(
        mp_os_assctv_cntnr_srch_tr_bnr_srch_tr_AVL_tr PROPERTIES
        VERSION 1.0
        DESCRIPTION "AVL tree implementation library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)