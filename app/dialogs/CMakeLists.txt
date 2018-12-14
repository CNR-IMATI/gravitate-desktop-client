find_package(Qt5 COMPONENTS Widgets Help REQUIRED)

set(HEADER_FILES
  "${CMAKE_CURRENT_LIST_DIR}/logindialog.h"
  "${CMAKE_CURRENT_LIST_DIR}/parameterselectiondialog.h"
  "${CMAKE_CURRENT_LIST_DIR}/resolutionchangedialog.h"
  "${CMAKE_CURRENT_LIST_DIR}/textnoteedit.h"
)

QT5_WRAP_CPP(_MOC_FILES
  ${HEADER_FILES}
)


target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/logindialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/logindialog.ui"
        "${CMAKE_CURRENT_LIST_DIR}/parameterselectiondialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/resolutionchangedialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/textnoteedit.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/textnoteedit.ui"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
