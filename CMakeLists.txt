#########
# Setup #
cmake_minimum_required(VERSION 2.6)
project(lentil)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH    "${PROJECT_SOURCE_DIR}/build/")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

######################
# External Libraries #
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(PNG REQUIRED)
find_package(glm REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
                    ${GLFW_INCLUDE_DIRS}
                    ${PNG_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIRS})

######################
# Internal Libraries #

# Core functionality.
set(CORE_SOURCES src/lentil/core/errors.c
                 src/lentil/core/debug.c
                 src/lentil/core/sleep.c)

add_library(lentil-core SHARED ${CORE_SOURCES})

# Math utilities.
set(MATH_SOURCES src/lentil/math/angles.c)

add_library(lentil-math SHARED ${MATH_SOURCES})

# Graphical rendering.
set(REND_SOURCES src/lentil/rend/modelutils.c
                 src/lentil/rend/buffering.c
                 src/lentil/rend/camera.c)

add_library(lentil-rend SHARED ${REND_SOURCES})

target_link_libraries(lentil-rend lentil-core
                                  ${OPENGL_LIBRARIES}
                                  ${GLEW_LIBRARIES}
                                  ${PNG_LIBRARIES})

# Resource management.
set(RESO_SOURCES src/lentil/reso/objloader.c
                 src/lentil/reso/texture.c
                 src/lentil/reso/shader.c
                 src/lentil/reso/model.c
                 src/lentil/reso/file.c)

add_library(lentil-reso SHARED ${RESO_SOURCES})
target_link_libraries(lentil-reso lentil-core
                                  ${OPENGL_LIBRARIES}
                                  ${GLEW_LIBRARIES}
                                  ${PNG_LIBRARIES})

################
# Installation #

install(TARGETS lentil-core
                lentil-rend
                lentil-reso
        LIBRARY DESTINATION lib)

install(DIRECTORY src/lentil/
        DESTINATION include/lentil
        FILES_MATCHING PATTERN *.h)

install(FILES src/cmake/Findlentil-core.cmake
              src/cmake/Findlentil-rend.cmake
              src/cmake/Findlentil-reso.cmake
        DESTINATION "${CMAKE_ROOT}/Modules")
