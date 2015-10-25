# Name  : Findlentil-rend.cmake
# Author: Cerek Hillen
#
# Description:
#   Finding the rendering section of lentil.

# Finding the include directory.
find_path(LENTIL-REND_INCLUDE_DIR lentil/rend.h
  PATHS /usr/local/include
)

# Finding the library.
find_library(LENTIL-REND_LIBRARY
  NAMES lentil-rend
  PATHS /usr/local
  PATH_SUFFIXES lib/static
                lib
)

# Finding its dependencies.
find_package(lentil-core REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(LENTIL-REND_INCLUDE_DIRS ${LENTIL-CORE_INCLUDE_DIRS}
                             ${LENTIL-REND_INCLUDE_DIR}
                             ${OPENGL_INCLUDE_DIRS}
                             ${GLEW_INCLUDE_DIRS})

set(LENTIL-REND_LIBRARIES ${LENTIL-CORE_LIBRARIES}
                          ${LENTIL-REND_LIBRARY}
                          ${OPENGL_LIBRARIES}
                          ${GLEW_LIBRARIES})

# Making sure we have everything. 
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LENTIL-REND REQUIRED_VARS LENTIL-REND_INCLUDE_DIR
                                                            LENTIL-REND_LIBRARY)
