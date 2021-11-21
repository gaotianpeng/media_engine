set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/third/gtest CACHE PATH "Path to googletest")
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package (glog 0.6.0 REQUIRED PATHS ${PROJECT_SOURCE_DIR}/third/glog)
include_directories(${GLOG_INCLUDE_DIRS})

find_package(nlohmann_json 3.2.0 REQUIRED  PATHS ${PROJECT_SOURCE_DIR}/third/json)

add_executable(unit_test test_main.cpp ut_main.cpp)

target_link_libraries(unit_test 
    GTest::GTest
    GTest::Main
    ${GTEST_BOTH_LIBRARIES}
    glog::glog
    nlohmann_json::nlohmann_json
    )

target_include_directories(unit_test PRIVATE ${PROJECT_SORUCE_DIR}/inc)     

gtest_add_tests(TARGET unit_test)


message(STATUS "UT PATH: " ${CMAKE_CURRENT_SOURCE_DIR})