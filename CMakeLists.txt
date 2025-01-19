cmake_minimum_required(VERSION 3.14)

project(cog2d LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES)

# Sources #
file(GLOB_RECURSE COG2D_SRC CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_library(cog2d STATIC ${COG2D_SRC})

target_compile_features(cog2d PUBLIC cxx_std_20)

target_include_directories(cog2d PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Dependencies #

find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND)
	option(COG2D_USE_PKGCONFIG "Use pkgconfig for finding dependencies" YES)
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
	target_link_libraries(cog2d PUBLIC PkgConfig::SDL2_mixer)
else()
	find_package(SDL2_mixer REQUIRED)
	target_link_libraries(cog2d
		PUBLIC
		$<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
	)
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

## json ##

include(FetchContent)

# The heck does this warning even mean?? Leave me alone!
if(CMAKE_VERSION VERSION_GREATER "3.24.0")
	cmake_policy(SET CMP0135 OLD)
endif()

FetchContent_Declare(
	json
	URL "https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz"
)
FetchContent_MakeAvailable(json)
target_include_directories(cog2d PUBLIC ${nlohmann_json_SOURCE_DIR}/include/)

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

