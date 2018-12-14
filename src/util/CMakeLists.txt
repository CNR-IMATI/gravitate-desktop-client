find_package(Qt5 COMPONENTS Widgets REQUIRED)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${LIBNAME}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/math_utils.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/gravitaterepoutils.cpp"
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/debugmessagehandling.h"
        "${CMAKE_CURRENT_LIST_DIR}/demoutils.h"
        "${CMAKE_CURRENT_LIST_DIR}/math_utils.h"
        "${CMAKE_CURRENT_LIST_DIR}/gravitaterepoutils.h"
        "${CMAKE_CURRENT_LIST_DIR}/qhashtree.h"
)

target_include_directories(${LIBNAME}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}"
)

target_link_libraries(${LIBNAME} PRIVATE Qt5::Widgets)
