cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

# set compiler-specific options
if(MSVC)
    add_compile_options(
        # MSVC doesn't report __cplusplus correctly unless you specify
        # /Zc:__cplusplus; it used to always be 199711L regardless of standard
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
        # /Wall enables excessive warnings about automatic inline expansion
        /wd4710 /wd4711
        # /Wall enables warning about 32-bit floats being stored in memory,
        # which is performance loss compared to storing in register
        /wd4738
        # /Od applied by default when using Debug configuration
        $<$<NOT:$<CONFIG:Release>>:/DEBUG>
    )
    # note: does not work correctly for Windows. it seems that programs using
    # compiled with AddressSanitizer enabled that use Google Test TYPED_TEST
    # will hang indefinitely with the CPU constantly spinning.
    # enable AddressSanitizer use
    if(ENABLE_ASAN)
        message(STATUS "AddressSanitizer (/fsanitize=address) enabled")
        # must remove run-time check options from C++ flags. CMake adds these
        # by default in CMAKE_C(XX)_FLAGS_DEBUG, so remove them
        string(
            REGEX REPLACE
            "/RTC[1csu]" "" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG}
        )
        message(STATUS "Removed /RTC[1csu] from CMAKE_C_FLAGS_DEBUG")
        string(
            REGEX REPLACE
            "/RTC[1csu]" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}
        )
        message(STATUS "Removed /RTC[1csu] from CMAKE_CXX_FLAGS_DEBUG")
        add_compile_options(/fsanitize=address)
        # need to disable MSVC std::vector annotation for static Google Test
        add_compile_definitions(_DISABLE_VECTOR_ANNOTATION)
        # AddressSanitizer cannot be used with incremental linking
        add_link_options(/INCREMENTAL:NO)
    endif()
else()
    add_compile_options(
        -Wall
        # don't suggest putting () around && statement in ||
        -Wno-parentheses
        # -O0 is default optimization level anyways, Clang ignores -ggdb
        $<$<NOT:$<CONFIG:Release>>:-ggdb> $<IF:$<CONFIG:Release>,-O3,-O0>
    )
    # enable AddressSanitizer use
    if(ENABLE_ASAN)
        message(STATUS "AddressSanitizer (-fsanitize=address) enabled")
        # must specifiy for both compile and link
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()
endif()
