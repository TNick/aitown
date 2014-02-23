# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================

# ----------------------------------------------------------------------------
# options

option(AITOWN_DEJAVU_INPUT_IS_VARIABLE "Input image size (w,h) may be changed after initialization" ON)
set(AITOWN_DEJAVU_FIX_WIDTH "0" CACHE INTEGER "Input image fixed width (set to 0 for variable)")
set(AITOWN_DEJAVU_FIX_HEIGHT "0" CACHE INTEGER "Input image fixed height (set to 0 for variable)")
option(AITOWN_DEJAVU_GRID_IS_SQUARE "Attention rectangle is a square (ON) or a rectangle (OFF)" ON)
set(AITOWN_DEJAVU_CHANGE_ROWS "64" CACHE INTEGER "Number of rows in the sudden changes area")
set(AITOWN_DEJAVU_CHANGE_COLS "64" CACHE INTEGER "Number of columns in the sudden changes area")

# ============================================================================

# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "aitown-dejavu" )
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
    "${CMAKE_CURRENT_SOURCE_DIR}/aitown-dejavu-config.h.in"
    "${PROJECT_BINARY_DIR}/../build/include/aitown/aitown-dejavu-config.h"
)
# ============================================================================


# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    aitown-dejavu.c
    dejavu-change.c
    dejavu-ar.c
)
set ( LIB_HEADERS
    aitown-dejavu.h
    dejavu-change.h
    dejavu-ar.h
)
set (LIBS)


add_library( ${LOC_PRJ_NAME} SHARED
    ${LIB_SOURCES}
)

target_link_libraries( ${LOC_PRJ_NAME}
    "aitown-core"
    "aitown-utils"
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

