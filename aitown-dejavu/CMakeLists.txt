# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================

# ----------------------------------------------------------------------------
# options

option(AITOWN_DEJAVU_INPUT_IS_VARIABLE "Input image size (w,h) may be changed after initialization" ON)
option(AITOWN_DEJAVU_FIX_WIDTH "Input image fixed width (set to 0 for variable)" 0)
option(AITOWN_DEJAVU_FIX_HEIGHT "Input image fixed height (set to 0 for variable)" 0)
option(AITOWN_DEJAVU_AR_SQUARE "Attention rectangle is a square or a rectangle" ON)

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
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    aitown-dejavu.c
)
set ( LIB_HEADERS
    aitown-dejavu.h
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

