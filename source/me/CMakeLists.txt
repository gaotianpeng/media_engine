cmake_minimum_required(VERSION 3.3)

add_definitions(-D MATH_EXPORTS)

file(GLOB SOURCE_FILES_H "*.h")
file(GLOB SOURCE_FILES "*.cpp")

set(TARGET_SRC 
    ${SOURCE_FILES}
    ${SOURCE_FILES_H} 
)

message(STATUS "     " ${OUTPUT_BIN_DIR})

get_filename_component(ProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "me " ${PROJECT_BINARY_DIR})
add_library(${ProjectName} SHARED ${TARGET_SRC})