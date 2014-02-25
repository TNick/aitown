# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================

# ----------------------------------------------------------------------------
# options

# ============================================================================

# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "aitown-cfg" )
project( ${LOC_PRJ_NAME} )

add_definitions( -DAITOWN_SHARED=1 )
if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_DEJAVU_DEBUG=1 )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_DEJAVU_NOT_DEBUG=1 )
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# ============================================================================


# ----------------------------------------------------------------------------
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/aitown-cfg-config.h.in"
    "${PROJECT_BINARY_DIR}/../build/include/aitown/aitown-cfg-config.h"
)
# ============================================================================


# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    aitown-cfg.c
)
set ( LIB_HEADERS
    aitown-cfg.h
)
set (LIBS)


add_library( ${LOC_PRJ_NAME} SHARED
    ${LIB_SOURCES}
)

target_link_libraries( ${LOC_PRJ_NAME}
    ${LIBS}
)

if (INCLUDE_OUTPUT_PATH)
    FOREACH (hfile ${LIB_HEADERS})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hfile}" "${INCLUDE_OUTPUT_PATH}/${hfile}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ENDFOREACH(hfile)
endif (INCLUDE_OUTPUT_PATH)
# ============================================================================

