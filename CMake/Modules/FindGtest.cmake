# Once done these will be defined:
#
#  GTEST_FOUND
#  GTEST_INCLUDE_DIRS
#  GTEST_LIBRARIES
#

find_package(PkgConfig QUIET)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(_lib_suffix 64)
else()
	set(_lib_suffix 32)
endif()

find_path(GTEST_INCLUDE_DIR
	NAMES gmock/gmock.h
	HINTS
		${DepsPath${_lib_suffix}}
		${DepsPath}
	PATHS
		/usr/include /usr/local/include /opt/local/include /sw/include
	PATH_SUFFIXES
		include)

find_library(GTEST_LIB
	NAMES ${_GTEST_LIBRARIES} gmockd gmock
	HINTS
		ENV DepsPath
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_GTEST_LIBRARY_DIRS}
	PATH_SUFFIXES
		lib${_lib_suffix} lib
		libs${_lib_suffix} libs
		bin${_lib_suffix} bin
		../lib${_lib_suffix} ../lib
		../libs${_lib_suffix} ../libs
		../bin${_lib_suffix} ../bin)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(gtest DEFAULT_MSG GTEST_LIB GTEST_INCLUDE_DIR)
mark_as_advanced(GTEST_INCLUDE_DIR GTEST_LIB)

if(GTEST_FOUND)
	set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
	set(GTEST_LIBRARIES ${GTEST_LIB})
endif()