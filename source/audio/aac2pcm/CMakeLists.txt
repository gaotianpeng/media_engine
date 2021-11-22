set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(FFmpeg REQUIRED
	COMPONENTS avcodec avfilter avdevice avutil swscale avformat swresample)

include_directories(${FFMPEG_INCLUDE_DIRS})
file(GLOB AAC2PCM_SRC *.h *.cc *.cpp)

set(TARGET_SRC ${AAC2PCM_SRC})

set(LIB_INTERNAL_DEPS ${FFMPEG_LIBRARIES})

me_add_executable(aac2pcm)