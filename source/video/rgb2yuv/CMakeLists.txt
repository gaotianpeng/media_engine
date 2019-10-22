cmake_minimum_required(VERSION 2.8.11)

set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(FFmpeg REQUIRED
	COMPONENTS avcodec avfilter avdevice avutil swscale avformat swresample)

include_directories(${FFMPEG_INCLUDE_DIRS})
file(GLOB RGB_YUV_SRC *.h *.cc *.cpp)

find_package(OpenCV REQUIRED)

set(TARGET_SRC ${RGB_YUV_SRC})
include_directories(${OpenCV_INCLUDE_DIRS})

set(LIB_INTERNAL_DEPS ${FFMPEG_LIBRARIES}  ${OpenCV_LIBS})

me_add_executable(rgb2yuv)