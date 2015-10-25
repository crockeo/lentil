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
  PATH_SUFFIXES lib64
                lib
                a
)

# Finding its dependencies.
find_package(lentil-core REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PNG REQUIRED)

set(LENTIL-REND_INCLUDE_DIRS ${LENTIL-CORE_INCLUDE_DIR}
                             ${LENTIL-REND_INCLUDE_DIR}
                             ${OPENGL_INCLUDE_DIRS}
                             ${GLEW_INCLUDE_DIRS}
                             ${PNG_INCLUDE_DIRS})

set(LENTIL-REND_LIBRARIES ${LENTIL-CORE_LIBRARIES}
                          ${LENTIL-REND_LIBRARY}
                          ${OPENGL_LIBRARIES}
                          ${GLEW_LIBRARIES}
                          ${PNG_LIBRARIES})

# Making sure we have everything. 
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW REQUIRED_VARS LENTIL-RESO_INCLUDE_DIRS
                                                     LENTIL-RESO_LIBRARIES)
