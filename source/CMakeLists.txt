cmake_minimum_required(VERSION 3.3)

include_directories(${CMAKE_SOURCE_DIR}/dependency/include)
link_directories(${CMAKE_SOURCE_DIR}/dependency/lib)

add_subdirectory(UI)
add_subdirectory(test)
add_subdirectory(me)
add_subdirectory(utility)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/dependency/include)
message(STATUS "The cmake module path is: " ${include_dir})

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependency/include)
# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependency/lib)