#########
# Setup #
cmake_minimum_required(VERSION 2.6)
project(lentil)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH    "${PROJECT_SOURCE_DIR}/build/")

add_definitions(-std=c11)

######################
# External Libraries #
find_package(glew REQUIRED)
find_package(glfw REQUIRED)
find_package(png REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS}
                    ${GLFW_INCLUDE_DIRS}
                    ${PNG_INCLUDE_DIRS})

######################
# Internal Libraries #

# Resource management.
set(RESO_SOURCES src/res/texture.c
                 src/res/shader.c
                 src/res/model.c)

add_library(lentil-reso ${RESO_SOURCES})

# Core functionality.
set(CORE_SOURCES src/errors.c)

add_library(lentil-core ${CORE_SOURCES})

##############
# Executable #
set(EXEC_SOURCES src/main.c)

add_executable(lentil ${EXEC_SOURCES})

target_link_libraries(lentil ${GLEW_LIBRARY}
                             ${GLFW_LIBRARY}
                             ${PNG_LIBRARY})

target_link_libraries(lentil lentil-reso
                             lentil-core)
