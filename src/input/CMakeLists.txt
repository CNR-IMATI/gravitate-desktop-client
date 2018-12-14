target_sources(${LIBNAME}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/houghfeature.h"
        "${CMAKE_CURRENT_LIST_DIR}/boundingbox.h"
        "${CMAKE_CURRENT_LIST_DIR}/facet.h"
        "${CMAKE_CURRENT_LIST_DIR}/meshpropertyreader.h"
        "${CMAKE_CURRENT_LIST_DIR}/scalarfieldproperty.h"
        "${CMAKE_CURRENT_LIST_DIR}/scalarfieldreader.h"
        "${CMAKE_CURRENT_LIST_DIR}/reassemblyreader.h"
        "${CMAKE_CURRENT_LIST_DIR}/partannotation.h"
        "${CMAKE_CURRENT_LIST_DIR}/threedselector.h"
        "${CMAKE_CURRENT_LIST_DIR}/webannotation.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/facet.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/meshpropertyreader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/scalarfieldproperty.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/scalarfieldreader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/reassemblyreader.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/threedselector.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/webannotation.cpp"
)

target_include_directories(${LIBNAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${LIBNAME} PUBLIC nlohmann-json)
target_link_libraries(${LIBNAME} PRIVATE Qt5::Core )
