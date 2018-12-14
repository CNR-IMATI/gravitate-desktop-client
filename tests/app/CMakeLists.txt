cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(tests-app LANGUAGES CXX)

find_package(Qt5 COMPONENTS Widgets Test REQUIRED)

add_executable(${PROJECT_NAME} "")

list(APPEND HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/include/mockbackendprovider.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mockdownloader.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mocknetworkaccessmanager.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mockdataproviders.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mockjobmanager.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/qfakenetworkreply.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${PROJECT_NAME} PRIVATE

    "${CMAKE_CURRENT_LIST_DIR}/../tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../include/qstringprint.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sparqlreplybuilder.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/qfakenetworkreply.cpp"
#    "${CMAKE_CURRENT_LIST_DIR}/backend/networkbackendprovider_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/backend/downloader_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/backend/jobmanager_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/backend/featureextractionalgorithm_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/backend/featureextraction_jobmanager_integration_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/clipboard/thumbnailprovider_tests.cpp"
#    "${CMAKE_CURRENT_LIST_DIR}/clipboard/clipboardmodel_integration_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/clipboard/clipboardmodel_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/clipboard/clipgroupsprovider_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/clipboard/qhashtree_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/help/stringresources_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/dialogs/parameterdialog_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/metadata/metadata3drequest_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/metadata/metadataapi_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/metadata/gravitateextrametadata_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/repository/repofix_tests.cpp"

    ${_MOC_FILES}

    PUBLIC
    ${HEADER_FILES}
    "${CMAKE_CURRENT_LIST_DIR}/include/sparqlreplybuilder.h"

    "${CMAKE_SOURCE_DIR}/resources/resources.qrc"
    "${CMAKE_CURRENT_LIST_DIR}/../resources/test_resources.qrc"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${CMAKE_CURRENT_LIST_DIR}/../include"
)


# Linking to libraries and client code
target_link_libraries(${PROJECT_NAME} PRIVATE gtest gmock)
target_link_libraries(${PROJECT_NAME} PRIVATE gravitate-client-lib)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Test)
