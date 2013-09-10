# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================


# ----------------------------------------------------------------------------
# customisation

set ( PROJECT_NAME aitown )

# ============================================================================


# ----------------------------------------------------------------------------
# configure the project

project(${PROJECT_NAME})

# may be either Release or Debug
if ( CMAKE_BUILD_TYPE STREQUAL "" )
    set ( CMAKE_BUILD_TYPE "Release" )
endif (CMAKE_BUILD_TYPE STREQUAL "")

# the version as known to CMake
set ( ${PROJECT_NAME}_MAJOR_VERSION 1)
set ( ${PROJECT_NAME}_MINOR_VERSION 0)
set ( ${PROJECT_NAME}_PATCH_VERSION 0)
set ( ${PROJECT_NAME}_VERSION
  "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION}")

# set some paths
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/build/bin" )
set ( EXECUTABLE_OUTPUT_PATH  "${PROJECT_BINARY_DIR}/build/bin" )
set ( LIBRARY_OUTPUT_PATH  "${PROJECT_BINARY_DIR}/build/lib" )
set ( INCLUDE_OUTPUT_PATH  "${PROJECT_BINARY_DIR}/build/include/${PROJECT_NAME}" )

include_directories(
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_BINARY_DIR}/build/include"
)

IF (WIN32)
  # set stuff for windows
  add_definitions( -DAITOWN_WIN32=1 )
ELSE()
  # set stuff for other systems
  
ENDIF()

# configure compiler
if ( CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)" )
    add_definitions(/W4)
else ( CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)" )
    add_definitions(-Wall)
endif()
if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DQT_DEBUG=1 )
    set ( DebugLogEnabled true )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DQT_NO_DEBUG=1 )
    set ( DebugLogEnabled false )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
set ( CMAKE_DEBUG_POSTFIX  "_debug")

# Find includes in corresponding build directories
set ( CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set ( CMAKE_AUTOMOC ON)

# ============================================================================


# ----------------------------------------------------------------------------
# external libraries

#find_package(OGRE REQUIRED)
#include_directories(${OGRE_INCLUDE_DIRS})

find_package(ZeroMQ REQUIRED)
include_directories(${ZEROMQ_INCLUDE_DIRS})

find_package(ArgTable REQUIRED)
include_directories(${ARGTABLE_INCLUDE_DIRS})

find_package(ProtobufC REQUIRED)
include_directories(${PROTOBUFC_INCLUDE_DIR})
# ============================================================================


# ----------------------------------------------------------------------------
# modules
set ( modules 
    "sglib"
    "inih"
    "cfgpath"
	"aitown-utils"
	"aitown-core"
	"aitown-client"
	"aitown-server"
	"aitown-index"
)

# generated libraries are accumulated in modules_libs
FOREACH ( mod ${modules} )
    add_subdirectory( "${mod}" )
ENDFOREACH(mod)
# ============================================================================

