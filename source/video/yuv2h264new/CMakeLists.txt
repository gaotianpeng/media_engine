cmake_minimum_required(VERSION 2.8.11)

set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(FFmpeg REQUIRED
	COMPONENTS avcodec avfilter avdevice avutil swscale avformat swresample)

include_directories(${FFMPEG_INCLUDE_DIRS})
file(GLOB YUV2H264NEW_SRC *.h *.cc *.cpp)

set(TARGET_SRC ${YUV2H264NEW_SRC})

set(LIB_INTERNAL_DEPS ${FFMPEG_LIBRARIES})

me_add_executable(yuv2h264new)