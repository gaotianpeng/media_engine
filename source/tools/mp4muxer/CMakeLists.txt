set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(FFmpeg REQUIRED
	COMPONENTS avcodec avfilter avdevice avutil swscale avformat swresample)
find_package (glog 0.6.0 REQUIRED PATHS ${PROJECT_SOURCE_DIR}/third/glog)
include_directories(${GLOG_INCLUDE_DIRS})

include_directories(${FFMPEG_INCLUDE_DIRS})
file(GLOB MP4MUXER_SRC *.h *.cc *.cpp)

set(TARGET_SRC ${MP4MUXER_SRC})

set(LIB_INTERNAL_DEPS ${FFMPEG_LIBRARIES})

me_add_executable(mp4muxer)
target_link_libraries(mp4muxer
	glog::glog
)