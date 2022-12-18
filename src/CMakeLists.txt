cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

pdccp_add_gtest_standalone(7)
pdccp_add_gtest_standalone(15)
pdccp_add_gtest_standalone(35)
pdccp_add_gtest_standalone(51)
pdccp_add_gtest_standalone(128)
# may define PDDCP_GTEST_STANDALONE for all standalone targets later
target_compile_definitions(128 PRIVATE PDDCP_GTEST_STANDALONE)
pdccp_add_gtest_standalone(176)
