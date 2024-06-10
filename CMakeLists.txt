cmake_minimum_required(VERSION 3.14)

project(cog2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

if(UNIX)
	# ??? ok
	add_compile_options(-lstdc++)
endif()

# Sources #
file(GLOB_RECURSE COG2D_SRC CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_library(cog2d SHARED ${COG2D_SRC})

# Dependencies #
find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
#find_package(SDL2_ttf REQUIRED)
#find_package(SDL2_mixer REQUIRED)

target_link_libraries(cog2d PUBLIC ${SDL2_LIBRARIES})
target_include_directories(cog2d PUBLIC ${SDL2_INCLUDE_DIRS})

target_include_directories(cog2d PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Tests #
add_subdirectory(tests)
