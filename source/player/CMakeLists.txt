cmake_minimum_required(VERSION 3.3 FATAL_ERROR)


set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB SOURCE_FILES_H "*.h")
file(GLOB SOURCE_FILES "*.cpp")


set(TARGET_SRC 
    ${SOURCE_FILES}
    ${SOURCE_FILES_H} 
)


set(SRC ${TARGET_SRC})
set(FFMPEG_LIB avcodec.lib avdevice.lib avfilter.lib avformat.lib avutil.lib swresample.lib swscale.lib)
set(SDL2_LIB SDL2d.lib SDL2maind.lib) 

set(TARGET_SRC ${SRC})

SET(LIB_INTERNAL_DEPS
	${FFMPEG_LIB}
	${SDL2_LIB}
)

me_add_executable(player)