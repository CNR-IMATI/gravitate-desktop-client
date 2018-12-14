include("${CMAKE_CURRENT_LIST_DIR}/inspection/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/fragment/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/reassembly/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/history/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/groups/CMakeLists.txt")

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/abstractview.h"
    "${CMAKE_CURRENT_LIST_DIR}/toolsetwidget.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/abstractview.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/toolsetwidget.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/abstractdashboard.h"
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
