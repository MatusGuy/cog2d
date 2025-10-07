cmake_minimum_required(VERSION 3.14)

project(COG2D LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)

# Utilities #

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
	src/assets/assetcollection.cpp
	src/assets/assetmanager.cpp
	src/audio/alsoft/alsoftaudioengine.cpp
	src/audio/audioengine.cpp
	src/audio/musicplayer.cpp
	src/audio/musictrack.cpp
	src/audio/sdl2/sdl2audioengine.cpp
	src/audio/soundeffect.cpp
	src/audio/tomlmusictrackparser.cpp
	src/filesystem/assetfile.cpp
	src/filesystem/file.cpp
	src/filesystem/iodevice.cpp
	src/input/inputmanager.cpp
	src/program.cpp
	src/scene/actor.cpp
	src/scene/actormanager.cpp
	src/scene/actorrefsiterator.cpp
	src/scene/actorstage.cpp
	src/scene/camera.cpp
	src/scene/collision/collision.cpp
	src/scene/collision/collisionbody.cpp
	src/scene/collision/collisionsystem.cpp
	src/scene/scene.cpp
	src/scene/tilemap/bintilemapparser.cpp
	src/scene/tilemap/cameratilelayeriterator.cpp
	src/scene/tilemap/tilelayer.cpp
	src/scene/tilemap/tilemap.cpp
	src/scene/tilemap/tilescene.cpp
	src/scene/tilemap/tileset.cpp
	src/scene/viewport.cpp
	src/util/logger.cpp
	src/util/timer.cpp
	src/util/timing.cpp
	src/video/font/font.cpp
	src/video/font/fontcache.cpp
	src/video/font/pixmapfont.cpp
	src/video/font/tomlpixmapfontparser.cpp
	src/video/graphicsengine.cpp
	src/video/sdl2/font/sdl2ttffont.cpp
	src/video/sdl2/sdl2graphicsengine.cpp
	src/video/sdl2/sdl2texture.cpp
	src/video/surface.cpp
	src/video/texture.cpp

	include/cog2d/assets/asset.hpp
	include/cog2d/assets/assetcollection.hpp
	include/cog2d/assets/assetmanager.hpp
	include/cog2d/audio/alsoft/alsoftaudioengine.hpp
	include/cog2d/audio/audioengine.hpp
	include/cog2d/audio/musicplayer.hpp
	include/cog2d/audio/musictrack.hpp
	include/cog2d/audio/sdl2/sdl2audioengine.hpp
	include/cog2d/audio/soundeffect.hpp
	include/cog2d/audio/tomlmusictrackparser.hpp
	include/cog2d/filesystem/assetfile.hpp
	include/cog2d/filesystem/file.hpp
	include/cog2d/filesystem/iodevice.hpp
	include/cog2d/input/inputmanager.hpp
	include/cog2d/program.hpp
	include/cog2d/scene/actor.hpp
	include/cog2d/scene/actorcomponents.hpp
	include/cog2d/scene/actorcontainers.hpp
	include/cog2d/scene/actorfactory.hpp
	include/cog2d/scene/actormanager.hpp
	include/cog2d/scene/actorrefsiterator.hpp
	include/cog2d/scene/actorstage.hpp
	include/cog2d/scene/camera.hpp
	include/cog2d/scene/collision/collision.hpp
	include/cog2d/scene/collision/collisionbody.hpp
	include/cog2d/scene/collision/collisionsystem.hpp
	include/cog2d/scene/scene.hpp
	include/cog2d/scene/tilemap/bintilemapparser.hpp
	include/cog2d/scene/tilemap/cameratilelayeriterator.hpp
	include/cog2d/scene/tilemap/tile.hpp
	include/cog2d/scene/tilemap/tilelayer.hpp
	include/cog2d/scene/tilemap/tilemap.hpp
	include/cog2d/scene/tilemap/tilescene.hpp
	include/cog2d/scene/tilemap/tileset.hpp
	include/cog2d/scene/viewport.hpp
	include/cog2d/screen.hpp
	include/cog2d/util/backend.hpp
	include/cog2d/util/buffer.hpp
	include/cog2d/util/currenton.hpp
	include/cog2d/util/fmt.hpp
	include/cog2d/util/logger.hpp
	include/cog2d/util/math/rect.hpp
	include/cog2d/util/math/util.hpp
	include/cog2d/util/math/vector.hpp
	include/cog2d/util/parsing.hpp
	include/cog2d/util/timer.hpp
	include/cog2d/util/timing.hpp
	include/cog2d/util/typetraits.hpp
	include/cog2d/video/color.hpp
	include/cog2d/video/font/font.hpp
	include/cog2d/video/font/fontcache.hpp
	include/cog2d/video/font/pixmapfont.hpp
	include/cog2d/video/font/tomlpixmapfontparser.hpp
	include/cog2d/video/graphicsengine.hpp
	include/cog2d/video/sdl2/font/sdl2ttffont.hpp
	include/cog2d/video/sdl2/sdl2graphicsengine.hpp
	include/cog2d/video/sdl2/sdl2texture.hpp
	include/cog2d/video/surface.hpp
	include/cog2d/video/texture.hpp)

