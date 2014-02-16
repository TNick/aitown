# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================



# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "aitown-db" )
project( ${LOC_PRJ_NAME} )

add_definitions( -DAITOWN_SHARED=1 )
if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_DB_DEBUG=1 )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_DB_NOT_DEBUG=1 )
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    aitown-db.c
    aitown-db-mng.c
    aitown-db-driver.c
)
set ( LIB_HEADERS
    aitown-db.h
    aitown-db-mng.h
    aitown-db-driver.h
)
set (LIBS)


if (${KYOTOCABINET_FOUND})
    set ( LIB_SOURCES ${LIB_SOURCES}
        buildin/kyoto.c
    )
    set ( LIBS ${LIBS}
        ${KYOTOCABINET_LIBRARIES}
    )
endif (${KYOTOCABINET_FOUND})

if (${TOKYOCABINET_FOUND})
    set ( LIB_SOURCES ${LIB_SOURCES}
        buildin/tokyo.c
    )
    set ( LIBS ${LIBS}
        ${TOKYOCABINET_LIBRARIES}
    )
endif (${TOKYOCABINET_FOUND})

if (${MYSQL_FOUND})
    set ( LIB_SOURCES ${LIB_SOURCES}
        buildin/mysql.c
    )
    set ( LIBS ${LIBS}
        ${MYSQL_LIBRARIES}
    )
endif (${MYSQL_FOUND})




add_library( ${LOC_PRJ_NAME} SHARED
    ${LIB_SOURCES}
)

target_link_libraries( ${LOC_PRJ_NAME}
    "aitown-core"
    "aitown-utils"
    "inih"
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

