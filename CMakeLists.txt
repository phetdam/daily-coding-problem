cmake_minimum_required(VERSION 3.21)

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
    add_compile_options(/Wall /Zc:__cplusplus)
    if(NOT ${CMAKE_BUILD_TYPE} STREQUAL Release)
        add_compile_options(/O0 /DEBUG)
    endif()
else()
    add_compile_options(-Wall)
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

# path to src directory
set(PDDCP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(${PDDCP_SRC_DIR})

enable_testing()
add_subdirectory(test)
