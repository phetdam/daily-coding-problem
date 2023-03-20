cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

# set compiler-specific options
if(MSVC)
    # MSVC doesn't report __cplusplus correctly unless you specify
    # /Zc:__cplusplus; it used to always be 199711L regardless of standard
    add_compile_options(
        /Wall /Zc:__cplusplus
        # typically don't care if unreferenced inline functions are removed
        /wd4514
        # silence warnings about padding (we don't care for this project)
        /wd4820
        # for Google Test ::testing::Test subclasses, these are reported a lot,
        # i.e. for implicitly deleted copy/move ctor + operator=
        /wd4625 /wd4626 /wd5026 /wd5027
        # Google Test gtest-param-util.h triggers these warnings (braced
        # initialization list eval order, Spectre mitigation)
        /wd4868 /wd5045
        # 89.cc, 135.cc Google Test warns that const variable is not used
        /wd5264
    )
    if(NOT CMAKE_BUILD_TYPE STREQUAL Release)
        add_compile_options(/Od /DEBUG)
    endif()
else()
    # don't suggest putting () around && statement in ||
    add_compile_options(-Wall -Wno-parentheses)
    if(NOT CMAKE_BUILD_TYPE STREQUAL Release)
        add_compile_options(-ggdb -O0)
    else()
        add_compile_options(-O3)
    endif()
endif()
