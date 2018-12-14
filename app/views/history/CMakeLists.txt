set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/historyviewmain.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

# TODO: Split in multiple list files
target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/historyviewmain.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/historyviewmain.ui"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
