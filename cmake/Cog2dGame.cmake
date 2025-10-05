function(cog2d_add_game _name)
	add_executable(${_name} ${ARGN})
	target_link_libraries(${_name} PRIVATE cog2d)
endfunction()
