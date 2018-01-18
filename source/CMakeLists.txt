cmake_minimum_required(VERSION 3.3)

include_directories(${CMAKE_SOURCE_DIR}/source/dependency/include)
link_directories(${CMAKE_SOURCE_DIR}/source/dependency/lib)

add_subdirectory(UI)
add_subdirectory(test)
add_subdirectory(me)
add_subdirectory(player)
add_subdirectory(utility)

