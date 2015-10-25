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
  PATH_SUFFIXES lib64
                lib
                a
)

# Finding its dependencies.

# Making sure we have everything. 
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW REQUIRED_VARS LENTIL-CORE_INCLUDE_DIRS
                                                     LENTIL-CORE_LIBRARIES)
