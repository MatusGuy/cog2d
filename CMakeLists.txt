cmake_minimum_required(VERSION 3.14)

project(COG2D LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

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

file(GLOB_RECURSE COG2D_SRC CONFIGURE_DEPENDS src/*.cpp include/cog2d/*.hpp)
add_library(cog2d STATIC ${COG2D_SRC})

target_compile_features(cog2d PUBLIC cxx_std_${COG2D_CXX_STANDARD})

#target_compile_options(cog2d PUBLIC -fno-rtti)

target_include_directories(cog2d SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Get all include subdirectories so that it becomes possible to just write the name of the header
# in source (cpp) files.
set(COG2D_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE _cog2d_includes LIST_DIRECTORIES true ${COG2D_INCLUDE_DIRECTORY}/cog2d/*)
list(FILTER _cog2d_includes EXCLUDE REGEX "[.][A-Za-z0-9]+$")
target_include_directories(cog2d PRIVATE ${_cog2d_includes} ${COG2D_INCLUDE_DIRECTORY}/cog2d)

# Dependencies #

find_package(PkgConfig QUIET)
option(COG2D_USE_PKGCONFIG "Use pkgconfig for finding dependencies" ${PKG_CONFIG_FOUND})

## SDL2 ##
find_package(SDL2 REQUIRED)
target_link_libraries(cog2d
	PUBLIC
	$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

## SDL2_image ##
if(COG2D_USE_PKGCONFIG)
	pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)
	target_link_libraries(cog2d PUBLIC PkgConfig::SDL2_image)
else()
	find_package(SDL2_image REQUIRED)
	target_link_libraries(cog2d
		PUBLIC
		$<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,
		     SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
	)
endif()

## OpenAL ##
if(COG2D_USE_PKGCONFIG)
	pkg_check_modules(OpenAL REQUIRED IMPORTED_TARGET openal)
	target_link_libraries(cog2d PUBLIC PkgConfig::OpenAL)
else()
	find_package(OpenAL REQUIRED)
	target_link_libraries(cog2d PUBLIC OpenAL::OpenAL)
endif()

## SDL2_ttf ##
if(COG2D_USE_PKGCONFIG)
	pkg_check_modules(SDL2_ttf REQUIRED IMPORTED_TARGET SDL2_ttf)
	target_link_libraries(cog2d PUBLIC PkgConfig::SDL2_ttf)
else()
	find_package(SDL2_ttf REQUIRED)
	target_link_libraries(cog2d
		PUBLIC
		$<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
	)
endif()

## fmt ##
option(COG2D_USE_FMT "Use fmt formatting library for cog2d instead of std::format" NO)
if(COG2D_USE_FMT OR (COG2D_CXX_STANDARD LESS 20))
	if(COG2D_USE_PKGCONFIG)
		pkg_check_modules(fmt REQUIRED IMPORTED_TARGET fmt)
		target_link_libraries(cog2d PUBLIC PkgConfig::fmt)
	else()
		find_package(fmt REQUIRED)
		target_link_libraries(cog2d PUBLIC fmt::fmt)
	endif()
	target_compile_definitions(cog2d PUBLIC COG2D_USE_FMT)
endif()

## toml++ ##

if(COG2D_USE_PKGCONFIG)
	pkg_check_modules(tomlplusplus REQUIRED IMPORTED_TARGET tomlplusplus)
	target_link_libraries(cog2d PUBLIC PkgConfig::tomlplusplus)
else()
	find_package(tomlplusplus REQUIRED)
	target_link_libraries(cog2d PUBLIC tomlplusplus::tomlplusplus)
endif()

## external ##
add_subdirectory(external/qoa)
target_link_libraries(cog2d PUBLIC qoa)

# Preprocessor definitions #
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(cog2d PUBLIC COG2D_DEBUG)
endif()

set(COG2D_ASSET_PATH "${CMAKE_SOURCE_DIR}" CACHE STRING "Where cog2d looks for assets.")
target_compile_definitions(cog2d PUBLIC COG2D_ASSET_PATH="${COG2D_ASSET_PATH}")

option(COG2D_GRAPHICS_USE_INT "Disable SDL's subpixel rendering." NO)
if(COG2D_GRAPHICS_USE_INT)
	target_compile_definitions(cog2d PUBLIC COG2D_GRAPHICS_USE_INT)
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
