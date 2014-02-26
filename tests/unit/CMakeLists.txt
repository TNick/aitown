set ( test_modules
    "aitown-utils"
    "aitown-core"
    "aitown-plugin"
    #"aitown-client"
    #"aitown-server"
    #"aitown-index"
    "aitown-dstorage"
    "inih"
    "kdtree"
    "aitown-db"
    "aitown-dejavu"
    "aitown-cfg"
)

FOREACH ( mod ${test_modules} )
    add_subdirectory( "${mod}" )
ENDFOREACH(mod)

set ( modules_testing_src ${modules_testing_src}
    "${CMAKE_CURRENT_SOURCE_DIR}/getRealTime.c"
    CACHE INTERNAL "" 
)
list( REMOVE_DUPLICATES modules_testing_src )


