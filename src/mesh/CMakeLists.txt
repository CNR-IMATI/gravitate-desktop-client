find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(VTK REQUIRED)

include(${VTK_USE_FILE})

set(QT_MOC_HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/annotation/annotationcaptions.h"
    "${CMAKE_CURRENT_LIST_DIR}/annotation/annotationtool.h"
    "${CMAKE_CURRENT_LIST_DIR}/annotation/selectiontool.h"
    "${CMAKE_CURRENT_LIST_DIR}/facet/facetoverlay.h"
    "${CMAKE_CURRENT_LIST_DIR}/core/meshviewwidget.h"
    "${CMAKE_CURRENT_LIST_DIR}/core/coordinateaxes.h"
    "${CMAKE_CURRENT_LIST_DIR}/core/viewermodule.h"
    "${CMAKE_CURRENT_LIST_DIR}/core/measuretool.h"
    "${CMAKE_CURRENT_LIST_DIR}/core/mesh.h"
    "${CMAKE_CURRENT_LIST_DIR}/core/valuepicker.h"
    "${CMAKE_CURRENT_LIST_DIR}/property/propertycoloring.h"
    "${CMAKE_CURRENT_LIST_DIR}/property/wireframebox.h"
)

QT5_WRAP_CPP(_MOC_FILES ${QT_MOC_HEADER_FILES})

target_sources(${LIBNAME}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/annotation/annotationcaptions.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/annotation/annotationtool.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/annotation/selectiontool.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/annotation/selectionutils.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/annotation/houghfeaturetool.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/facet/facetoverlay.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/core/mesh.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/core/viewermodule.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/core/meshviewwidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/core/meshviewwidget.ui"
        "${CMAKE_CURRENT_LIST_DIR}/core/coordinateaxes.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/core/measuretool.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/core/valuepicker.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/property/propertycoloring.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/property/propertyrendering.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/property/transferfunctionfactory.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/property/wireframebox.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/vtk/interactorstylevertexpicker.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/vtk/interactorstylelazoselector.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/vtk/interactorstylebrushselector.cpp"
        ${_MOC_FILES}
    PUBLIC
        ${HEADER_FILES}
        "${CMAKE_CURRENT_LIST_DIR}/core/graphicssettings.h"
        "${CMAKE_CURRENT_LIST_DIR}/property/propertyrendering.h"
        "${CMAKE_CURRENT_LIST_DIR}/annotation/houghfeaturetool.h"
        "${CMAKE_CURRENT_LIST_DIR}/annotation/selectionutils.h"
        "${CMAKE_CURRENT_LIST_DIR}/property/transferfunctionfactory.h"
        "${CMAKE_CURRENT_LIST_DIR}/vtk/highlightinteractorstyle.h"
        "${CMAKE_CURRENT_LIST_DIR}/vtk/interactorstylevertexpicker.h"
        "${CMAKE_CURRENT_LIST_DIR}/vtk/interactorstylelazoselector.h"
        "${CMAKE_CURRENT_LIST_DIR}/vtk/interactorstylebrushselector.h"
)

target_include_directories(${LIBNAME}
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
)


target_link_libraries(${LIBNAME} PRIVATE Qt5::Widgets)
target_link_libraries(${LIBNAME} PRIVATE ${VTK_LIBRARIES})
