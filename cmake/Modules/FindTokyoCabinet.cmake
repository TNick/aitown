# - Try to find argtable library
# Once done this will define
#  TOKYOCABINET_FOUND - System has TokyoCabinet
#  TOKYOCABINET_INCLUDE_DIRS - The TokyoCabinet include directories
#  TOKYOCABINET_LIBRARIES - The libraries needed to use TokyoCabinet
#  TOKYOCABINET_DEFINITIONS - Compiler switches required for using TokyoCabinet

find_package(PkgConfig)
pkg_check_modules(PC_TOKYOCABINET QUIET tokyocabinet)
set(TOKYOCABINET_DEFINITIONS ${PC_TOKYOCABINET_CFLAGS_OTHER})

find_path(TOKYOCABINET_INCLUDE_DIR tchdb.h
          HINTS ${PC_TOKYOCABINET_INCLUDEDIR} ${PC_TOKYOCABINET_INCLUDE_DIRS} 
          PATHS ENV TOKYOCABINET_INC
          )

find_library(TOKYOCABINET_LIBRARY NAMES tokyocabinet libtokyocabinet
             HINTS ${PC_TOKYOCABINET_LIBDIR} ${PC_TOKYOCABINET_LIBRARY_DIRS} 
             PATHS ENV TOKYOCABINET_LIBRARY )

set(TOKYOCABINET_LIBRARIES ${TOKYOCABINET_LIBRARY} )
set(TOKYOCABINET_INCLUDE_DIRS ${TOKYOCABINET_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set TOKYOCABINET_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(TokyoCabinet  DEFAULT_MSG
                                  TOKYOCABINET_LIBRARY TOKYOCABINET_INCLUDE_DIR)

mark_as_advanced(TOKYOCABINET_INCLUDE_DIR TOKYOCABINET_LIBRARY )

