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

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_U)
string(TIMESTAMP ${PROJECT_NAME}_BUILD_TIME UTC)

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

if (WIN32)
  # set stuff for windows
  add_definitions( -DAITOWN_WIN32=1 )
else (WIN32)
  # set stuff for other systems
  
endif (WIN32)

# configure compiler
if ( CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)" )
    add_definitions(/W4)
else ( CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)" )
    add_definitions(-Wall)
endif()
if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DQT_DEBUG=1 -DAITOWN_DEBUG=1 -D_DEBUG=1 )
    set ( DebugLogEnabled true )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DQT_NO_DEBUG=1 -DAITOWN_NO_DEBUG=1 -D_NDEBUG=1 )
    set ( DebugLogEnabled false )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
set ( CMAKE_DEBUG_POSTFIX  "_debug")

# Find includes in corresponding build directories
set ( CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set ( CMAKE_AUTOMOC ON)

# accumulate testing source files
set (modules_testing_src)

# ============================================================================


# ----------------------------------------------------------------------------
# external libraries
# enable_language(Fortran REQUIRED)

find_package(OGRE)
include_directories(${OGRE_INCLUDE_DIRS})

find_package(ZeroMQ REQUIRED)
include_directories(${ZEROMQ_INCLUDE_DIRS})

find_package(ArgTable REQUIRED)
include_directories(${ARGTABLE_INCLUDE_DIRS})

find_package(ProtobufC REQUIRED)
include_directories(${PROTOBUFC_INCLUDE_DIR})

find_package(KyotoCabinet)
include_directories(${KYOTOCABINET_INCLUDE_DIR})

find_package(TokyoCabinet)
include_directories(${TOKYOCABINET_INCLUDE_DIR})

find_package(MySQL)
include_directories(${MYSQL_INCLUDE_DIR})


# -DBLA_VENDOR=ATLAS,Goto, etc; see FindBLASS.cmake
# sets BLAS_LIBRARIES; BLAS_INCLUDE_DIRS not set
find_package(BLAS REQUIRED)
#include_directories(${BLAS_INCLUDE_DIRS})
#message (${BLAS_LIBRARIES})

# -DCMAKE_BUILD_TYPE=Debug -DBLA_VENDOR=ATLAS

add_subdirectory( "googletest" )
include_directories(googletest/include)
# ============================================================================

add_definitions(-DUSE_KYOTO_CABINET=1)

# ----------------------------------------------------------------------------
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/build/include/aitown/config.h"
)
# ============================================================================

# ----------------------------------------------------------------------------
# modules
set ( lib_modules
    "aitown-utils"
    "aitown-core"
    "aitown-plugin"
    "aitown-dstorage"
    "aitown-image"
    "aitown-db"
)

set ( modules
    "sglib"
    "inih"
    "cfgpath"
    "kdtree"
    ${lib_modules}
    "aitown-index"
    "aitown-client"
    "aitown-server"
)

# generated libraries are accumulated in modules_libs
FOREACH ( mod ${modules} )
    add_subdirectory( "${mod}" )
ENDFOREACH(mod)
# ============================================================================

# ----------------------------------------------------------------------------
# internal tests
add_subdirectory( "tests" )
list( REMOVE_DUPLICATES modules_testing_src )
if (modules_testing_src)
    add_executable(internal-test
        ${modules_testing_src}
    )
    target_link_libraries ( internal-test 
        ${lib_modules}
        gtest_main gtest
        ${KYOTOCABINET_LIBRARIES}
        cfgpath
    )
    set ( props "AITOWN_INTERNAL_TESTS=1")
    set_target_properties ( internal-test
        PROPERTIES COMPILE_DEFINITIONS "${props}"
    )
endif (modules_testing_src)
# ============================================================================

