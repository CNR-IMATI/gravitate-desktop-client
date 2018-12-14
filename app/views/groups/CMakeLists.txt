set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/groupsviewmain.h"
    "${CMAKE_CURRENT_LIST_DIR}/groupproxymodel.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/groupsviewmain.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/groupsviewmain.ui"
        "${CMAKE_CURRENT_LIST_DIR}/groupproxymodel.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
