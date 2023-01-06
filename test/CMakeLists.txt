cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

add_executable(
    pddcp_test
    # tests included from standalone runners
    ${PDDCP_SRC_DIR}/7.cc
    ${PDDCP_SRC_DIR}/11.cc
    ${PDDCP_SRC_DIR}/15.cc
    ${PDDCP_SRC_DIR}/35.cc
    ${PDDCP_SRC_DIR}/51.cc
    ${PDDCP_SRC_DIR}/89.cc
    ${PDDCP_SRC_DIR}/124.cc
    ${PDDCP_SRC_DIR}/128.cc
    ${PDDCP_SRC_DIR}/135.cc
    ${PDDCP_SRC_DIR}/141.cc
    ${PDDCP_SRC_DIR}/176.cc
    # tests from shared library unit tests
    binary_tree_test.cc
    matrix_test.cc
    prefix_tree_test.cc
)
target_link_libraries(pddcp_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(pddcp_test)
