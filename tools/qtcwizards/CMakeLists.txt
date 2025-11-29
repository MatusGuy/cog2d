set(_wizards)
set(_tar_prefix qtcwizard_install_)

macro(add_wizard _wizard)
	set(_tar_name "${_tar_prefix}${_wizard}")
	set(_wizarddir "QtProject/qtcreator/templates/wizards")
	if(CMAKE_HOST_WIN32)
		set(_userdir "$ENV{APPDATA}")
	else()
		set(_userdir "$ENV{HOME}/.config/")
	endif()
	add_custom_target(${_tar_name}
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${_wizard}
												   ${_userdir}/${_wizarddir}/${_wizard}
	)
	list(APPEND _wizards "${_wizard}")
endmacro()

add_wizard(component_system)

find_program(QTCREATOR_COMMAND NAMES qtcreator)

if(QTCREATOR_COMMAND)
	add_custom_target(qtcwizard_test COMMAND ${QTCREATOR_COMMAND} -customwizard-verbose -color green
																  -noload all -load CppEditor)
	set_target_properties(qtcwizard_test PROPERTIES FOLDER "qtc_runnable")
	foreach(_wizard ${_wizards})
		add_dependencies(qtcwizard_test ${_tar_prefix}${_wizard})
	endforeach()
endif()
