cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

add_executable(176 176.cc)
target_link_libraries(176 GTest::gtest_main)

gtest_discover_tests(176)
