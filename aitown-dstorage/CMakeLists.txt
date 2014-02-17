












#
#   TODO
#
#   Refactor to use aitown-db for storage
#
#   Make sure the ref count is increased in controller


















# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================



# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "aitown-dstorage" )
project( ${LOC_PRJ_NAME} )

add_definitions( -DAITOWN_SHARED=1 )
if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_DSTORAGE_DEBUG=1 )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_DSTORAGE_NOT_DEBUG=1 )
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    aitown-dstorage.c
    dstorage_handle_mng.c
    dstorage_handle.c
    dstorage_lookup.c
    dstorage_ctrl.c
    dstorage_clist.c
    dstorage_controllers.c
    dstorage_chunk.c
    dstorage_chunk_mng.c
    dstorage_cdata.c
    controllers/dstorage_ctrl_local_db.c
    controllers/dstorage_ctrl_local_dir.c
)
set ( LIB_HEADERS
    aitown-dstorage.h
    dstorage_handle_mng.h
    dstorage_handle.h
    dstorage_lookup.h
    dstorage_id.h
    dstorage_func.h
    dstorage_ctrl.h
    dstorage_clist.h
    dstorage_controllers.h
    dstorage_chunk.h
    dstorage_chunk_mng.h
    dstorage_cdata.h
    controllers/dstorage_ctrl_local_db.h
    controllers/dstorage_ctrl_local_dir.h
)


add_library( ${LOC_PRJ_NAME} SHARED
    ${LIB_SOURCES}
)
target_link_libraries( ${LOC_PRJ_NAME}
    "aitown-core"
    "aitown-utils"
    "inih"
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

