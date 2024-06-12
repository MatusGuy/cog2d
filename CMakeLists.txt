cmake_minimum_required(VERSION 3.14)

project(cog2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

# Sources #
file(GLOB_RECURSE COG2D_SRC CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_library(cog2d SHARED ${COG2D_SRC})

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
if(USE_PKGCONFIG)
	pkg_check_modules(SDL2_image SDL2_image)
else()
	find_package(SDL2_image REQUIRED)
endif()
target_link_libraries(cog2d
	PUBLIC
	$<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)

## SDL2_ttf ##
## SDL2_mixer ##

target_include_directories(cog2d PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Tests #
add_subdirectory(tests)

