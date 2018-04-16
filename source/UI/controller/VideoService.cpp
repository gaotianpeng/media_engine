#include "VideoService.h"
#include "txCloudLog.h"

VideoService::VideoService()
	: m_pMediaPlayCallback(NULL)
	, m_mediaRender(NULL)
	, m_mainRender(NULL)
	, m_bOpenMic(false)
{
	GetILive()->setLocalVideoCallBack(onLocalVideo, this);
	GetILive()->setRemoteVideoCallBack(onRemoteVideo, this);
	GetILive()->setDeviceOperationCallback(OnDeviceOperation, this);
}

VideoService::~VideoService()
{
}

VideoService * VideoService::shared_instance()
{
	static VideoService * pVideoService = new VideoService;
	return pVideoService;
}

void VideoService::setCallback(VideoCallback * callback, void * pUserData)
{
	m_callback = callback;

}

void VideoService::setMediaPlayCallback(MediaPlayCallback* callback)
{
	m_pMediaPlayCallback = callback;
}

void VideoService::startLocalPreview(const char * cameraID, const char * micID)
{
	TXCScopedSpinLock lock(m_mapRenderLock);
	if (m_mapRender.find(m_id) != m_mapRender.end())
		return;

	GetILive()->openCamera(cameraID);
	//GetILive()->openMic(micID);
	m_cameraID = cameraID;
	m_micID = micID;

	if (m_mapRender.size() < SUB_RENDER_MAX )
	{
		m_mapRender[m_id] = m_mapRender.size();
	}
}

void VideoService::stopLocalPreview()
{
	GetILive()->closeCamera();
	GetILive()->closeMic();
	
	rearrangeView(m_id.c_str());
}

void VideoService::startRemoteView(const char * id)
{
	TXCScopedSpinLock lock(m_mapRenderLock);
	if (m_mapRender.size() >= SUB_RENDER_MAX || m_mapRender.find(id) != m_mapRender.end())
		return;
	m_mapRender[id] = m_mapRender.size();
}

void VideoService::stopRemoteView(const char * id)
{
	rearrangeView(id);
}

void VideoService::batchRemoteView(const std::vector<std::string> * vIDs)
{
	TXCScopedSpinLock lock(m_mapRenderLock);
	m_mapRender.clear();

	for (int i = 0; i < min((*vIDs).size(), SUB_RENDER_MAX);i++)
	{
		m_mapRender[(*vIDs)[i]] = i;
	}
}

void VideoService::setMute(bool mute)
{
	m_bOpenMic = !mute;
	if (mute)
	{
		GetILive()->closeMic();
	}
	else
	{
		if (m_micID.empty())
		{
			m_micList.clear();
			GetILive()->getMicList(m_micList);
			if (!m_micList.empty())
			{
				m_micID = m_micList[0].first.c_str();
			}			
		}
		GetILive()->openMic(m_micID.c_str());
	}
}

void VideoService::setCameraView(bool open)
{
	if (open)
	{
		GetILive()->openCamera(m_cameraID.c_str());
	}
	else
	{
		GetILive()->closeCamera();
	}
}

void VideoService::startScreenPreView(HWND hWnd, uint32 & fps)
{
	GetILive()->openScreenShare(hWnd, fps);
}

void VideoService::startScreenPreView(int32 & left, int32 & top, int32 & right, int32 & bottom, uint32 & fps)
{
	GetILive()->openScreenShare(left, top, right, bottom, fps);
}

void VideoService::stopScreenPreView()
{
	GetILive()->closeScreenShare();
}

void VideoService::changeScreenPreView(int32& left, int32& top, int32& right, int32& bottom)
{
	GetILive()->changeScreenShareSize(left, top, right, bottom);
}

void VideoService::mediaPlay(const char* szFileName)
{
	GetILive()->openPlayMediaFile(szFileName);
}

int VideoService::mediaResume()
{
	return GetILive()->resumePlayMediaFile();
}

int VideoService::mediaPause()
{
	return GetILive()->pausePlayMediaFile();
}

int VideoService::mediaRestart()
{
	return GetILive()->restartMediaFile();
}

