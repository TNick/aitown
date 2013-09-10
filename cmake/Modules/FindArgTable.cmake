# - Try to find argtable library
# Once done this will define
#  ARGTABLE_FOUND - System has ArgTable
#  ARGTABLE_INCLUDE_DIRS - The ArgTable include directories
#  ARGTABLE_LIBRARIES - The libraries needed to use ArgTable
#  ARGTABLE_DEFINITIONS - Compiler switches required for using ArgTable

find_package(PkgConfig)
pkg_check_modules(PC_ARGTABLE QUIET argtable2)
set(ARGTABLE_DEFINITIONS ${PC_ARGTABLE_CFLAGS_OTHER})

find_path(ARGTABLE_INCLUDE_DIR argtable2.h
          HINTS ${PC_ARGTABLE_INCLUDEDIR} ${PC_ARGTABLE_INCLUDE_DIRS} 
          PATHS ENV ARGTABLE_INC
          PATH_SUFFIXES argtable )

find_library(ARGTABLE_LIBRARY NAMES argtable2 libargtable2
             HINTS ${PC_ARGTABLE_LIBDIR} ${PC_ARGTABLE_LIBRARY_DIRS} 
             PATHS ENV ARGTABLE_BINARY )

set(ARGTABLE_LIBRARIES ${ARGTABLE_LIBRARY} )
set(ARGTABLE_INCLUDE_DIRS ${ARGTABLE_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ARGTABLE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(ArgTable  DEFAULT_MSG
                                  ARGTABLE_LIBRARY ARGTABLE_INCLUDE_DIR)

mark_as_advanced(ARGTABLE_INCLUDE_DIR ARGTABLE_LIBRARY )

