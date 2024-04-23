cmake_minimum_required(VERSION 3.21)
project(mp_os_assctv_cntnr_srch_tr_indxng_tr_b_str_tr)

add_subdirectory(tests)
add_library(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_str_tr
        include/b_star_tree.h)
target_include_directories(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_str_tr
        PUBLIC
        ./include)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_str_tr
        PUBLIC
        mp_os_cmmn)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_str_tr
        PUBLIC
        mp_os_lggr_clnt_lggr)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_str_tr
        PUBLIC
        mp_os_assctv_cntnr)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_str_tr
        PUBLIC
        mp_os_assctv_cntnr_srch_tr)
set_target_properties(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_str_tr PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "B* tree implementation library")