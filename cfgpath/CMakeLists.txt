# ----------------------------------------------------------------------------
# install

set ( LIB_HEADERS
    "cfgpath.h"
)

set ( LIB_SOURCES
    "cfgpath.c"
)

add_library ( cfgpath 
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

# ============================================================================

