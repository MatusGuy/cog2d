cmake_minimum_required(VERSION 3.14)
project(QOA LANGUAGES C)

add_library(qoa STATIC qoa.c qoa.h)
target_include_directories(qoa PUBLIC "${PROJECT_SOURCE_DIR}")
