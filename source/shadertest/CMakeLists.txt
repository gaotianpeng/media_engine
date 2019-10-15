cmake_minimum_required(VERSION 2.8.11)

set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")


include_directories(${FFMPEG_INCLUDE_DIRS})
file(GLOB RGB_YUV_SRC *.h *.cc *.cpp)

set(TARGET_SRC ${RGB_YUV_SRC})
MESSAGE( STATUS "ffmepg include dir is:  ${FFMPEG_INCLUDE_DIRS}.")
link_directories(${PROJECT_SOURCE_DIR}/dependency/win64/bin)
MESSAGE( STATUS "project source dir is:  ${FFMPEG_INCLUDE_DIRS}.")

set(SHARED_LIBRARIES opengl32.lib glew32.lib glu32.lib)
set(LIB_INTERNAL_DEPS ${SHADER_LIBRARIES})

me_add_executable(shadertest)