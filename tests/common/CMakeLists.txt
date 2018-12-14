cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(tests-common LANGUAGES CXX)

find_package(Qt5 COMPONENTS Widgets Test REQUIRED)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/../tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../include/qstringprint.h"

    "${CMAKE_CURRENT_LIST_DIR}/annotation/webannotation_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/annotation/threedselector_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mesh/scalarfieldreader_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mesh/scalarfield/util_math_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mesh/scalarfield/unit_conversion_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mesh/facet_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mesh/selectionutils_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mesh/boundingbox_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mesh/houghfeature_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/reassembly/reassembly_tests.cpp"

    "${CMAKE_SOURCE_DIR}/resources/resources.qrc"
    "${CMAKE_CURRENT_LIST_DIR}/../resources/test_resources.qrc"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_LIST_DIR}/../include"
)

# Linking to libraries and client code
target_link_libraries(${PROJECT_NAME} PRIVATE gtest gmock)
target_link_libraries(${PROJECT_NAME} PRIVATE common)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Test)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann-json)
