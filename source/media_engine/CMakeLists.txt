add_definitions(-D MATH_EXPORTS)

file(GLOB SOURCE_FILES_H "*.h")
file(GLOB SOURCE_FILES "*.cc")

set(TARGET_SRC 
    ${SOURCE_FILES}
    ${SOURCE_FILES_H} 
)

message(STATUS "     " ${OUTPUT_BIN_DIR})
include_directories(${CMAKE_SOURCE_DIR}/source/include)

get_filename_component(ProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "me " ${PROJECT_BINARY_DIR})
add_library(${ProjectName} SHARED ${TARGET_SRC})