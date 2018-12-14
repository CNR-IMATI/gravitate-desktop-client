cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set (DEPLOY_FILES
  deploy-linux.sh
  deploy-windows.cmd
)

set(BINARY_FILES
    gravitate-icon.png
    libs/libeay32.dll
    libs/ssleay32.dll
)

add_custom_target(deployment-files SOURCES ${DEPLOY_FILES})

get_filename_component(QTBINDIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
get_filename_component(QTDIR "${QTBINDIR}/.." ABSOLUTE)

file(TO_NATIVE_PATH "${QTDIR}" QTDIR)
file(TO_NATIVE_PATH "${VTK_DIR}" VTK_DIR)
file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}" BUILD_DIR)

if(UNIX AND NOT APPLE)
    set(LINUX true)
endif()

if(LINUX)
    configure_file(deploy-linux.sh "${CMAKE_BINARY_DIR}/deployment/deploy-linux.sh" @ONLY)
endif(LINUX)
if(MSVC)
    # Get VCVARSALL FROM
    get_filename_component(MSVC_COMPILER_DIR
      ${CMAKE_CXX_COMPILER} DIRECTORY)
    find_file(VCVARSALL_BAT
       vcvarsall.bat
       PATHS
       "${MY_COMPILER_DIR}/.."
       "${MY_COMPILER_DIR}/../.."
       "${MY_COMPILER_DIR}/../../.."
       "${MY_COMPILER_DIR}/../../../.."
       "${MY_COMPILER_DIR}/../../../../../.."
       "${MY_COMPILER_DIR}/../../../../../../.."
       "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC"

       PATH_SUFFIXES
       "/Auxiliary/Build"
    )

    message(STATUS ${VCVARSALL_BAT})

    file(TO_NATIVE_PATH "${VCVARSALL_BAT}" VCVARSALL_BAT)

    configure_file(deploy-windows.cmd "${CMAKE_BINARY_DIR}/deployment/deploy-windows.cmd" @ONLY)
endif(MSVC)

## Binary resources


foreach(file ${BINARY_FILES})
    file(COPY ${file} DESTINATION "${CMAKE_BINARY_DIR}/deployment/")
endforeach(file)
