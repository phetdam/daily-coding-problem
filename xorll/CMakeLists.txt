cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

# XOR linked list mini-library. on Windows, we also need to copy it to where
# the legacy XOR linked list driver program will be copied
add_library(pddcp_xorll SHARED xorll.c)
if(WIN32)
    add_custom_command(
        TARGET pddcp_xorll POST_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:pddcp_xorll>
                ${CMAKE_SOURCE_DIR}/xorll
    )
endif()
# XOR linked list legacy driver program. needs to be copied to source location
add_executable(xorll_driver main.c)
target_link_libraries(xorll_driver PRIVATE pddcp_xorll)
add_custom_command(
    TARGET xorll_driver POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:xorll_driver>
            ${CMAKE_SOURCE_DIR}/xorll
)
