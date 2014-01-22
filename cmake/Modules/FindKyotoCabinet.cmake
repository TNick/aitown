# - Try to find argtable library
# Once done this will define
#  KYOTOCABINET_FOUND - System has KyotoCabinet
#  KYOTOCABINET_INCLUDE_DIRS - The KyotoCabinet include directories
#  KYOTOCABINET_LIBRARIES - The libraries needed to use KyotoCabinet
#  KYOTOCABINET_DEFINITIONS - Compiler switches required for using KyotoCabinet

find_package(PkgConfig)
pkg_check_modules(PC_KYOTOCABINET QUIET kyotocabinet)
set(KYOTOCABINET_DEFINITIONS ${PC_KYOTOCABINET_CFLAGS_OTHER})

find_path(KYOTOCABINET_INCLUDE_DIR kclangc.h
          HINTS ${PC_KYOTOCABINET_INCLUDEDIR} ${PC_KYOTOCABINET_INCLUDE_DIRS} 
          PATHS ENV KYOTOCABINET_INC
          )

find_library(KYOTOCABINET_LIBRARY NAMES kyotocabinet libkyotocabinet
             HINTS ${PC_KYOTOCABINET_LIBDIR} ${PC_KYOTOCABINET_LIBRARY_DIRS} 
             PATHS ENV KYOTOCABINET_LIBRARY )

set(KYOTOCABINET_LIBRARIES ${KYOTOCABINET_LIBRARY} )
set(KYOTOCABINET_INCLUDE_DIRS ${KYOTOCABINET_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set KYOTOCABINET_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(KyotoCabinet  DEFAULT_MSG
                                  KYOTOCABINET_LIBRARY KYOTOCABINET_INCLUDE_DIR)

mark_as_advanced(KYOTOCABINET_INCLUDE_DIR KYOTOCABINET_LIBRARY )

