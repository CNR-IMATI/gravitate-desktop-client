cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Qxt)

find_package(Qt5 COMPONENTS Core REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/qxtsignalwaiter.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

#set_source_files_properties(${HEADER_FILES} PROPERTIES LANGUAGE CXX)

add_library(${PROJECT_NAME} STATIC "")

target_compile_definitions(${PROJECT_NAME} PUBLIC
    QXT_STATIC=1
)

target_sources(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/qxtsignalwaiter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/qxtglobal.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/qxtglobal.h"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)
