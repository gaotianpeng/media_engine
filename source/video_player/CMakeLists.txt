cmake_minimum_required(VERSION 2.8.11)

set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(FFmpeg REQUIRED
	COMPONENTS avcodec avfilter avdevice avutil swscale avformat swresample)
find_package(SDL)

include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(${SDL_INCLUDE_DIRS})

file(GLOB VIDEO_PLAYER_SRC *.h *.cc)

set(TARGET_SRC ${VIDEO_PLAYER_SRC})

set(LIB_INTERNAL_DEPS ${FFMPEG_LIBRARIES} ${SDL_LIBRARIES})

me_add_executable(video_player)