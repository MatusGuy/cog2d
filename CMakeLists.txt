cmake_minimum_required(VERSION 3.14)

project(COG2D LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)

# Options #

set(COG2D_ASSET_PATH "${CMAKE_SOURCE_DIR}" CACHE STRING "Where cog2d looks for assets.")
set(COG2D_GRAPHICS_BACKEND SDL2 CACHE STRING "Backend to use for graphics")
set(COG2D_AUDIO_BACKEND ALSOFT CACHE STRING "Backend to use for graphics")

option(COG2D_TESTS "Add little programs that test cog2d." NO)
option(COG2D_UTIL "Add little utilities for cog2d development." NO)
option(COG2D_USE_FMT "Use fmt formatting library for cog2d instead of std::format" NO)
option(COG2D_GRAPHICS_USE_INT "Disable SDL's subpixel rendering." NO)

# Utilities #

# For use in the dependant project
# Adds all public cog2d CMake functions
macro(cog2d_project_setup)
	list(APPEND CMAKE_MODULE_PATH "${COG2D_SOURCE_DIR}/cmake/")
	include(Cog2dGame)
endmacro()

function(mklink link target)
	file(TO_NATIVE_PATH "${link}" nlink)
	file(TO_NATIVE_PATH "${target}" ntarget)
	if(WIN32)
		execute_process(COMMAND cmd /C "mklink /D /J ${nlink} ${ntarget}"
			            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
						ERROR_VARIABLE result)
		message(DEBUG "mklink result: ${result}")
	else()
		file(CREATE_LINK "${ntarget}" "${nlink}" SYMBOLIC)
	endif()
endfunction()

# Dependencies #

find_package(PkgConfig REQUIRED)

find_package(SDL2 REQUIRED)
pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_ttf REQUIRED IMPORTED_TARGET SDL2_ttf)
pkg_check_modules(OpenAL REQUIRED IMPORTED_TARGET openal)

add_subdirectory(external/qoa)
add_subdirectory(external/tomlc)

## fmt ##
if(COG2D_USE_FMT OR (COG2D_CXX_STANDARD LESS 20))
	pkg_check_modules(fmt REQUIRED IMPORTED_TARGET fmt)
	set(COG2D_USE_FMT YES)
endif()

# Sources #

set(COG2D_CXX_STANDARD 20
	CACHE STRING "C++ standard version to be used by cog2d and its dependants")

if(COG2D_CXX_STANDARD LESS 17)
	message(FATAL_ERROR "cog2d only supports C++17 or higher.")
endif()

