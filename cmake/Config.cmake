############################
# Build Configuration
############################

# Set the runtime output directory as
# the root binary dir for all the targets
# This is done to prevent path issues with binaries in subdirectories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

############################
# C++ Configuration
############################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

############################
# VTK Configuration
############################
find_package(VTK REQUIRED)

if(${VTK_VERSION} VERSION_EQUAL 8.1)
    message(FATAL_ERROR "VTK version 8.1 is not supported. Please use 7.1 or 8.0")
endif()

if(${VTK_VERSION} VERSION_LESS 7.1)
    message(FATAL_ERROR "Required VTK 7.1 or higher")
endif()


include(${VTK_USE_FILE})
message( STATUS "VTK_VERSION: " ${VTK_VERSION})
message( STATUS "VTK_QT_VERSION: " ${VTK_QT_VERSION})

############################
# Qt Configuration
############################
if(POLICY CMP0020)
  # Automatically link Qt executables to qtmain target on Windows.
  cmake_policy(SET CMP0020 NEW)
endif()

# set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
