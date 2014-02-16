# - Try to find argtable library
# Once done this will define
#  MYSQL_FOUND - System has MySQL
#  MYSQL_INCLUDE_DIRS - The MySQL include directories
#  MYSQL_LIBRARIES - The libraries needed to use MySQL
#  MYSQL_DEFINITIONS - Compiler switches required for using MySQL

find_package(PkgConfig)
pkg_check_modules(PC_MYSQL QUIET mysql)
set(MYSQL_DEFINITIONS ${PC_MYSQL_CFLAGS_OTHER})

find_path(MYSQL_INCLUDE_DIR mysql.h
          HINTS ${PC_MYSQL_INCLUDEDIR} ${PC_MYSQL_INCLUDE_DIRS} 
          PATHS ENV MYSQL_INC
          PATH_SUFFIXES mysql
          )

find_library(MYSQL_LIBRARY NAMES mysqlclient mysqlclient_r mysql libmysql
             HINTS ${PC_MYSQL_LIBDIR} ${PC_MYSQL_LIBRARY_DIRS} 
             PATHS ENV MYSQL_LIBRARY
             PATH_SUFFIXES mysql
             )

set(MYSQL_LIBRARIES ${MYSQL_LIBRARY} )
set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set MYSQL_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(MySQL  DEFAULT_MSG
                                  MYSQL_LIBRARY MYSQL_INCLUDE_DIR)

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARY )

