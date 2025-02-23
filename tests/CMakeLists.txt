file(GLOB_RECURSE test_src CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/tests/*.[ch]pp
)

add_executable(cog2d_tests ${test_src} ${PROJECT_SOURCE_DIR}/tests/main.cpp)

target_link_libraries(cog2d_tests PUBLIC cog2d)
target_include_directories(cog2d_tests PUBLIC ${PROJECT_SOURCE_DIR}/tests/)

if(WIN32)
        add_custom_command(TARGET cog2d_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_RUNTIME_DLLS:cog2d_tests>
                $<TARGET_FILE_DIR:cog2d_tests>
                COMMAND_EXPAND_LISTS
        )
endif()
