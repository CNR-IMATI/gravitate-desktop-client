cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

# TODO Disable warnings for third-party code

include(BuildSerdSord)
add_subdirectory(json)
add_subdirectory(csv)
add_subdirectory(qxt)
