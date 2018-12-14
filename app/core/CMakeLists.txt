find_package(Qt5 COMPONENTS Core REQUIRED)

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/gresource.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/artefactid.h"
        "${CMAKE_CURRENT_LIST_DIR}/groupid.h"
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(${APPLIB} PUBLIC Qt5::Core)
