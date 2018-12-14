project(standalone-mesh-viewer LANGUAGES CXX)

if(POLICY CMP0071)
    message("Disable processing of generated files in AUTOUIC and AUTORCC")
    cmake_policy(SET CMP0071 OLD)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(UI_FILES
    standalonemeshviewer.ui
)

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/standalonemeshviewer.h"
    "${CMAKE_CURRENT_LIST_DIR}/tweakparamwidget.h"
    "${CMAKE_CURRENT_LIST_DIR}/qdoubleslider.h"
    )

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

set(CXX_FILES
    "${CMAKE_CURRENT_LIST_DIR}/standalonemeshviewer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tweakparamwidget.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/qdoubleslider.cpp"
    )

add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${_MOC_FILES}
        ${HEADER_FILES}
        ${CXX_FILES}
        ${UI_FILES}
        ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE common)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
