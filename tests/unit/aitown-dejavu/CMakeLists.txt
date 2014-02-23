set ( modules_testing_src ${modules_testing_src}
    "${CMAKE_CURRENT_SOURCE_DIR}/aitown_dejavu_image_test.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/aitown_dejavu_test.cc"

    CACHE INTERNAL "" 
)
list( REMOVE_DUPLICATES modules_testing_src )