void VideoService::mediaStop()
{
	GetILive()->closePlayMediaFile();
}

void VideoService::openPlayer()
{
	Vector< Pair<String/*id*/, String/*name*/> > playerList;
	GetILive()->getPlayerList(playerList);
	if (playerList.size() > 0)
	{
		GetILive()->openPlayer(playerList[0].first.c_str());
	}
}

void VideoService::closePlayer()
{
	GetILive()->closePlayer();
}

void VideoService::setRenders(QWidgetVideoRender * render, VideoRenderType renderType)
{
	switch (renderType)
	{
	case VIDEO_RENDER_MAIN:
		m_mainRender = render;
		break;
	case VIDEO_RENDER_SUB:
		if (m_subRenderCount < SUB_RENDER_MAX)
		{
			m_subRender[m_subRenderCount] = render;
			m_subRenderCount++;
		}
		break;
	default:
		break;
	}
}

void VideoService::setMediaRender(QWidgetVideoRender* render)
{
	m_mediaRender = render;
}

//void VideoService::updateRenderMap(int mainRenderIndex, const std::map<std::string, int>& renderMap)
//{
//	if (renderMap.size() > 0)
//	{
//		m_mapRender = renderMap;
//		m_mainRenderIndex = mainRenderIndex;
//		enterMainRender(m_mainRenderIndex);
//	}
//}

void VideoService::resetRenders()
{
	m_mainRender = nullptr;

	for (int i = 0; i < SUB_RENDER_MAX; i++)
	{
		m_subRender[i] = nullptr;
	}
	//m_mapRender.clear();
	m_subRenderCount = 0;
	//m_mainRenderIndex = -1;
}

void VideoService::resetRenderMap()
{
	m_mapRender.clear();
}

void VideoService::setId(const char * id)
{
	m_id = id;
}

void VideoService::enterMainRender(int index)
{
	if (index <= SUB_RENDER_MAX)
	{
		for (auto it = m_mapRender.begin();it!=m_mapRender.end();it++)
		{
			if (it->second + 1 == index)
			{
				m_mainRenderID = it->first;
				m_mainRenderIndex = index;
				break;
			}
		}
	}
}

void VideoService::exitMainRender(int index)
{
	//0 为index，即为dis_main接收到esc
	if (m_mainRenderIndex == index || 0 == index)
	{
		m_mainRenderID = "";
	}
}

void VideoService::setMainRenderType(E_VideoSrc mainRenderType)
{
	m_mainRenderType = mainRenderType;
}

void VideoService::doRender(const char * id, E_VideoSrc srcType, const LiveVideoFrame * videoFrame)
{
	QWidgetVideoRender *render = getRender(id);

	if (render && srcType == VIDEO_SRC_TYPE_CAMERA)
	{
		render->DoRender(videoFrame);
	}
	if (m_mainRender && srcType == m_mainRenderType)
	{
		if (videoFrame->desc.srcType == VIDEO_SRC_TYPE_CAMERA && m_mainRenderID != id)
		{
			return;
		}
		
		if (videoFrame->desc.srcType == VIDEO_SRC_TYPE_MEDIA)
		{
			m_mediaRender->DoRender(videoFrame);
		}
		else
		{
			m_mainRender->DoRender(videoFrame);
		}
	}
}

void VideoService::changeRole(const char * szControlRole)
{
	//openCamera();
	m_role = szControlRole;
	GetILive()->changeRole(szControlRole, OnChangeRoleSuc, OnChangeRoleErr, this);
}

void VideoService::OnChangeRoleSuc(void* data)
{
	VideoService *videoService = (VideoService*)data;
	if (videoService->getRole() != Guest)
	{
		videoService->openCamera();
		videoService->setMute(false);
	}
	else
	{
		videoService->closeCamera();
		videoService->setMute(true);
	}

}

void VideoService::OnChangeRoleErr(int code, const char *desc, void* data)
{
	VideoService *videoService = (VideoService*)data;
}

