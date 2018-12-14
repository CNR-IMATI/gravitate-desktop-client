find_package(Qt5 COMPONENTS Widgets Help REQUIRED)

set(HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/artefactloadhelper.h"
    "${CMAKE_CURRENT_LIST_DIR}/meshpropertytoolbar.h"
    "${CMAKE_CURRENT_LIST_DIR}/propertyselector.h"
    "${CMAKE_CURRENT_LIST_DIR}/resolutionselector.h"
    "${CMAKE_CURRENT_LIST_DIR}/viewcontainer.h"
    "${CMAKE_CURRENT_LIST_DIR}/viewselectionbar.h"
)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/artefactloadhelper.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/dashboardsdfpropertyrendering.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitatemetadata.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/meshpropertytoolbar.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/propertyselector.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/resolutionselector.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/viewcontainer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/viewselectionbar.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/gravitatemetadata.h"
        "${CMAKE_CURRENT_LIST_DIR}/dashboardsdfpropertyrendering.h"
)

target_include_directories(${APPLIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
