find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/backendfactorygravitate.h"
    "${CMAKE_CURRENT_LIST_DIR}/gravitateclipgroupsprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/gravitatethumbnailprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/gravitatejobmanager.h"
    "${CMAKE_CURRENT_LIST_DIR}/gravitatejobwatcher.h"
    "${CMAKE_CURRENT_LIST_DIR}/gravitateloginprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/gravitateitemlistprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/gravitatemetadatachprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/metadata3drequestgravitate.h"
)

 QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/backendfactorygravitate.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitatebackend.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitateclipgroupsprovider.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitateextrametadata.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitatesparql.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitatethumbnailprovider.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitatejobmanager.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitatejobwatcher.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitateloginprovider.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitateitemlistprovider.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitatemetadatachprovider.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/metadata3drequestgravitate.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/gravitatebackendconf.h"
        "${CMAKE_CURRENT_LIST_DIR}/gravitatebackend.h"
        "${CMAKE_CURRENT_LIST_DIR}/gravitateextrametadata.h"
        "${CMAKE_CURRENT_LIST_DIR}/gravitatesparql.h"
)

target_include_directories(${APPLIB}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(${APPLIB} PUBLIC Qt5::Core Qt5::Network csv)
