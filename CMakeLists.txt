cmake_minimum_required(VERSION 3.21)

##
# How to build:
#
# *nix:
#   cmake -S . -B build && cmake --build build -j
#
# Windows:
#   cmake -S . -B build_windows -G Ninja && cmake --build build_windows -j
#
# Note on Windows you should use the correct Developer Prompt, i.e. the default
# Developer Prompt for x86 output, x64 Native Tools prompt for x64 output
#

# if C++ compiler is not defined, we automatically choose Clang on Linux. GCC
# 11.3 is required at the minimum and not all *nix system have this yet.
if(NOT WIN32)
    if(NOT DEFINED CMAKE_C_COMPILER)
        set(MESSAGE "CMAKE_C_COMPILER not defined, using clang as default")
        set(CMAKE_C_COMPILER clang)
    endif()
    if(NOT DEFINED CMAKE_CXX_COMPILER)
    set(MESSAGE "CMAKE_CXX_COMPILER not defined, using clang++ as default")
        set(CMAKE_CXX_COMPILER clang++)
    endif()
endif()

project(
    pddcp
    VERSION 0.1.0
    DESCRIPTION "Solutions written for Daily Coding Problem exercises"
    HOMEPAGE_URL https://github.com/phetdam/daily-coding-problem
    LANGUAGES C CXX
)

# check if we are using single or multi-config generator
get_property(PDDCP_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

# default build type Debug for single-config generators
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# eliminate per-configuration subdirs from being appended to build artifact
# output dirs for multi-config generators, e.g. Visual Studio
set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}$<$<CONFIG:Debug>:>
)
set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}$<$<CONFIG:Debug>:>
)
set(
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}$<$<CONFIG:Debug>:>
)

find_package(GTest 1.10 REQUIRED)

# update CMake module path to allow inclusion of custom CMake code
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# set custom compile options
include(ConfigCompile)
# try to use OpenMP if possible
include(ConfigOpenMP)
# add pdccp_add_gtest_standalone function
include(AddGTestStandalone)

# path to include directory (include is a reserved CMake keyword)
set(PDDCP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PDDCP_INCLUDE_DIR})
# path to src directory (we use this later when adding files to test runner)
set(PDDCP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# interface target for precompiled headers usable by each target. no PCH is
# built for a target unless they explicitly link against this.
add_library(pddcp_pch INTERFACE)
target_precompile_headers(
    pddcp_pch
    INTERFACE <algorithm> <string> <type_traits> <vector>
)

add_subdirectory(${PDDCP_SOURCE_DIR})
# XOR linked list implementation used for #6, also used by legacy Python driver
add_subdirectory(xorll)

enable_testing()
add_subdirectory(test)
