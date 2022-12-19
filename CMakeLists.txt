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

project(
    pddcp
    VERSION 0.1.0
    DESCRIPTION "Solutions written for Daily Coding Problem exercises"
    LANGUAGES CXX
)

# default build type Debug
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compiler-specific options
if(MSVC)
    # MSVC doesn't report __cplusplus correctly unless you specify
    # /Zc:__cplusplus; it used to always be 199711L regardless of standard
    add_compile_options(
        /Wall /Zc:__cplusplus
        # for Google Test ::testing::Test subclasses, these are reported a lot,
        # i.e. for implicitly deleted copy/move ctor + operator=
        /wd4625 /wd4626 /wd5026 /wd5027
        # Google Test gtest-param-util.h triggers these warnings (braced
        # initialization list eval order, Spectre mitigation)
        /wd4868 /wd5045
    )
    if(NOT ${CMAKE_BUILD_TYPE} STREQUAL Release)
        add_compile_options(/Od /DEBUG)
    endif()
else()
    # don't suggest putting () around && statement in ||
    add_compile_options(-Wall -Wno-parentheses)
    if(NOT ${CMAKE_BUILD_TYPE} EQUAL Release)
        add_compile_options(-O3)
    else()
        add_compile_options(-ggdb -O0)
    endif()
endif()

find_package(GTest 1.10 REQUIRED)

# update CMake module path to allow inclusion of custom CMake code
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# add pdccp_add_gtest_standalone function
include(AddGTestStandalone)

# path to include directory (include is a reserved CMake keyword)
set(PDDCP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PDDCP_INCLUDE_DIR})
# path to src directory (we use this later when adding files to test runner)
set(PDDCP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(${PDDCP_SRC_DIR})

enable_testing()
add_subdirectory(test)
