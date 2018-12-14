find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/base/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/gravitate/CMakeLists.txt")


set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/abstractdownloader.h"
    "${CMAKE_CURRENT_LIST_DIR}/authenticator.h"
    "${CMAKE_CURRENT_LIST_DIR}/downloadprogressdialog.h"
    "${CMAKE_CURRENT_LIST_DIR}/backendprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/networkbackendprovider.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/abstractdownloader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/authenticator.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/downloadprogressdialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/networkbackendprovider.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/sparqlresults.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/localfilecache.cpp"
        ${_MOC_FILES}
  PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/sparqlresults.h"
        "${CMAKE_CURRENT_LIST_DIR}/localfilecache.h"
)

target_include_directories(${APPLIB}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(${APPLIB} PUBLIC Qt5::Core Qt5::Widgets Qt5::Network Qxt)
