cmake_minimum_required(VERSION 3.3)

include_directories(${CMAKE_SOURCE_DIR}/dependency/include)

add_subdirectory(UI)
add_subdirectory(test)
add_subdirectory(me)
add_subdirectory(coding_module)

