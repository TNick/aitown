# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================



# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "sample-plugin" )
project( ${LOC_PRJ_NAME} )
# ============================================================================



# ----------------------------------------------------------------------------
# external libraries

# ============================================================================


# ----------------------------------------------------------------------------
# our library

set ( AITOWN_PLUGIN_UNAME "Gorgeous plug-in" )
set ( AITOWN_PLUGIN_DESCR "Example plug-in that does nothing, really" )
set ( AITOWN_PLUGIN_V_MAJOR 1 )
set ( AITOWN_PLUGIN_V_MINOR 0 )
set ( AITOWN_PLUGIN_V_PATCH 0 )
set ( AITOWN_PLUGIN_DEPENDS 
#    "badabum         :1.0.0"
#    "big-badabum     :2.0"
)
set ( AITOWN_PLUGIN_FOR_SERVER ON )
set ( AITOWN_PLUGIN_FOR_CLIENT ON )

# ---------------------------------------------------------------------------------

set ( AITOWN_PLUGIN_SIG_FILE 
    "${CMAKE_BINARY_DIR}/build/bin/plugins/${LOC_PRJ_NAME}.inf"
)
string ( REGEX REPLACE "ON" "yes" 
    AITOWN_PLUGIN_FOR_SERVER  
    ${AITOWN_PLUGIN_FOR_SERVER}
)
string ( REGEX REPLACE "OFF" "no" 
    AITOWN_PLUGIN_FOR_SERVER  
    ${AITOWN_PLUGIN_FOR_SERVER}
)
string ( REGEX REPLACE "ON" "yes" 
    AITOWN_PLUGIN_FOR_CLIENT  
    ${AITOWN_PLUGIN_FOR_CLIENT}
)
string ( REGEX REPLACE "OFF" "no" 
    AITOWN_PLUGIN_FOR_CLIENT  
    ${AITOWN_PLUGIN_FOR_CLIENT}
)
       
file(WRITE ${AITOWN_PLUGIN_SIG_FILE} 
    "[general]\n"
    "name = ${AITOWN_PLUGIN_UNAME}\n"
    "description = ${AITOWN_PLUGIN_DESCR}\n"
    "version = ${AITOWN_PLUGIN_V_MAJOR}.${AITOWN_PLUGIN_V_MINOR}.${AITOWN_PLUGIN_V_PATCH}\n"
    "manager = ${aitown_VERSION}\n"

) 
foreach (ONE_DEP ${AITOWN_PLUGIN_DEPENDS})
    file(APPEND ${AITOWN_PLUGIN_SIG_FILE} "dependencies = ${ONE_DEP}\n")
endforeach(ONE_DEP)

file(APPEND ${AITOWN_PLUGIN_SIG_FILE} 
    "\n"
    "[aitown]\n"
    "server = ${AITOWN_PLUGIN_FOR_SERVER}\n"
    "client = ${AITOWN_PLUGIN_FOR_CLIENT}\n"
)

# ---------------------------------------------------------------------------------


set ( LIB_SOURCES
    sample-plugin.c
)
set ( LIB_HEADERS
    sample-plugin.h
)

set ( CMAKE_DEBUG_POSTFIX  "")
set ( LIBRARY_OUTPUT_PATH  "${CMAKE_BINARY_DIR}/build/bin/plugins" )
set ( EXECUTABLE_OUTPUT_PATH  "${CMAKE_BINARY_DIR}/build/bin/plugins" )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/build/bin/plugins" CACHE
        PATH "Directory where .exe and .dll files are dumped." FORCE)

add_library( ${LOC_PRJ_NAME} SHARED
    ${LIB_SOURCES}
)
target_link_libraries( ${LOC_PRJ_NAME}
    "aitown-core"
    "aitown-utils"
    "aitown-plugin"
)
set_target_properties(${LOC_PRJ_NAME} PROPERTIES PREFIX "")

# ============================================================================
