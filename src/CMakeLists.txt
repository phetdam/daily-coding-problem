cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

# define globally for all standalone targets
add_compile_definitions(PDDCP_GTEST_STANDALONE)

pdccp_add_gtest_standalone(7)
pdccp_add_gtest_standalone(11)
pdccp_add_gtest_standalone(15)
pdccp_add_gtest_standalone(35)
pdccp_add_gtest_standalone(51)
pdccp_add_gtest_standalone(89)
pdccp_add_gtest_standalone(124)
# kill flood of warnings that pop up on Windows from ppltasks.h
if(MSVC)
    target_compile_options(124 PRIVATE /wd4355 /wd4514 /wd4820 /wd5204 /wd5220)
endif()
pdccp_add_gtest_standalone(128)
pdccp_add_gtest_standalone(141)
pdccp_add_gtest_standalone(176)
