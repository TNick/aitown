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
)

FOREACH ( mod ${test_modules} )
    add_subdirectory( "${mod}" )
ENDFOREACH(mod)
