#########
# Setup #
cmake_minimum_required(VERSION 2.6)
project(lentil)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH    "${PROJECT_SOURCE_DIR}/build/")

add_definitions("-Wall")
add_definitions("-std=c11")

######################
# External Libraries #
find_package(opengl REQUIRED)
find_package(glew REQUIRED)
find_package(glfw REQUIRED)
find_package(gsl REQUIRED)
find_package(png REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
                    ${GLFW_INCLUDE_DIRS}
                    ${GSL_INCLUDE_DIRS}
                    ${PNG_INCLUDE_DIRS})

######################
# Internal Libraries #

# Resource management.
set(RESO_SOURCES src/reso/objloader.c
                 src/reso/texture.c
                 src/reso/shader.c
                 src/reso/model.c
                 src/reso/file.c)

add_library(lentil-reso ${RESO_SOURCES})

# Core functionality.
set(CORE_SOURCES src/core/errors.c
                 src/core/debug.c)

add_library(lentil-core ${CORE_SOURCES})

##############
# Executable #
set(EXEC_SOURCES src/main.c)

add_executable(lentil ${EXEC_SOURCES})

# Linking external libraries.
target_link_libraries(lentil ${OPENGL_LIBRARY}
                             ${GLEW_LIBRARY}
                             ${GLFW_LIBRARY}
                             ${GSL_LIBRARY}
                             ${PNG_LIBRARY})

# Linking local libraries.
target_link_libraries(lentil lentil-reso
                             lentil-core)
