find_package(Qt5 COMPONENTS Widgets Help REQUIRED)

set(HEADER_FILES
  "${CMAKE_CURRENT_LIST_DIR}/browsedialog.h"
  "${CMAKE_CURRENT_LIST_DIR}/datasetfetchmodel.h"
)

QT5_WRAP_CPP(_MOC_FILES
  ${HEADER_FILES}
)


target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/browsedialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/browsedialog.ui"
        "${CMAKE_CURRENT_LIST_DIR}/datasetfetchmodel.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

target_link_libraries(${APPLIB} PUBLIC Qt5::Core Qt5::Widgets Qxt)
