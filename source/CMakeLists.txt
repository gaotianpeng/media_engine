include_directories(${CMAKE_SOURCE_DIR}/dependency/include)
link_directories(${CMAKE_SOURCE_DIR}/dependency/lib)

add_subdirectory(av_mux)
add_subdirectory(UI)
add_subdirectory(test)
add_subdirectory(me)
add_subdirectory(utility)