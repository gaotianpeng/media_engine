set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_SOUCE_DIR}/source/include)

file(GLOB TEST_SRC *.h *.cc *.cpp)

set(TARGET_SRC ${TEST_SRC})

me_add_executable(test)