find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

QT5_WRAP_CPP(_MOC_FILES ${HEADER_FILES})

target_sources(${APPLIB}
    PRIVATE
        # "${CMAKE_CURRENT_LIST_DIR}/gravitaterepoutils.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/repofix.cpp"
    PUBLIC
        # "${CMAKE_CURRENT_LIST_DIR}/gravitaterepoutils.h"
        "${CMAKE_CURRENT_LIST_DIR}/gravitaterepoconf.h"
        "${CMAKE_CURRENT_LIST_DIR}/repofix.h"
)

target_include_directories(${APPLIB}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${APPLIB} PUBLIC Qt5::Core Qt5::Widgets)
