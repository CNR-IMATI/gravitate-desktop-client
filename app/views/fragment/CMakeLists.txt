set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/fragmentviewmain.h"
    "${CMAKE_CURRENT_LIST_DIR}/featureextractionalgorithm.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/fragmentviewmain.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/fragmentviewmain.ui"
        "${CMAKE_CURRENT_LIST_DIR}/featureextractionalgorithm.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
