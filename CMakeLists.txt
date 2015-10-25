#########
# Setup #
cmake_minimum_required(VERSION 2.6)
project(lentil)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH    "${PROJECT_SOURCE_DIR}/build/")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")

######################
# External Libraries #
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(PNG REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
                    ${GLFW_INCLUDE_DIRS}
                    ${PNG_INCLUDE_DIRS})

######################
# Internal Libraries #

# Core functionality.
set(CORE_SOURCES src/lentil/core/errors.c
                 src/lentil/core/debug.c
                 src/lentil/core/sleep.c)

add_library(lentil-core ${CORE_SOURCES})

# Graphical rendering.
set(REND_SOURCES src/lentil/rend/modelutils.c)

add_library(lentil-rend ${REND_SOURCES})

# Resource management.
set(RESO_SOURCES src/lentil/reso/objloader.c
                 src/lentil/reso/texture.c
                 src/lentil/reso/shader.c
                 src/lentil/reso/model.c
                 src/lentil/reso/file.c)

add_library(lentil-reso ${RESO_SOURCES})

##############
# Executable #
set(EXEC_SOURCES src/lentil/main.c)

add_executable(lentil ${EXEC_SOURCES})

# Linking external libraries.
target_link_libraries(lentil ${OPENGL_LIBRARY}
                             ${GLEW_LIBRARY}
                             ${GLFW_LIBRARY}
                             ${PNG_LIBRARY})

# Linking local libraries.
target_link_libraries(lentil lentil-core
                             lentil-rend
                             lentil-reso)
