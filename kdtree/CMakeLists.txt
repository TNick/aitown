# ----------------------------------------------------------------------------
# install

set ( LIB_HEADERS
    "kdtree.h"
    "aitown-kdtree.h"
)

set ( LIB_SOURCES
    "kdtree.c"
)

if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DKDTREE_DEBUG=1 )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DKDTREE_NO_DEBUG=1 )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")


add_library ( kdtree SHARED
    ${LIB_SOURCES}
)

INSTALL(FILES
    ${LIB_HEADERS}
    DESTINATION "include/${PROJECT_NAME}"
)

if (INCLUDE_OUTPUT_PATH)
    FOREACH (hfile ${LIB_HEADERS})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hfile}" "${INCLUDE_OUTPUT_PATH}/${hfile}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ENDFOREACH(hfile)
endif (INCLUDE_OUTPUT_PATH)

add_subdirectory( "examples" )

# ============================================================================

