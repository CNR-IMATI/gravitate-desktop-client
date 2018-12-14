find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/itemlistproviderbase.h"
    "${CMAKE_CURRENT_LIST_DIR}/loginprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/metadata3drequest.h"
    "${CMAKE_CURRENT_LIST_DIR}/metadatachprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/clipgroupsprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/jobmanagerbase.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/backendfactorybase.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/metadata3drequest.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/backendfactorybase.h"
)

target_include_directories(${APPLIB}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(${APPLIB} PUBLIC Qt5::Core Qt5::Network)
