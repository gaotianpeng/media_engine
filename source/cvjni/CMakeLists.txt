set(MyCategory)
file(RELATIVE_PATH MyCategory ${PROJECT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_SOUCE_DIR}/source/include)
include_directories(d:/programfile/Java/jdk-12.0.2/include)
include_directories(d:/programfile/Java/jdk-12.0.2/include/win32)

file(GLOB LIB_HEADERS *.h)
file(GLOB LIB_SOURCES *.cpp *.cc)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS}) # Not needed for CMake >= 2.8.11
set(LIB_INTERNAL_DEPS ${OpenCV_LIBS})

me_add_library(OpenCVJni)
