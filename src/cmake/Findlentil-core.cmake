# Name  : Findlentil-core.cmake
# Author: Cerek Hillen
#
# Description:
#   Finding the core section of lentil.

# Finding the include directory.
find_path(LENTIL-CORE_INCLUDE_DIRS lentil/core.h
  PATHS /usr/local/include
)

# Finding the library.
find_library(LENTIL-CORE_LIBRARIES
  NAMES lentil-core
  PATHS /usr/local
  PATH_SUFFIXES lib/static
                lib
)

# Making sure we have everything. 
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(lentil-core REQUIRED_VARS LENTIL-CORE_LIBRARIES
                                                            LENTIL-CORE_INCLUDE_DIRS)
