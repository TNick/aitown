# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================



# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "aitown-core" )
project( ${LOC_PRJ_NAME} )

add_definitions( -DAITOWN_SHARED=1 )
if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_CORE_DEBUG=1 )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DAITOWN_CORE_NOT_DEBUG=1 )
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
if   (UNIX)

endif(UNIX)
# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================



# ----------------------------------------------------------------------------
# modules
set ( modules 
    "protobuf"
)

# generated libraries are accumulated in modules_libs
FOREACH ( mod ${modules} )
    add_subdirectory( "${mod}" )
ENDFOREACH(mod)
# ============================================================================



# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    dummy.cc
)
set ( LIB_HEADERS
    aitown_global.h
)

add_library( ${LOC_PRJ_NAME} SHARED
    ${LIB_SOURCES}
)
target_link_libraries( ${LOC_PRJ_NAME} "aitown-protobuf" ${PROTOBUFC_LIBRARIES} )

if (INCLUDE_OUTPUT_PATH)
    FOREACH (hfile ${LIB_HEADERS})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hfile}" "${INCLUDE_OUTPUT_PATH}/${hfile}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ENDFOREACH(hfile)
endif (INCLUDE_OUTPUT_PATH)
# ============================================================================

