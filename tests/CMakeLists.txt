cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(tests-common LANGUAGES CXX)

add_definitions(-DGTEST_LANGUAGE_CXX11)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# The exclusion of pthreads is restricted only to Windows platforms.
# Setting gtest_disable_pthreads=on on Linux causes run-time errors
if(WIN32)
    set (gtest_disable_pthreads on)
endif()

include(MakeGtestBuild)

config_compiler_and_linker()

enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/app)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common)
