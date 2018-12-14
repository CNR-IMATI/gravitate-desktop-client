include(DownloadProject)

# Modified from: https://github.com/dmrub/seord

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SERD_VERSION 0.28.0)
set(SORD_VERSION 0.16.0)

message(STATUS "SERD_VERSION: ${SERD_VERSION}")
message(STATUS "SORD_VERSION: ${SORD_VERSION}")

download_project(
    PROJ serd
    GIT_REPOSITORY "https://github.com/drobilla/serd.git"
    GIT_TAG "v${SERD_VERSION}"
    UPDATE_DISCONNECTED 1
#    QUIET
)

download_project(
    PROJ sord
    GIT_REPOSITORY "https://github.com/drobilla/sord.git"
    GIT_TAG "v${SORD_VERSION}"
    UPDATE_DISCONNECTED 1
#    QUIET
)

include(CheckFunctionExists)

check_function_exists(posix_memalign HAVE_POSIX_MEMALIGN)
check_function_exists(posix_fadvise HAVE_POSIX_FADVISE)
check_function_exists(fileno HAVE_FILENO)

configure_file("${CMAKE_CURRENT_LIST_DIR}/serd_config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/serd_config.h")
configure_file("${CMAKE_CURRENT_LIST_DIR}/sord_config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/sord_config.h")

set(SERD_INCLUDE_DIRS ${serd_SOURCE_DIR} PARENT_SCOPE)
set(SORD_INCLUDE_DIRS ${sord_SOURCE_DIR} PARENT_SCOPE)

add_library(serd-sord "")

target_sources(serd-sord
    PUBLIC
        "${serd_SOURCE_DIR}/serd/serd.h"
        "${sord_SOURCE_DIR}/sord/sord.h"
    PRIVATE
        "${serd_SOURCE_DIR}/src/byte_source.c"
        "${serd_SOURCE_DIR}/src/env.c"
        "${serd_SOURCE_DIR}/src/node.c"
        "${serd_SOURCE_DIR}/src/reader.c"
        "${serd_SOURCE_DIR}/src/serdi.c"
        "${serd_SOURCE_DIR}/src/string.c"
        "${serd_SOURCE_DIR}/src/writer.c"
        "${serd_SOURCE_DIR}/src/uri.c"

        "${sord_SOURCE_DIR}/src/zix/hash.c"
        "${sord_SOURCE_DIR}/src/zix/btree.c"
        "${sord_SOURCE_DIR}/src/zix/digest.c"
        "${sord_SOURCE_DIR}/src/syntax.c"
        "${sord_SOURCE_DIR}/src/sord.c"
)

target_include_directories(serd-sord
    PUBLIC
      "${serd_SOURCE_DIR}"
      "${sord_SOURCE_DIR}"
      "${sord_SOURCE_DIR}/src"
      "${CMAKE_CURRENT_BINARY_DIR}"
)
