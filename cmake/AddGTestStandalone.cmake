cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

##
# Add a standalone Google Test runner from a single C++ source file.
#
# Arguments:
#   target_name - Name of the output executable and C++ source file stem
#
function(pddcp_add_gtest_standalone target_name)
    add_executable(${target_name} ${target_name}.cc)
    target_link_libraries(${target_name} PRIVATE GTest::gtest_main)
endfunction()

##
# Add a Google Test runner with custom main from a single C++ source file.
#
# Arguments:
#   target_name - Name of the output executable and C++ source file stem
#
function(pddcp_add_gtest_custom_main target_name)
    add_executable(${target_name} ${target_name}.cc)
    target_link_libraries(${target_name} PRIVATE GTest::gtest)
endfunction()
