cmake_minimum_required(VERSION 3.0)
project(nlohmann-json)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS
        "${CMAKE_CURRENT_LIST_DIR}/nlohmann/json.hpp"
)

set_source_files_properties(${HEADERS} PROPERTIES LANGUAGE CXX)

add_library(nlohmann-json INTERFACE)

target_include_directories(nlohmann-json INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}")
