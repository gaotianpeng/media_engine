option(ENABLE_UI "Enables the media_engine user interfaces" ON)
if(DISABLE_UI)
	message(STATUS "UI disabled")
	return()
elseif(ENABLE_UI)
	set(FIND_MODE REQUIRED)
else()
	set(FIND_MODE QUIET)
endif()

project(UI)

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

include_directories(${CMAKE_PREFIX_PATH}/include)

include_directories(../tencent-sdk/include)
include_directories(../tencent-sdk/include/FFmpeg)
include_directories(../tencent-sdk/include/iLiveSDK)
include_directories(../tencent-sdk/include/IMSDK)
include_directories(model)
include_directories(model/ilive)
include_directories(view)
include_directories(view/ilive)
include_directories(model/ilive)
include_directories(controller)
include_directories(base)

link_directories(../tencent-sdk/lib)

if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_output_suffix "64")
	else()
		set(_output_suffix "32")
	endif()
endif()

if(WIN32)
	set(me_PLATFORM_SOURCES
		ui.rc
		)
endif()

file(GLOB me_HEADERS "*.h")
file(GLOB me_SOURCES "*.cpp")

file(GLOB view_HEADERS view "*.h")
message(STATUS "view headers " ${view_HEADERS})

set(me_base_HEADERS
	base/gzip.h
	base/txCloudLog.h
	)
set(me_base_SOURCES
	base/gzip.cpp
	base/txCloudLog.cpp)

set(me_controller_HEADERS
	controller/AccountAuth.h
	controller/AccountService.h
	controller/BoardService.h
	controller/IMService.h
	controller/MemberService.h
	controller/notify.h
	controller/VideoService.h)
set(me_controller_SOURCES
	controller/AccountAuth.cpp
	controller/AccountService.cpp
	controller/BoardService.cpp
	controller/IMService.cpp
	controller/MemberService.cpp
	controller/notify.cpp
	controller/VideoService.cpp)

set(me_lock_HEADERS
	lock/TXCLock.h
	lock/TXCMutex.h
	lock/TXCSpinLock.h)
set(me_lock_SOURCES
	lock/TXCMutex.cpp)

set(me_model_HEADERS
	model/avsdkTime.h
	model/commonType.h
	model/ilive/ConfReq.h
	model/ilive/ConfRsp.h
	model/ilive/Context.h)
set(me_model_SOURCES
	model/ilive/ConfReq.cpp
	model/ilive/ConfRsp.cpp
	model/ilive/Context.cpp)

set(me_view_HEADERS
	view/QDialogCourseInfo.h
	view/QDialogCourseList.h
	view/QDialogDeviceMgr.h
	view/QDialogInvite.h
	view/QDialogMessage.h
	view/QDialogProgress.h
	view/QProgressIndicator.h
	view/QSliderCustom.h
	view/QTextEditCustomEditor.h
	view/QWidgetBottomBar.h
	view/QWidgetCourseItem.h
	view/QWidgetMediaPlayer.h
	view/QWidgetMemberItem.h
	view/QWidgetMemberPanel.h
	view/QWidgetMsgItem.h
	view/QWidgetMsgPanel.h
	view/QWidgetSMain.h
	view/QWidgetTab.h
	view/QWidgetToast.h
	view/QWidgetVideo.h
	view/QWidgetVolume.h
	view/QWindowCourse.h
	view/QWindowScreenShareArea.h
	view/QWindowScreenTool.h
	view/QWindowShareRect.h
	view/QWindowShareToolBar.h
	view/QWindowWhiteBoard.h
	view/ilive/QWidgetVideoRender.h

)
set(me_view_SOURCES
	view/QDialogCourseInfo.cpp
	view/QDialogCourseList.cpp
	view/QDialogDeviceMgr.cpp
	view/QDialogInvite.cpp
	view/QDialogMessage.cpp
	view/QDialogProgress.cpp
	view/QProgressIndicator.cpp
	view/QSliderCustom.cpp
	view/QTextEditCustomEditor.cpp
	view/QWidgetBottomBar.cpp
	view/QWidgetCourseItem.cpp
	view/QWidgetMediaPlayer.cpp
	view/QWidgetMemberItem.cpp
	view/QWidgetMemberPanel.cpp
	view/QWidgetMsgItem.cpp
	view/QWidgetMsgPanel.cpp
	view/QWidgetSMain.cpp
	view/QWidgetTab.cpp
	view/QWidgetToast.cpp
	view/QWidgetVideo.cpp
	view/QWidgetVolume.cpp
	view/QWindowCourse.cpp
	view/QWindowScreenShareArea.cpp
	view/QWindowScreenTool.cpp
	view/QWindowShareRect.cpp
	view/QWindowShareToolBar.cpp
	view/QWindowWhiteBoard.cpp
	view/ilive/QWidgetVideoRender.cpp)

set(me_UI 
	forms/QDialogCourseInfo.ui
	forms/QDialogCourseList.ui
	forms/QDialogDeviceMgr.ui
	forms/QDialogInvite.ui
	forms/QDialogMessage.ui
	forms/QDialogProgress.ui
	forms/QWidgetBottomBar.ui
	forms/QWidgetCourseItem.ui
	forms/QWidgetMediaPlayer.ui
	forms/QWidgetMemberItem.ui
	forms/QWidgetMemberPanel.ui
	forms/QWidgetMsgItem.ui
	forms/QWidgetMsgPanel.ui
	forms/QWidgetSMain.ui
	forms/QWidgetTab.ui
	forms/QWidgetVideo.ui
	forms/QWidgetVolume.ui
	forms/QWindowCourse.ui
	forms/QWindowScreenShareArea.ui
	forms/QWindowScreenTool.ui
	forms/QWindowShareRect.ui
	forms/QWindowShareToolBar.ui
	forms/QWindowWhiteBoard.ui)
set(me_QRC
	res/Course.qrc
	res/WhiteBoard.qrc
	)

message(STATUS "The me_UI dir is: " ${me_UI})

qt5_wrap_ui(me_UI_HEADERS ${me_UI})
qt5_add_resources(me_QRC_SOURCES ${me_QRC})


source_group("base\\Header Files" FILES ${me_base_HEADERS})
source_group("base\\Source Files" FILES ${me_base_SOURCES})
source_group("controller\\Header Files" FILES ${me_controller_HEADERS})
source_group("controller\\Source Files" FILES ${me_controller_SOURCES})
source_group("lock\\Header Files" FILES ${me_lock_HEADERS})
source_group("lock\\Source Files" FILES ${me_lock_SOURCES})
source_group("model\\Header Files" FILES ${me_model_HEADERS})
source_group("model\\Source Files" FILES ${me_model_SOURCES})
source_group("view\\Header Files" FILES ${me_view_HEADERS})
source_group("view\\Source Files" FILES ${me_view_SOURCES})

add_executable(UI WIN32
	jsoncpp.cpp
	${me_base_HEADERS}
	${me_base_SOURCES}
	${me_controller_HEADERS}
	${me_controller_SOURCES}
	${me_lock_HEADERS}
	${me_lock_SOURCES}
	${me_model_HEADERS}
	${me_model_SOURCES}
	${me_view_HEADERS}
	${me_view_SOURCES}
	${me_SOURCES}
	${me_UI}

	${me_UI_HEADERS}
	${me_QRC_SOURCES})

set(third_LIB avcodec.lib
	avformat.lib
	avutil.lib
	BoardSDK.lib
	CosSDK.lib
	iLiveSDK.lib
	libtim.lib
	swresample.lib
	zlib.lib
)

target_link_libraries(UI Qt5::Widgets
	${third_LIB})