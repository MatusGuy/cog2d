cmake_minimum_required(VERSION 3.14)

project(cog2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Sources #
file(GLOB_RECURSE COG2D_SRC CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_library(cog2d STATIC ${COG2D_SRC})

# Assets #
#source_group(Assets REGULAR_EXPRESSION assets/.*)

# Dependencies #

find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND)
	option(COG2D_USE_PKGCONFIG "Use pkgconfig for finding dependencies" ON)
endif()

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
		$<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
	)
endif()

## SDL2_mixer ##
if(COG2D_USE_PKGCONFIG)
	pkg_check_modules(SDL2_mixer REQUIRED IMPORTED_TARGET SDL2_mixer)
	target_link_libraries(cog2d PUBLIC PkgConfig::SDL2_mixer )
else()
	find_package(SDL2_mixer REQUIRED)
	target_link_libraries(cog2d
		PUBLIC
		$<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
	)
endif()

## SDL2_ttf ##

target_include_directories(cog2d PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Preprocessor definitions #
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	target_compile_definitions(cog2d PUBLIC COG2D_DEBUG)

	target_compile_definitions(cog2d PUBLIC COG2D_ASSET_PATH="${PROJECT_SOURCE_DIR}/assets")
endif()

# Tests #
add_subdirectory(tests)

