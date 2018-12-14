cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(imati-desktop-client)

option(BUILD_DOC "Build documentation" OFF)

# This policy has to be set at the top level CMakeLists
if(POLICY CMP0053)
  # Use the simplified variable reference and escape sequence evaluation
  message( STATUS "Simplified variable reference rule ON")
  cmake_policy(SET CMP0053 NEW)
endif()

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Config)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/app)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/debug-gui/standalone-mesh-viewer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Deployment scripts directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deployment)

# User manual
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/docs)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/docs/help)
