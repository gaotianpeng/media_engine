
cmake_minimum_required(VERSION 3.3)

set(CMAKE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/dependency/bin)
if(DEVELOP_MODE)
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${LIBRARY_OUTPUT_PATH})
endif()
link_directories(${CMAKE_LIBRARY_PATH})


set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

set(GTEST_SRC sample.cpp test_main.cpp)
set(GTEST_LIB gmock.lib gmock_main.lib gtest.lib gtest_main.lib)
set(LOG_LIB easyloggingpp.lib )
set(DEPS_LIB me.lib)

set(TARGET_SRC ${GTEST_SRC})

SET(LIB_INTERNAL_DEPS
	${GTEST_LIB}
	${LOG_LIB}
	${DEPS_LIB}
)

me_add_executable(unittest)

