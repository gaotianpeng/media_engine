cmake_minimum_required(VERSION 2.8.11)

project(av_mux)

find_package(FFmpeg REQUIRED
	COMPONENTS avcodec avfilter avdevice avutil swscale avformat swresample)

include_directories(${FFMPEG_INCLUDE_DIRS})

add_executable(av_mux av_mux.cpp)
target_link_libraries(av_mux ${FFMPEG_LIBRARIES})