set(CMAKE_CXX_STANDARD ${COG2D_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(cog2d INTERFACE)
target_sources(cog2d INTERFACE
	src/cog2d/assets/assetcollection.cpp
	src/cog2d/assets/assetmanager.cpp
	src/cog2d/audio/alsoftaudioengine.cpp
	src/cog2d/audio/audioengine.cpp
	src/cog2d/audio/musicplayer.cpp
	src/cog2d/audio/musictrack.cpp
	src/cog2d/audio/soundeffect.cpp
	src/cog2d/audio/musictrackparser.cpp
	src/cog2d/filesystem/file.cpp
	src/cog2d/input/inputmanager.cpp
	src/cog2d/program.cpp
	src/cog2d/ecs/builtins.cpp
	src/cog2d/ecs/builtins/texture.cpp
	src/cog2d/ecs/entitybase.cpp
	src/cog2d/util/timer.cpp
	src/cog2d/util/timing.cpp
	src/cog2d/util/toml.cpp
	src/cog2d/video/font/sdl2ttffont.cpp
	src/cog2d/video/font/fontcache.cpp
	src/cog2d/video/font/pixmapfont.cpp
	src/cog2d/video/font/pixmapfontparser.cpp
	src/cog2d/video/graphicsengine.cpp
	src/cog2d/video/sdl2graphicsengine.cpp
	src/cog2d/video/font/sdl2ttffont.cpp
	src/cog2d/video/surface.cpp
	src/cog2d/video/texture.cpp
	src/cog2d/tilemap/tilemapparser.cpp
	src/cog2d/tilemap/tilemap.hpp
	src/cog2d/tilemap/tilelayer.hpp
	src/cog2d/tilemap/tileset.hpp
	src/cog2d/tilemap/tileset.cpp
	src/cog2d/tilemap/tilemap.cpp
	src/cog2d/tilemap/tilelayer.cpp
	src/cog2d/tilemap/tile.hpp

	src/cog2d/assets/asset.hpp
	src/cog2d/assets/assetcollection.hpp
	src/cog2d/assets/assetmanager.hpp
	src/cog2d/audio/alsoftaudioengine.hpp
	src/cog2d/audio/audioengine.hpp
	src/cog2d/audio/musicplayer.hpp
	src/cog2d/audio/musictrack.hpp
	src/cog2d/audio/soundeffect.hpp
	src/cog2d/filesystem/file.hpp
	src/cog2d/input/inputmanager.hpp
	src/cog2d/program.hpp
	src/cog2d/ecs/activestate.hpp
	src/cog2d/ecs/entitybase.hpp
	src/cog2d/ecs/world.hpp
	src/cog2d/ecs/builtins.hpp
	src/cog2d/ecs/builtins/texture.hpp
	src/cog2d/ecs/builtins/properties.hpp
	src/cog2d/ecs/builtins/collision/collision.cpp
	src/cog2d/ecs/builtins/collision/collisionsystem.cpp
	src/cog2d/screen.hpp
	src/cog2d/util/backend.hpp
	src/cog2d/util/buffer.hpp
	src/cog2d/util/currenton.hpp
	src/cog2d/util/fmt.hpp
	src/cog2d/util/logger.hpp
	src/cog2d/util/math/rect.hpp
	src/cog2d/util/math/util.hpp
	src/cog2d/util/math/vector.hpp
	src/cog2d/util/toml.hpp
	src/cog2d/util/timer.hpp
	src/cog2d/util/timing.hpp
	src/cog2d/util/typetraits.hpp
	src/cog2d/video/color.hpp
	src/cog2d/video/font/ttffont.hpp
	src/cog2d/video/font/fontcache.hpp
	src/cog2d/video/font/pixmapfont.hpp
	src/cog2d/video/graphicsengine.hpp
	src/cog2d/video/sdl2graphicsengine.hpp
	src/cog2d/video/surface.hpp
	src/cog2d/video/texture.hpp)

target_compile_features(cog2d INTERFACE cxx_std_${COG2D_CXX_STANDARD})

#target_compile_options(cog2d PUBLIC -fno-rtti)

target_include_directories(cog2d SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(cog2d INTERFACE
	SDL2::SDL2
	PkgConfig::SDL2_image
	PkgConfig::SDL2_ttf
	PkgConfig::OpenAL
	qoa
	tomlc
)

if(COG2D_USE_FMT)
	target_link_libraries(cog2d INTERFACE PkgConfig::fmt)
endif()

# Warnings #
if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(cog2d INTERFACE -Wunused-function)
endif()

# Preprocessor definitions #
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(cog2d INTERFACE COG2D_DEBUG)
endif()

target_compile_definitions(cog2d INTERFACE COG2D_ASSET_PATH="${COG2D_ASSET_PATH}")

if(COG2D_GRAPHICS_USE_INT)
	target_compile_definitions(cog2d INTERFACE COG2D_GRAPHICS_USE_INT)
endif()

target_compile_definitions(cog2d INTERFACE COG2D_GRAPHICS_BACKEND_${COG2D_GRAPHICS_BACKEND})
target_compile_definitions(cog2d INTERFACE COG2D_AUDIO_BACKEND_${COG2D_GRAPHICS_BACKEND})

# Misc. #

if(COG2D_TESTS)
	add_subdirectory(tests)
endif()

if(COG2D_UTIL)
	add_custom_target(tidy_files
		COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${PROJECT_SOURCE_DIR}
		                         -P ${PROJECT_SOURCE_DIR}/cmake/Cog2dTidySources.cmake
		COMMENT "Tidying source files"
	)

	add_subdirectory(tools)
endif()
