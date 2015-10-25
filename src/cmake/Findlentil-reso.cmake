# Name  : Findlentil-rend.cmake
# Author: Cerek Hillen
#
# Description:
#   Finding the resource loading section of lentil.

# Finding the include directory.
find_path(LENTIL-RESO_INCLUDE_DIR lentil/reso.h
  PATHS /usr/local/include
)

# Finding the library.
find_library(LENTIL-RESO_LIBRARY
  NAMES lentil-reso
  PATHS /usr/local
  PATH_SUFFIXES lib/static
                lib
)

# Finding its dependencies.
find_package(lentil-core REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PNG REQUIRED)

set(LENTIL-RESO_INCLUDE_DIRS ${LENTIL-CORE_INCLUDE_DIRS}
                             ${LENTIL-RESO_INCLUDE_DIR}
                             ${OPENGL_INCLUDE_DIRS}
                             ${GLEW_INCLUDE_DIRS}
                             ${PNG_INCLUDE_DIRS})

set(LENTIL-RESO_LIBRARIES ${LENTIL-CORE_LIBRARIES}
                          ${LENTIL-RESO_LIBRARY}
                          ${OPENGL_LIBRARIES}
                          ${GLEW_LIBRARIES}
                          ${PNG_LIBRARIES})

# Making sure we have everything. 
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LENTIL-RESO REQUIRED_VARS LENTIL-RESO_INCLUDE_DIR
                                                            LENTIL-RESO_LIBRARY)
