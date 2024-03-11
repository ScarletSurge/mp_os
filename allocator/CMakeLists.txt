cmake_minimum_required(VERSION 3.21)
project(mp_os_allctr
        VERSION 1.0
        DESCRIPTION "allocators interface & implementations project"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(allocator)
add_subdirectory(allocator_boundary_tags)
add_subdirectory(allocator_buddies_system)
add_subdirectory(allocator_global_heap)
add_subdirectory(allocator_red_black_tree)
add_subdirectory(allocator_sorted_list)