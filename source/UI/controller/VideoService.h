#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "iLive.h"
#include "QWidgetVideoRender.h"
#include "lock/TXCLock.h"

using namespace ilive;

#define SUB_RENDER_MAX 5
#define LiveMaster	"LiveMaster" //主播
#define LiveGuest	"LiveGuest"  //连麦观众
#define Guest		"Guest"		 //观众

enum VideoEventCode
{
	VIDEO_EVENT_PLAY_BEGIN = 1000,
	VIDEO_EVENT_PLAY_DISCONNECT = 1001,
	VIDEO_EVENT_PLAY_ERROR = 1002,

	VIDEO_EVENT_PUSH_BEGIN = 2000,
	VIDEO_EVENT_PUSH_DISCONNECT = 2001,
	VIDEO_EVENT_PUSH_ERROR = 2002,
};

enum VideoRenderType
{
	VIDEO_RENDER_MAIN = 1,
	VIDEO_RENDER_SUB = 2,
};

class VideoCallback
{
public:
	virtual ~VideoCallback() = default;

	virtual void onVideoEventCallback(VideoEventCode status, const char *desc) = 0;
};

class MediaPlayCallback
{
public:
	virtual ~MediaPlayCallback() = default;

	virtual void onOpenMediaPlay() = 0;
	virtual void onCloseMediaPlay() = 0;
	virtual void onOpenPlayer() = 0;
	virtual void onClosePlayer() = 0;
};

class VideoService
{
public:
	VideoService();
	virtual ~VideoService();

	static VideoService* shared_instance();

	void setCallback(VideoCallback* callback, void* pUserData);
	void setMediaPlayCallback(MediaPlayCallback* callback);
	
	//local 和 remote 没有区分，因此MAIN_USER建议先startlocal再startRemote，否则视频位第一个可能不是自己的视频
	void startLocalPreview(const char * cameraID, const char * micID);
	void stopLocalPreview();

	void startRemoteView(const char * id);
	void stopRemoteView(const char * id);
	void batchRemoteView(const std::vector<std::string> * vIDs);

	void startScreenPreView(HWND hWnd, uint32& fps);
	void startScreenPreView(int32& left, int32& top, int32& right, int32& bottom, uint32& fps);
	void stopScreenPreView();
	void changeScreenPreView(int32& left, int32& top, int32& right, int32& bottom);

	void mediaPlay(const char* szFileName);
	int mediaResume();
	int mediaPause();
	int mediaRestart();
	void mediaStop();

	void setMute(bool mute);
	void setCameraView(bool open);
	void openCamera();
	void closeCamera();

	void openPlayer();
	void closePlayer();

	void setRenders(QWidgetVideoRender * render, VideoRenderType renderType);
	void setMediaRender(QWidgetVideoRender* render);
	void resetRenders();
	void resetRenderMap();

	void setId(const char * id);
	void enterMainRender(int index);
	void exitMainRender(int index);
	void setMainRenderType(E_VideoSrc mainRenderType);
	void doRender(const char * id, E_VideoSrc srcType, const LiveVideoFrame * videoFrame);
	void updateHomeScreen(std::string& screen, std::string& id);

	std::string getRole();
	void changeRole(const char * szControlRole);

	void getCameraList(Vector< Pair<String, String> >& cameraList); 
	void getMicList(Vector< Pair<String, String> >& micList);
	void getCurrentCamera(std::string& cameraID);
	void getCurrentMic(std::string& micID);
	void setCamera(std::string& cameraID);
	void setMic(std::string& micID);
	bool getMicOpen();
	void setMicOpen(bool bOpen);
	void setMainRenderID(std::string renderID);

private:
	static void onLocalVideo(const LiveVideoFrame* videoFrame, void* customData);
	static void onRemoteVideo(const LiveVideoFrame* videoFrame, void* customData);
	static void OnChangeRoleSuc(void* data);
	static void OnChangeRoleErr(int code, const char *desc, void* data);
	static void OnDeviceOperation(E_DeviceOperationType oper, int retCode, void* data);
	QWidgetVideoRender * getRender(const char * id);
	void rearrangeView(const char * id);

	VideoCallback* m_callback;
	MediaPlayCallback* m_pMediaPlayCallback;
	QWidgetVideoRender *m_subRender[SUB_RENDER_MAX];
	QWidgetVideoRender *m_mainRender;
	QWidgetVideoRender *m_mediaRender;
	int m_subRenderCount = 0;
	std::map<std::string, int> m_mapRender;
	std::string m_id;
	std::string m_cameraID;
	std::string m_micID;
	std::string m_mainRenderID;
	std::string m_role;
	int m_mainRenderIndex = -1;
	TXCSpinLock m_mapRenderLock;
	E_VideoSrc m_mainRenderType = VIDEO_SRC_TYPE_CAMERA;
	Vector< Pair<String/*id*/, String/*name*/> > m_cameraList;
	Vector< Pair<String/*id*/, String/*name*/> > m_micList;
	bool m_bOpenMic;
};