target_compile_features(cog2d INTERFACE cxx_std_${COG2D_CXX_STANDARD})

#target_compile_options(cog2d PUBLIC -fno-rtti)

target_include_directories(cog2d SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/include)

# Get all include subdirectories so that it becomes possible to just write the name of the header
# in source (cpp) files.
set(COG2D_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE _cog2d_includes LIST_DIRECTORIES true ${COG2D_INCLUDE_DIRECTORY}/cog2d/*)
list(FILTER _cog2d_includes EXCLUDE REGEX "[.][A-Za-z0-9]+$")
target_include_directories(cog2d INTERFACE ${_cog2d_includes} ${COG2D_INCLUDE_DIRECTORY}/cog2d)

# Dependencies #

find_package(PkgConfig QUIET)
option(COG2D_USE_PKGCONFIG "Use pkgconfig for finding dependencies" ${PKG_CONFIG_FOUND})

## SDL2 ##
find_package(SDL2 REQUIRED)
target_link_libraries(cog2d
	INTERFACE
	$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

## SDL2_image ##
if(COG2D_USE_PKGCONFIG)
	pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)
	target_link_libraries(cog2d INTERFACE PkgConfig::SDL2_image)
else()
	find_package(SDL2_image REQUIRED)
	target_link_libraries(cog2d
		INTERFACE
		$<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,
		     SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
	)
endif()

## OpenAL ##
if(COG2D_USE_PKGCONFIG)
	pkg_check_modules(OpenAL REQUIRED IMPORTED_TARGET openal)
	target_link_libraries(cog2d INTERFACE PkgConfig::OpenAL)
else()
	find_package(OpenAL REQUIRED)
	target_link_libraries(cog2d INTERFACE OpenAL::OpenAL)
endif()

## SDL2_ttf ##
if(COG2D_USE_PKGCONFIG)
	pkg_check_modules(SDL2_ttf REQUIRED IMPORTED_TARGET SDL2_ttf)
	target_link_libraries(cog2d INTERFACE PkgConfig::SDL2_ttf)
else()
	find_package(SDL2_ttf REQUIRED)
	target_link_libraries(cog2d
		INTERFACE
		$<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
	)
endif()

## fmt ##
option(COG2D_USE_FMT "Use fmt formatting library for cog2d instead of std::format" NO)
if(COG2D_USE_FMT OR (COG2D_CXX_STANDARD LESS 20))
	if(COG2D_USE_PKGCONFIG)
		pkg_check_modules(fmt REQUIRED IMPORTED_TARGET fmt)
		target_link_libraries(cog2d INTERFACE PkgConfig::fmt)
	else()
		find_package(fmt REQUIRED)
		target_link_libraries(cog2d INTERFACE fmt::fmt)
	endif()
	target_compile_definitions(cog2d INTERFACE COG2D_USE_FMT)
endif()

## toml++ ##

if(COG2D_USE_PKGCONFIG)
	pkg_check_modules(tomlplusplus REQUIRED IMPORTED_TARGET tomlplusplus)
	target_link_libraries(cog2d INTERFACE PkgConfig::tomlplusplus)
else()
	find_package(tomlplusplus REQUIRED)
	target_link_libraries(cog2d INTERFACE tomlplusplus::tomlplusplus)
endif()

## external ##
add_subdirectory(external/qoa)
target_link_libraries(cog2d INTERFACE qoa)

# Preprocessor definitions #
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(cog2d INTERFACE COG2D_DEBUG)
endif()

set(COG2D_ASSET_PATH "${CMAKE_SOURCE_DIR}" CACHE STRING "Where cog2d looks for assets.")
target_compile_definitions(cog2d INTERFACE COG2D_ASSET_PATH="${COG2D_ASSET_PATH}")

option(COG2D_GRAPHICS_USE_INT "Disable SDL's subpixel rendering." NO)
if(COG2D_GRAPHICS_USE_INT)
	target_compile_definitions(cog2d INTERFACE COG2D_GRAPHICS_USE_INT)
endif()

# Tests #

option(COG2D_TESTS "Add little programs that test cog2d." NO)

if(COG2D_TESTS)
	add_subdirectory(tests)
endif()

option(COG2D_UTIL "Add little utilities for cog2d development." NO)
if(COG2D_UTIL)
	add_custom_target(tidy_files
		COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${PROJECT_SOURCE_DIR}
		                         -P ${PROJECT_SOURCE_DIR}/cmake/Cog2dTidySources.cmake
		COMMENT "Tidying source files"
	)

	add_subdirectory(tools)
endif()
