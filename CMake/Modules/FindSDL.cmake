# Once done these will be defined:
#
#  SDL_FOUND
#  SDL_INCLUDE_DIRS
#  SDL_LIBRARIES
#

find_package(PkgConfig QUIET)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(_lib_suffix 64)
else()
	set(_lib_suffix 32)
endif()

find_path(SDL_INCLUDE_DIR
	NAMES sdl/SDL.h
	HINTS
		${DepsPath${_lib_suffix}}
		${DepsPath}
	PATHS
		/usr/include /usr/local/include /opt/local/include /sw/include
	PATH_SUFFIXES
		include)

find_library(SDL_LIB
	NAMES ${_SDL_LIBRARIES} SDL2 SDL2main
	HINTS
		ENV DepsPath
		${DepsPath${_lib_suffix}}
		${DepsPath}
		${_SDL_LIBRARY_DIRS}
	PATH_SUFFIXES
		lib${_lib_suffix} lib
		libs${_lib_suffix} libs
		bin${_lib_suffix} bin
		../lib${_lib_suffix} ../lib
		../libs${_lib_suffix} ../libs
		../bin${_lib_suffix} ../bin)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL DEFAULT_MSG GTEST_LIB GTEST_INCLUDE_DIR)
mark_as_advanced(SDL_INCLUDE_DIR SDL_LIB)

if(SDL_FOUND)
	set(SDL_INCLUDE_DIRS ${SDL_INCLUDE_DIR})
	set(SDL_LIBRARIES ${SDL_LIB})
endif()