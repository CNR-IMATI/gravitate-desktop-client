find_package(Qt5 COMPONENTS Widgets Help REQUIRED)

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/helpdialog.h"
    "${CMAKE_CURRENT_LIST_DIR}/helphandler.h"
)

QT5_WRAP_CPP(_MOC_FILES  ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/helpbrowser.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/helpdialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/helphandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/stringresources.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/helpdialog.ui"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/helpbrowser.h"
        "${CMAKE_CURRENT_LIST_DIR}/stringresources.h"
)

target_link_libraries(${APPLIB} PUBLIC Qt5::Help)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
