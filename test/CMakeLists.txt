cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

add_executable(
    pddcp_test
    # tests included from standalone runners. if a runner's source file
    # compiles the same under -DPDDCP_GTEST_STANDALONE, we can avoid
    # recompiling its source by directly linking the object into pddcp_test
    $<TARGET_OBJECTS:7>
    $<TARGET_OBJECTS:11>
    $<TARGET_OBJECTS:15>
    $<TARGET_OBJECTS:23>
    $<TARGET_OBJECTS:27>
    $<TARGET_OBJECTS:31>
    $<TARGET_OBJECTS:32>
    $<TARGET_OBJECTS:35>
    $<TARGET_OBJECTS:47>
    $<TARGET_OBJECTS:49>
    $<TARGET_OBJECTS:51>
    $<TARGET_OBJECTS:60>
    $<TARGET_OBJECTS:61>
    $<TARGET_OBJECTS:62>
    $<TARGET_OBJECTS:68>
    $<TARGET_OBJECTS:76>
    $<TARGET_OBJECTS:80>
    $<TARGET_OBJECTS:88>
    $<TARGET_OBJECTS:89>
    $<TARGET_OBJECTS:102>
    $<TARGET_OBJECTS:107>
    $<TARGET_OBJECTS:109>
    ${PDDCP_SOURCE_DIR}/124.cc
    ${PDDCP_SOURCE_DIR}/128.cc
    $<TARGET_OBJECTS:129>
    $<TARGET_OBJECTS:135>
    $<TARGET_OBJECTS:141>
    $<TARGET_OBJECTS:151>
    $<TARGET_OBJECTS:154>
    $<TARGET_OBJECTS:161>
    $<TARGET_OBJECTS:164>
    $<TARGET_OBJECTS:167>
    $<TARGET_OBJECTS:176>
    $<TARGET_OBJECTS:188>
    $<TARGET_OBJECTS:194>
    $<TARGET_OBJECTS:206>
    # tests from shared library unit tests
    binary_tree_test.cc
    enums_test.cc
    matrix_test.cc
    prefix_tree_test.cc
    rtti_test.cc
    type_traits_test.cc
)
# use OpenMP if available
if(OpenMP_CXX_FOUND)
    target_link_libraries(pddcp_test PRIVATE OpenMP::OpenMP_CXX)
endif()
target_link_libraries(pddcp_test PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(pddcp_test)
