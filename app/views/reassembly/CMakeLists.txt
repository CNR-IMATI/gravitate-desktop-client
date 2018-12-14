set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/reassemblyviewmain.h"
    "${CMAKE_CURRENT_LIST_DIR}/reassembly3dwidget.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/reassemblyviewmain.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/reassemblyviewmain.ui"
        "${CMAKE_CURRENT_LIST_DIR}/reassembly3dwidget.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
