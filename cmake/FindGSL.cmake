# Name  : FindGSL.cmake
# Author: Cerek Hillen
#
# Description:
#   Attempting to find the GSL Science Library and registering it with CMake.

SET(GSL_SEARCH_PATHS /usr/local)

FIND_PATH(GSL_INCLUDE_DIR gsl
    HINTS
    PATH_SUFFIXES include/gsl include
    PATHS ${GSL_SEARCH_PATHS}
)

FIND_LIBRARY(GSL_LIBRARY
    NAMES gsl
    HINTS
    PATH_SUFFIXES lib64 lib
    PATHS ${GSL_SEARCH_PATHS}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GSL
                                  REQUIRED_VARS
                                  GSL_INCLUDE_DIR
                                  GSL_LIBRARY)
