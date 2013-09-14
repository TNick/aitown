set ( modules_testing_src ${modules_testing_src}
    "${CMAKE_CURRENT_SOURCE_DIR}/diriterator_test.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/char_buff_test.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/dirutils_test.cc"

    CACHE INTERNAL "" 
)
list( REMOVE_DUPLICATES modules_testing_src )

