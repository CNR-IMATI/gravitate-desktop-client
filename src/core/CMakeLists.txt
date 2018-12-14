target_sources(${LIBNAME}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/enums.h"
        "${CMAKE_CURRENT_LIST_DIR}/qudt_units.h"
        "${CMAKE_CURRENT_LIST_DIR}/unitofmeasure.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/unitofmeasure.cpp"
)

target_include_directories(${LIBNAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
