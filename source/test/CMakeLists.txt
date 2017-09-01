
set(CMAKE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/dependency/bin)
link_directories(${CMAKE_LIBRARY_PATH})

set(GTEST_SRC sample.cpp test_main.cpp)
set(GTEST_LIB gmock.lib gmock_main.lib gtest.lib gtest_main.lib)
set(LOG_LIB easyloggingpp.lib)

add_executable(unittest ${GTEST_SRC})
target_link_libraries(unittest ${GTEST_LIB} ${LOG_LIB})
