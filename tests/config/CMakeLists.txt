cmake_minimum_required(VERSION 3.14)

project(cog2d_test_config LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(Cog2dTests)

cog2d_add_test(${PROJECT_NAME})
