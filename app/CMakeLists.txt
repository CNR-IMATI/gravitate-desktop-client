cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(POLICY CMP0071)
    message("Disable processing of generated files in AUTOUIC and AUTORCC")
    cmake_policy(SET CMP0071 OLD)
endif()



project(gravitate-client LANGUAGES CXX)

set(APPLIB "${PROJECT_NAME}-lib")

QT5_WRAP_CPP(_MOC_FILES
    mainwindow.h
)

add_library(${APPLIB}
    ${_MOC_FILES}
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(${APPLIB} PUBLIC common)

target_include_directories(${APPLIB}
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
      ${CMAKE_CURRENT_BINARY_DIR}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/browse/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/backend/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/clipboard/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/core/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/common/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/dialogs/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/metadata/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/repository/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/help/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/views/CMakeLists.txt)


if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${APPLIB} PUBLIC "-Wall" "-Werror=return-type")
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${APPLIB})
