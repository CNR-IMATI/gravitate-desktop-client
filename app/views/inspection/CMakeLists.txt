
set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/inspectionviewmain.h"
    "${CMAKE_CURRENT_LIST_DIR}/inspectionitemwidget.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/inspectionviewmain.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/inspectionviewmain.ui"
        "${CMAKE_CURRENT_LIST_DIR}/inspectionitemwidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/inspectionitemwidget.ui"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
