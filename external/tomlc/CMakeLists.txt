cmake_minimum_required(VERSION 3.14)
project(TOMLC LANGUAGES C)

add_library(tomlc STATIC tomlc.c tomlc.h)
target_include_directories(tomlc PUBLIC "${PROJECT_SOURCE_DIR}")
