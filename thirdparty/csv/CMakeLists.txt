cmake_minimum_required(VERSION 3.0)
project(csv)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS
    "${CMAKE_CURRENT_LIST_DIR}/csv.h"
)

set_source_files_properties(${HEADERS} PROPERTIES LANGUAGE CXX)

add_library(csv INTERFACE)

target_include_directories(csv INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}")

