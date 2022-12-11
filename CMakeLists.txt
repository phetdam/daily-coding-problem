cmake_minimum_required(VERSION 3.21)

project(
    pddcp
    VERSION 0.1.0
    DESCRIPTION "Solutions written for Daily Coding Problem exercises"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest 1.10 REQUIRED)
include(Googletest)
enable_testing()

add_subdirectory(src)
