set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_SOUCE_DIR}/source/include)

file(GLOB CJSONTEST_SRC *.h *.cc *.c )

set(TARGET_SRC ${CJSONTEST_SRC})

me_add_executable(cjsontest)