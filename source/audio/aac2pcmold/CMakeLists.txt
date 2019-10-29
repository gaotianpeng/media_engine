cmake_minimum_required(VERSION 2.8.11)

set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(FFmpeg REQUIRED
	COMPONENTS avcodec avfilter avdevice avutil swscale avformat swresample)

include_directories(${FFMPEG_INCLUDE_DIRS})
file(GLOB AAC2PCMOLD_SRC *.h *.cc *.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS}) # Not needed for CMake >= 2.8.11
set(LIB_INTERNAL_DEPS ${OpenCV_LIBS})

set(TARGET_SRC ${AAC2PCMOLD_SRC})

set(LIB_INTERNAL_DEPS ${FFMPEG_LIBRARIES} ${OpenCV_LIBS})

me_add_executable(aac2pcmold)