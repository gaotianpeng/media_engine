# project(FlvParser)

if(DEFINED QTDIR${_lib_suffix})
	list(APPEND CMAKE_PREFIX_PATH "${QTDIR${_lib_suffix}}")
elseif(DEFINED QTDIR)
	list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")
elseif(DEFINED ENV{QTDIR${_lib_suffix}})
	list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR${_lib_suffix}}")
elseif(DEFINED ENV{QTDIR})
	list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(_lib_suffix 64)
else()
	set(_lib_suffix 32)
endif()

find_package(Qt5Widgets ${FIND_MODE})

if(NOT Qt5Widgets_FOUND)
	if(ENABLE_UI)
		message(FATAL_ERROR "Failed to find Qt5")
	else()
		message(STATUS "Qt5 not found - UI disabled")
		return()
	endif()
endif()

include_directories(${CMAKE_PREFIX_PATH}/include ${CMAKE_SOURCE_DIR}/source/include)
message(STATUS ${CMAKE_PREFIX_PATH})

if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_output_suffix "64")
	else()
		set(_output_suffix "32")
	endif()
endif()

file(GLOB me_HEADERS "*.h")
file(GLOB me_SOURCES "*.cc")

set(me_UI
	forms/MEBasic.ui)

message(STATUS "The me_UI dir is: " ${me_UI})

qt5_wrap_ui(me_UI_HEADERS ${me_UI})

add_executable(FlvParser WIN32
	${me_HEADERS}
	${me_SOURCES}
	${me_UI})

add_custom_command(TARGET FlvParser
	POST_BUILD
	COMMAND  editbin /SUBSYSTEM:CONSOLE "$(OUTDIR)$(TargetName).exe"
	COMMENT "Running console window"
	)

target_link_libraries(FlvParser Qt5::Widgets)