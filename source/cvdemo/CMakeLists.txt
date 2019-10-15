set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_SOUCE_DIR}/source/include)

file(GLOB CVDEMO_SRC *.h *.cc )

set(TARGET_SRC ${CVDEMO_SRC})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS}) # Not needed for CMake >= 2.8.11
set(LIB_INTERNAL_DEPS ${OpenCV_LIBS})

me_add_executable(cvdemo)