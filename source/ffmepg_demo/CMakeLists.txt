set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/third/gtest CACHE PATH "Path to googletest")
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(FFmpeg REQUIRED
	COMPONENTS avcodec avfilter avdevice avutil swscale avformat swresample)

include_directories(${FFMPEG_INCLUDE_DIRS})
file(GLOB FFMEPG_TEST_SRC *.h *.cc)

set(TARGET_SRC ${FFMEPG_TEST_SRC})

set(LIB_INTERNAL_DEPS ${FFMPEG_LIBRARIES})


me_add_executable(ffmpeg_demo)

target_link_libraries(ffmpeg_demo
    GTest::GTest
    GTest::Main
    ${GTEST_BOTH_LIBRARIES}
)
