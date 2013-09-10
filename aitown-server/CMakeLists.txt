# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================



# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "aitown-server" )
project( ${LOC_PRJ_NAME} )
# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    main.c
    aiserver_data.c
)
set ( LIB_HEADERS
    
)
if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_SERVER_DEBUG=1 )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_SERVER_NOT_DEBUG=1 )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

add_executable( ${LOC_PRJ_NAME}
    ${LIB_SOURCES}
)
target_link_libraries( ${LOC_PRJ_NAME}
	"inih"
	"cfgpath"
	${PROTOBUF_LIBRARY}
	${ARGTABLE_LIBRARIES}
	${ZEROMQ_LIBRARIES}

	"aitown-core"
	"aitown-utils"
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

