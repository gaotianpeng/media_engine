set(CMAKE_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/dependency/include)
set(CMAKE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/dependency/bin)

include_directories(${CMKAE_INCLUDE_PATH})
message(STATUS "cmake include path is :" ${CMAKE_INCLUDE_PATH})
link_directories(${CMAKE_LIBRARY_PATH})
message(STATUS "cmake library path is : " ${CMAKE_LIBRARY_PATH})


set(GTEST_SRC sample1.cpp sample1_unittest.cpp)
set(GTEST_LIB gmock.lib gmock_main.lib gtest.lib gtest_main.lib)

add_executable(test_main ${GTEST_SRC})
target_link_libraries(test_main ${GTEST_LIB})
