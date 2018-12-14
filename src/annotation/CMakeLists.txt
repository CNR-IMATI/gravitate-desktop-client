set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/annotationhelper.h"
    "${CMAKE_CURRENT_LIST_DIR}/annotationdialog.h"
    "${CMAKE_CURRENT_LIST_DIR}/annotationmodelbase.h"
    "${CMAKE_CURRENT_LIST_DIR}/partonomywidget.h"
    "${CMAKE_CURRENT_LIST_DIR}/localannotationmodel.h"
    "${CMAKE_CURRENT_LIST_DIR}/thesaurusmodel.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${LIBNAME}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/annotationhelper.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/annotationdialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/annotationdialog.ui"
        "${CMAKE_CURRENT_LIST_DIR}/annotationmodelbase.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/partonomywidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/partonomywidget.ui"
        "${CMAKE_CURRENT_LIST_DIR}/localannotationmodel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/skostreeparsersord.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/thesaurusmodel.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/term.h"
        "${CMAKE_CURRENT_LIST_DIR}/skostreeparsersord.h"
)

target_link_libraries(common PRIVATE Qt5::Widgets serd-sord)

target_include_directories(${LIBNAME}
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    "${CEREAL_INCLUDE_DIRS}"
    $<TARGET_PROPERTY:serd-sord,INCLUDE_DIRECTORIES>
    PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
)
