set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_SOUCE_DIR}/source/include)

file(GLOB LOGTEST_SRC *.h *.cc )

set(TARGET_SRC ${LOGTEST_SRC})

me_add_executable(logtest)