cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

add_executable(35 35.cc)
target_link_libraries(35 GTest::gtest_main)

add_executable(176 176.cc)
target_link_libraries(176 GTest::gtest_main)
