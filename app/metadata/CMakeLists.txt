find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/metadatamodel.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/artefact3dmetadata.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/metadatamodel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/stringmanipulation.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/artefact3dmetadata.h"
        "${CMAKE_CURRENT_LIST_DIR}/artefacturladapter.h"
        "${CMAKE_CURRENT_LIST_DIR}/stringmanipulation.h"
)

target_include_directories(${APPLIB}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${APPLIB} PUBLIC Qt5::Core Qt5::Widgets Qt5::Network)
