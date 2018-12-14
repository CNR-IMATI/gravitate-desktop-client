find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/thumbnailprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/clipboardmodel.h"
    "${CMAKE_CURRENT_LIST_DIR}/clipboardpanel.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/thumbnailprovider.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/clipboardsynccommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/clipboardmodel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/clipboardpanel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/clipboardpanel.ui"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/clipboardsynccommand.h"
)

target_include_directories(${APPLIB}
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(${APPLIB} PUBLIC Qt5::Widgets Qt5::Network)
