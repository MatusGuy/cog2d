macro(cog2d_add_test name)
	file(GLOB test_src CONFIGURE_DEPENDS
		${PROJECT_SOURCE_DIR}/*.cpp
		${PROJECT_SOURCE_DIR}/*.h*)

	add_executable(${name} ${test_src})

	target_link_libraries(${name} PUBLIC cog2d)

	if(WIN32)
		add_custom_command(TARGET ${name} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
			$<TARGET_RUNTIME_DLLS:${name}>
			$<TARGET_FILE_DIR:${name}>
			COMMAND_EXPAND_LISTS
		)
	endif()
endmacro()