void VideoService::OnDeviceOperation(E_DeviceOperationType oper, int retCode, void* data)
{
	VideoService* pThis = (VideoService*)data;
	switch (oper)
	{
	case E_OpenPlayMediaFile:
		pThis->m_pMediaPlayCallback->onOpenMediaPlay();
		break;
	case E_ClosePlayMediaFile:
		pThis->m_pMediaPlayCallback->onCloseMediaPlay();
		break;
	case E_OpenPlayer:
		if (pThis->m_pMediaPlayCallback)
		{
			pThis->m_pMediaPlayCallback->onOpenPlayer();
		}		
		break;
	case E_ClosePlayer:
		if (pThis->m_pMediaPlayCallback)
		{
			pThis->m_pMediaPlayCallback->onClosePlayer();
		}		
		break;
	case E_OpenScreenShare:
		break;
	case E_CloseScreenShare:
		break;
	default:
		break;
	}
}


std::string VideoService::getRole()
{
	return m_role;
}

void VideoService::openCamera()
{
	if (m_cameraID.empty())
	{
		m_cameraList.clear();
		GetILive()->getCameraList(m_cameraList);
		m_cameraID = m_cameraList[0].first.c_str();
	}
	GetILive()->openCamera(m_cameraID.c_str());
}

void VideoService::closeCamera()
{
	GetILive()->closeCamera();
}

void VideoService::updateHomeScreen(std::string& screen, std::string& id)
{
	if (screen == "camera")
	{
		setMainRenderType(VIDEO_SRC_TYPE_CAMERA);
	}
	else if (screen == "screen")
	{
		setMainRenderType(VIDEO_SRC_TYPE_SCREEN);
	}
	else if (screen == "video")
	{
		setMainRenderType(VIDEO_SRC_TYPE_MEDIA);
	}
	else
	{
		setMainRenderType(VIDEO_SRC_TYPE_NONE);
	}
	m_mainRenderID = id;
}

void VideoService::getCameraList(Vector< Pair<String, String> >& cameraList)
{
	cameraList = m_cameraList;
}

void VideoService::getMicList(Vector< Pair<String, String> >& micList)
{
	micList = m_micList;
}

void VideoService::getCurrentCamera(std::string& cameraID)
{
	cameraID = m_cameraID;
}

void VideoService::getCurrentMic(std::string& micID)
{
	micID = m_micID;
}

void VideoService::setCamera(std::string& cameraID)
{
	m_cameraID = cameraID;
}

void VideoService::setMic(std::string& micID)
{
	m_micID = micID;
}

bool VideoService::getMicOpen()
{
	return m_bOpenMic;
}

void VideoService::setMicOpen(bool bOpen)
{
	m_bOpenMic = bOpen;
	if (m_bOpenMic)
	{
		GetILive()->openMic(m_micID.c_str());
	}
	else
	{
		GetILive()->closeMic();
	}

}

void VideoService::setMainRenderID(std::string renderID)
{
	m_mainRenderID = renderID;
}


void VideoService::onLocalVideo(const LiveVideoFrame * videoFrame, void * customData)
{
	VideoService *videoService = (VideoService*)customData;
	videoService->doRender(videoFrame->identifier.c_str(), videoFrame->desc.srcType, videoFrame);	
}

void VideoService::onRemoteVideo(const LiveVideoFrame * videoFrame, void* customData)
{
	VideoService *videoService = (VideoService*)customData;
	videoService->doRender(videoFrame->identifier.c_str(), videoFrame->desc.srcType, videoFrame);
}

QWidgetVideoRender * VideoService::getRender(const char * id)
{
	auto it = m_mapRender.find(id);
	if (it != m_mapRender.end())
	{
		return m_subRender[it->second];
	}

	return nullptr;
}

void VideoService::rearrangeView(const char * id)
{
	TXCScopedSpinLock lock(m_mapRenderLock);
	auto it = m_mapRender.find(id);
	if (it != m_mapRender.end())
	{
		auto index = it->second;
		m_mapRender.erase(id);

		for (auto i = m_mapRender.begin(); i != m_mapRender.end(); ++i)
		{
			if (i->second > index)
			{
				i->second = i->second - 1;
			}
		}
	}
}
