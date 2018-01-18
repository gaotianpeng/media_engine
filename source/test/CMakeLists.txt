cmake_minimum_required(VERSION 3.3)


set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

set(GTEST_SRC sample.cpp test_main.cpp)
set(GTEST_LIB gmockd.lib gmock_maind.lib gtestd.lib gtest_maind.lib)
set(LOG_LIB easyloggingpp.lib )
set(DEPS_LIB me.lib)

set(TARGET_SRC ${GTEST_SRC})

SET(LIB_INTERNAL_DEPS
	${GTEST_LIB}
	${LOG_LIB}
	${DEPS_LIB}
	utility.lib
)

me_add_executable(unittest)

