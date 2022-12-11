cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

enable_testing()

add_executable(
    pddcp_test
    ${PDDCP_SRC_DIR}/35.cc
    ${PDDCP_SRC_DIR}/176.cc
)
target_link_libraries(pddcp_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(pddcp_test)
