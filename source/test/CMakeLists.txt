cmake_minimum_required(VERSION 3.3)

set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

find_package(gtest)

set(LOG_LIB easyloggingpp.lib )
set(DEPS_LIB me.lib)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${DepsPath}/include)
link_directories(${DepsPath}/bin)
file(GLOB GTEST_SRC *.h *.cc)

set(TARGET_SRC ${GTEST_SRC})

SET(LIB_INTERNAL_DEPS
	${GTEST_LIBRARIES}
	${LOG_LIB}
	${DEPS_LIB}
	utility.lib
)

me_add_executable(unittest)

