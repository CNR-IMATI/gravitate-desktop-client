project(common LANGUAGES CXX)

if(POLICY CMP0071)
    message("Disable processing of generated files in AUTOUIC and AUTORCC")
    cmake_policy(SET CMP0071 OLD)
endif()

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

set(LIBNAME "common")

add_library(${LIBNAME} STATIC "")

include(${CMAKE_CURRENT_SOURCE_DIR}/annotation/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/core/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/input/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/mesh/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/util/CMakeLists.txt)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
