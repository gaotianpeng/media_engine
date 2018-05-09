cmake_minimum_required(VERSION 3.3)

project(media_engine CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

if(MSVC AND NOT EXISTS "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user")
	file(GENERATE
		OUTPUT "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user"
		INPUT "${CMAKE_SOURCE_DIR}/cmake/ALL_BUILD.vcxproj.user.in")
endif()

message(STATUS "The cmake module path is: " ${CMAKE_MODULE_PATH})

if(MSVC)
    set(CMAKE_C_FLAGS_DEBUG "/DDEBUG=1 /D_DEBUG=1 ${CMAKE_C_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "/DDEBUG=1 /D_DEBUG=1 ${CMAKE_C_FLAGS_DEBUG}")

    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
    endif()
endif()

if(WIN32)
    if (DepsPath OR DEFINED ENV{DepsPath} OR DEFINED ENV{DepsPath32} OR DEFINED ENV{DepsPath64})
    else()
        set(DepsPath "" CACHE PATH "Path to compiled dependencies (e.g. D:/dependencies/win64)")
        message(WARNING "DepsPath variable is missing.  Please set this variable to specify path to compiled dependencies (e.g. D:/dependencies/win64)")
    endif()
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/CMake/Modules")

include(UtilityMacros)
add_subdirectory(source)