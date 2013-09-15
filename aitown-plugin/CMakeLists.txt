# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================



# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "aitown-plugin" )
project( ${LOC_PRJ_NAME} )
# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    plugin_data.c
    plugin_manager.c
    plugin_sign.c
    plugin_manager_inf.c
)
set ( LIB_HEADERS
    aitown-plugin.h
    plugin_data.h
    plugin_manager.h
    plugin_sign.h
    plugin_definition.h
)
if (WIN32)
    set ( LIB_HEADERS ${LIB_HEADERS} plugin_win.h )
else (WIN32)
    set ( LIB_HEADERS ${LIB_HEADERS} plugin_posix.h )
endif (WIN32)

set ( AITOWN_PLUGINS_LIBS )

if (WIN32)
  # set stuff for windows
  
else (WIN32)
    # under unix like hosts we need dl library
    set ( AITOWN_PLUGINS_LIBS ${AITOWN_PLUGINS_LIBS}
        "dl"
    )
endif (WIN32)

add_library( ${LOC_PRJ_NAME}
    ${LIB_SOURCES}
)
target_link_libraries( ${LOC_PRJ_NAME}
    "inih"
    ${AITOWN_PLUGINS_LIBS}
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

add_subdirectory(sample-plugin)
