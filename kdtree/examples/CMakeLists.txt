# ----------------------------------------------------------------------------
find_package(PkgConfig)
pkg_check_modules(PC_M QUIET m2)
set(M_DEFINITIONS ${PC_M_CFLAGS_OTHER})

find_library(M_LIBRARY NAMES m libm
             HINTS ${PC_M_LIBDIR} ${PC_M_LIBRARY_DIRS}
             PATHS ENV M_BINARY )

## TODO: Exclude from all

add_executable( kdtree_test_1
    test.c
)
target_link_libraries( kdtree_test_1
    kdtree
)

add_executable( kdtree_test_2
    test2.c
)
target_link_libraries( kdtree_test_2
    kdtree
    ${M_LIBRARY}
)


# ============================================================================

