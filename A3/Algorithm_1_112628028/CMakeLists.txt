cmake_minimum_required(VERSION 3.22)

project(Algorithm_1_112628028)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g)

SET(CMAKE_SHARED_LINKER_FLAGS "-fPIC -shared -fvisibility=hidden -fvisibility-inlines-hidden")
add_library(Algorithm_1_112628028 SHARED Algo_112628028.cpp)
target_include_directories(Algorithm_1_112628028 PUBLIC include)