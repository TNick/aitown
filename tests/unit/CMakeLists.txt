set ( test_modules
    "aitown-utils"
    "aitown-core"
    "aitown-plugin"
    #"aitown-client"
    #"aitown-server"
    #"aitown-index"
)

FOREACH ( mod ${test_modules} )
    add_subdirectory( "${mod}" )
ENDFOREACH(mod)
