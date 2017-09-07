cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)


set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

set(SRC coding_test.cpp)
set(FFMPEG_LIB avcodec.lib avdevice.lib avfilter.lib avformat.lib avutil.lib swresample.lib swscale.lib)

set(TARGET_SRC ${SRC})

SET(LIB_INTERNAL_DEPS
	${FFMPEG_LIB}
)

me_add_executable(coding_module)