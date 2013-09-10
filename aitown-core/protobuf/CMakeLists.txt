# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================



# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "aitown-protobuf" )
project( ${LOC_PRJ_NAME} )
# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    aitown_messages.pb-c.c
    protobuf_wrapper.c
)
set ( LIB_HEADERS
    aitown_messages.pb-c.h
    protobuf_wrapper.h
)

add_library( ${LOC_PRJ_NAME} STATIC
    ${LIB_SOURCES}
)
if   (CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET_TARGET_PROPERTIES( ${LOC_PRJ_NAME} PROPERTIES COMPILE_FLAGS -fPIC)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if (INCLUDE_OUTPUT_PATH)
    FOREACH (hfile ${LIB_HEADERS})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hfile}" "${INCLUDE_OUTPUT_PATH}/${hfile}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ENDFOREACH(hfile)
endif (INCLUDE_OUTPUT_PATH)
# ============================================================================

