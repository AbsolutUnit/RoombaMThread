cmake_minimum_required(VERSION 3.22)
project(A3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_subdirectory(Algorithm_1_112628028)
add_subdirectory(Algorithm_2_112710107)
add_subdirectory(Simulation)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g)

set(common_libs Algorithm_1_112628028 Algorithm_2_112710107 Simulation)