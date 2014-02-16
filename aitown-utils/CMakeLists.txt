# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_definitions( -DAITOWN_SHARED=1 )
if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_UTILS_DEBUG=1 )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_UTILS_NOT_DEBUG=1 )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
# ============================================================================



# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "aitown-utils" )
project( ${LOC_PRJ_NAME} )
# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    utils.c
    accumulator.c
    linked_list_str.c
    dir_iterator.c
    char_buff.c
    dir_utils.c
    pivotal_gmtime.c
)
set ( LIB_HEADERS
    accumulator.h
    dbg_assert.h
    error_codes.h
    pointer_aritmetic.h
    utils.h
    utils_null.h
    utils_unused.h
    linked_list_str.h
    dir_iterator.h
    char_buff.h
    dir_utils.h
    utils_version.h
    utils_offset.h
    stack_buff.h
<<<<<<< HEAD
    pivotal_gmtime.h
=======
    zmq_wrapper.h
>>>>>>> d4304003d7f9c1925cd3f8287269e504baaffd7c
)

add_library( ${LOC_PRJ_NAME} SHARED
    ${LIB_SOURCES}
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

