/**************************************************************************
    Copyright:      Copyright ? 1998 - 2018 Tencent. All Rights Reserved
    Created:        2018-1-17 17:00:04
    Filename:       AccountService.cpp

    Description:
***************************************************************************/

#include "AccountService.h"
#include "txCloudLog.h"
#include "TXCHttpClient.h"
#include "json.h"
#include "Context.h"
#include "ConfReq.h"
#include "ConfRsp.h"
#include "iLive.h"
#include "IMService.h"
#include <assert.h>
#include <QTime>
#include "QDialogMessage.h"

static const wchar_t* reqURLPrefix = L"https://sxb.qcloud.com/conf_svr_sdk/conference_server/public/api/conference?sdkappid=";


/**************************************************************************/

AccountService::AccountService(int sdkAppId, int accountType)
    : m_callback(nullptr)
    , m_http(new TXCHttpClient(L"TXCloudEdu"))
    , m_stop(false)
    , m_heartBeatMutex()
    , m_condition()
    , m_thread()
{
	Context::instance().setSdkAppID(sdkAppId);
	Context::instance().setAccountType(accountType);
    ilive::GetILive()->init(sdkAppId, accountType);
}

AccountService::~AccountService()
{
	if (m_thread.joinable())
	{
		m_thread.join();
	}

}

void AccountService::setCallback(IAccountService* callback)
{
    m_callback = callback;
}

void AccountService::queryRoomList()
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(Context::instance().sdkAppID()));
    if (Context::instance().isLogin())
    {
        url.append(L"&identifier=")
            .append(TXCHttpRequest::a2w(Context::instance().userId()))
            .append(L"&user_token=")
            .append(TXCHttpRequest::a2w(Context::instance().token()));
    }

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    RoomListReq req;
    std::string body = req.GenReq();
	QTime tBefore = QTime::currentTime();

    m_http->asyn_post(url, headers, body, [this, tBefore](DWORD code, std::string& resp) {
		QTime tAfter = QTime::currentTime();
		txCloudEduLog_i("txcloud", "queryRoomList use = %d msec", tAfter.msec() - tBefore.msec());
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onQueryRoomList(-1, "", std::vector<RoomItem>());
            }
        }
        else
        {
            RoomListRsp rsp;
            rsp.Parse(resp.c_str());

            std::unique_lock<std::mutex> lock(m_heartBeatMutex);

            if (m_callback)
            {
                m_callback->onQueryRoomList(rsp.GetCode(), rsp.GetMsg().c_str(), rsp.GetRoomList());
            }
        }
    });
}

void AccountService::registerAccount(const std::string& id, const std::string& pwd)
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(Context::instance().sdkAppID()));

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    RegisterReq req(id, pwd);
    std::string body = req.GenReq();
	QTime tBefore = QTime::currentTime();

    m_http->asyn_post(url, headers, body, [this, tBefore](DWORD code, std::string& resp) {
		QTime tAfter = QTime::currentTime();
		txCloudEduLog_i("txcloud", "createRoom use = %d msec", tAfter.msec() - tBefore.msec());
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onRegister(-1, "");
            }
        }
        else
        {
            RegisterRsp rsp;
            rsp.Parse(resp.c_str());
            if (m_callback)
            {
                m_callback->onRegister(rsp.GetCode(), rsp.GetMsg().c_str());
            }
        }
    });
}

void AccountService::loginAccount(const std::string& id, const std::string& pwd)
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(Context::instance().sdkAppID()));

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    LoginReq req(id, pwd);
    std::string body = req.GenReq();
	QTime tBefore = QTime::currentTime();

    m_http->asyn_post(url, headers, body, [this, tBefore](DWORD code, std::string& resp) {
		QTime tAfter = QTime::currentTime();
		txCloudEduLog_i("txcloud", "createRoom use = %d msec", tAfter.msec() - tBefore.msec());
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onLogin(-1, "");
            }
        }
        else
        {
            LoginRsp rsp;
            rsp.Parse(resp);
            if (0 != rsp.GetCode())
            {
                Context::instance().setToken("");
                Context::instance().setUserSig("");
            }
            else
            {
                Context::instance().setToken(rsp.GetToken());
                Context::instance().setUserSig(rsp.GetSig());
            }

            if (m_callback)
            {
                m_callback->onLogin(rsp.GetCode(), rsp.GetMsg().c_str());
            }
        }
    });
}

void AccountService::loginiLive()
{
    ilive::GetILive()->login(Context::instance().userId().c_str(), Context::instance().userSig().c_str(), onIliveLoginSuc, onIliveLoginErr, this);
}

void AccountService::logoutiLive()
{
	ilive::GetILive()->logout(onIliveLogoutSuc, onIliveLogoutErr, this);
}

void AccountService::logoutAccount()
{

}

void AccountService::createRoom(const std::string& courtName, const std::string& pwd, const std::string& nickName)
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(Context::instance().sdkAppID()));
    if (Context::instance().isLogin())
    {
        url.append(L"&identifier=")
            .append(TXCHttpRequest::a2w(Context::instance().userId()))
            .append(L"&user_token=")
            .append(TXCHttpRequest::a2w(Context::instance().token()));
    }

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    CreateConfReq req(courtName, pwd);
    std::string body = req.GenReq();
	QTime tBefore = QTime::currentTime();

    m_http->asyn_post(url, headers, body, [this, tBefore](DWORD code, std::string& resp) {
		QTime tAfter = QTime::currentTime();
		txCloudEduLog_i("txcloud", "createRoom use = %d msec", tAfter.msec() - tBefore.msec());
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onCreateRoom(-1, "");
            }
        }
        else
        {
            CreateConfRsp rsp;
            rsp.Parse(resp);
            if (0 == rsp.GetCode())
            {
                Context::instance().setRoomID(rsp.GetRoomId());
                Context::instance().setRoomSig(rsp.GetRoomToken());
                Context::instance().setRoomName(rsp.GetRoomName());
                Context::instance().setRoomOwner(rsp.GetRoomOwner());
                Context::instance().setChatGroudID(rsp.GetChatGroupId());
                Context::instance().setWhiteBoardGroudID(rsp.GetBoardGroupId());
                Context::instance().setHeartbeatInterval(rsp.GetHeartbeat());
                Context::instance().setJoinConfURL(rsp.GetInviteUrl());
            }

            if (m_callback)
            {
                m_callback->onCreateRoom(rsp.GetCode(), rsp.GetMsg().c_str());
            }
        }
    });
}

void AccountService::createiLiveRoom()
{
    //ilive::iLiveRoomOption roomOption;
    //roomOption.audioCategory = ilive::AUDIO_CATEGORY_MEDIA_PLAY_AND_RECORD;//互动直播场景
    //roomOption.roomId = m_curRoomInfo.info.roomnum;
    //roomOption.authBits = AUTH_BITS_DEFAULT;
    //roomOption.controlRole = ilive::LiveMaster;
    //roomOption.roomDisconnectListener = Live::OnRoomDisconnect;
    //roomOption.memberStatusListener = Live::OnMemStatusChange;
    ////roomOption.qualityParamCallback = Live::OnQualityParamCallback;
    //roomOption.data = m_pLive;
    //GetILive()->createRoom(roomOption, OniLiveCreateRoomSuc, OniLiveCreateRoomErr, this);
}

void AccountService::joinRoom(int confID, const std::string& pwd, const std::string& nickName)
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(Context::instance().sdkAppID()));
    if (Context::instance().isLogin())
    {
        url.append(L"&identifier=")
            .append(TXCHttpRequest::a2w(Context::instance().userId())) 
            .append(L"&user_token=")
            .append(TXCHttpRequest::a2w(Context::instance().token()));
    }

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    JoinConfReq req(confID, pwd, nickName);
    std::string body = req.GenReq();
	QTime tBefore = QTime::currentTime();

    m_http->asyn_post(url, headers, body, [this, tBefore](DWORD code, std::string& resp) {
		QTime tAfter = QTime::currentTime();
		txCloudEduLog_i("txcloud", "joinRoom use = %d msec", tAfter.msec() - tBefore.msec());
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onJoinRoom(-1, "");
            }
        }
        else
        {
            JoinConfRsp rsp;
            rsp.Parse(resp);
            if (0 == rsp.GetCode())
            {
                Context::instance().setRoomID(rsp.GetRoomId());
                Context::instance().setRoomSig(rsp.GetRoomToken());
                Context::instance().setRoomName(rsp.GetRoomName());
                Context::instance().setRoomOwner(rsp.GetRoomOwner());
                Context::instance().setChatGroudID(rsp.GetChatGroupId());
                Context::instance().setWhiteBoardGroudID(rsp.GetBoardGroupId());
                Context::instance().setHeartbeatInterval(rsp.GetHeartbeat());
                Context::instance().setJoinConfURL(rsp.GetInviteUrl());
				Context::instance().setHomeScreen(rsp.GetHomeScreen());
            }

            if (m_callback)
            {
                m_callback->onJoinRoom(rsp.GetCode(), rsp.GetMsg().c_str());
            }
        }
    });
}

void AccountService::joiniLiveRoom()
{
    ilive::iLiveRoomOption roomOption;
    roomOption.audioCategory = ilive::AUDIO_CATEGORY_MEDIA_PLAY_AND_RECORD;
    roomOption.roomId = Context::instance().roomID();
    roomOption.groupId = ilive::String(Context::instance().chatGroudID().c_str());
    roomOption.joinImGroup = false;
    roomOption.authBits = ilive::AUTH_BITS_DEFAULT;
    roomOption.memberStatusListener = onMemStatusChange;//自定义的成员状态变化回调
    roomOption.roomDisconnectListener = onRoomDisconnect;//自定义的房间断连的回调
    roomOption.autoRequestCamera = true;
    roomOption.autoRequestScreen = true;
    ilive::GetILive()->joinRoom(roomOption, oniLiveJoinRoomSuc, oniLiveJoinRoomErr, this);
}

void AccountService::quitRoom()
{
    AccountService* service = reinterpret_cast<AccountService*>(this);

    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(Context::instance().sdkAppID()));
    if (Context::instance().isLogin())
    {
        url.append(L"&identifier=")
            .append(TXCHttpRequest::a2w(Context::instance().userId()))
            .append(L"&user_token=")
            .append(TXCHttpRequest::a2w(Context::instance().token()));
    }

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    QuitConfReq req(Context::instance().roomID());
    std::string body = req.GenReq();

    service->stopHeartBeat();

    ilive::GetILive()->quitRoom(oniLiveQuitRoomSuc, oniLiveQuitRoomErr, this);
    service->m_http->asyn_post(url, headers, body, [service](DWORD code, std::string& resp) {
		if (service->m_callback)
		{
			service->m_callback->onQuitRoom(code, resp.c_str());
		}
    });
}

void AccountService::destoryRoom()
{
	AccountService* service = reinterpret_cast<AccountService*>(this);

	std::wstring url = reqURLPrefix;
	url.append(std::to_wstring(Context::instance().sdkAppID()));
	if (Context::instance().isLogin())
	{
		url.append(L"&identifier=")
			.append(TXCHttpRequest::a2w(Context::instance().userId()))
			.append(L"&user_token=")
			.append(TXCHttpRequest::a2w(Context::instance().token()));
	}

	HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
	headers->set_content_type(L"application/json; charset=utf-8");

	DestroyConfReq req(Context::instance().roomID(),"");
	std::string body = req.GenReq();

	service->stopHeartBeat();

	ilive::GetILive()->quitRoom(oniLiveQuitRoomSuc, oniLiveQuitRoomErr, this);
	QTime tBefore = QTime::currentTime();
	service->m_http->asyn_post(url, headers, body, [service, tBefore](DWORD code, std::string& resp) {
		QTime tAfter = QTime::currentTime();
		txCloudEduLog_i("txcloud", "destoryRoom use = %d msec", tAfter.msec() - tBefore.msec());
		if (service->m_callback)
		{
			service->m_callback->onQuitRoom(code, resp.c_str());
		}
	});
}

void AccountService::exitService()
{
	logoutiLive();
	//ilive::GetILive()->release();
}

void AccountService::onIliveLoginSuc(void *data)
{
	IMService::shared_instance()->registerIM();
}

void AccountService::onIliveLoginErr(const int code, const char *desc, void* data)
{
	AccountService* service = reinterpret_cast<AccountService*>(data);
	txCloudEduLog_e("txcloud", "onIliveLoginErr, code = %d", code);
	QString str;
	str.sprintf("iLive login fail! code = %d", code);
	QDialogMessage::exec(str, QDialogMessage::OK);
	service->m_callback->onError();
}

void AccountService::onIliveLogoutSuc(void *data)
{
	AccountService* service = reinterpret_cast<AccountService*>(data);
	GetILive()->release();
	service->m_callback->onLogout(0,"");
}

void AccountService::onIliveLogoutErr(const int code, const char *desc, void* data)
{
	GetILive()->release();
}

void AccountService::oniLiveQuitRoomSuc(void* data)
{
	AccountService* service = reinterpret_cast<AccountService*>(data);
	//service->stopHeartBeat();
	service->m_callback->onQuitAVRoom();
	//service->logoutiLive();
}

void AccountService::oniLiveQuitRoomErr(int code, const char *desc, void* data)
{
	txCloudEduLog_e("txcloud", "oniLiveQuitRoomErr, code = %d", code);
}

void AccountService::onMemStatusChange(ilive::E_EndpointEventId event_id, const ilive::Vector<ilive::String> &ids, void* data)
{

}

void AccountService::onRoomDisconnect(int reason, const char *errorinfo, void* data)
{
	txCloudEduLog_e("txcloud", "onRoomDisconnect, reason = %d", reason);
	QDialogMessage::exec(QString::fromWCharArray(L"房间已断开"), QDialogMessage::OK);
}

void AccountService::oniLiveJoinRoomSuc(void* data)
{
    AccountService* service = reinterpret_cast<AccountService*>(data);

    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(Context::instance().sdkAppID()));
    if (Context::instance().isLogin())
    {
        url.append(L"&identifier=")
            .append(TXCHttpRequest::a2w(Context::instance().userId()))
            .append(L"&user_token=")
            .append(TXCHttpRequest::a2w(Context::instance().token()));
    }

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    ReportJoinReq req(Context::instance().roomID());
    std::string body = req.GenReq();
	QTime tBefore = QTime::currentTime();

    service->m_http->asyn_post(url, headers, body, [service, tBefore](DWORD code, std::string& resp) {
		QTime tAfter = QTime::currentTime();
		txCloudEduLog_i("txcloud", "ReportJoin use = %d msec", tAfter.msec() - tBefore.msec());
        if (200 != code)
        {
            if (service->m_callback)
            {
                service->m_callback->onReportJoinRoom(-1, "");
            }
        }
        else
        {
            ReportJoinRsp rsp;
            rsp.Parse(resp);
            if (0 == rsp.GetCode())
            {
				Context::instance().setHomeScreen(rsp.GetHomeScreen());
                service->startHeartBeat();
            }

            if (service->m_callback)
            {
                service->m_callback->onReportJoinRoom(rsp.GetCode(), rsp.GetMsg().c_str());
            }
        }
    });
}

void AccountService::oniLiveJoinRoomErr(int code, const char *desc, void* data)
{
	AccountService* service = reinterpret_cast<AccountService*>(data);
	txCloudEduLog_e("txcloud", "oniLiveJoinRoomErr, code = %d", code);
	QString str;
	str.sprintf("iLive join room fail! code = %d", code);
	QDialogMessage::exec(str, QDialogMessage::OK);
	service->m_callback->onError();
}

void AccountService::startHeartBeat()
{
    m_stop = false;
    m_thread = std::thread(&AccountService::onHeartBeat, this);
}

void AccountService::stopHeartBeat()
{
    m_stop = true;
    m_condition.notify_all();
	m_thread.join();
}

void AccountService::onHeartBeat()
{
    std::chrono::milliseconds interval(Context::instance().heartbeatInterval() * 1000);
    std::chrono::steady_clock::time_point fireTime = std::chrono::steady_clock::now() + interval;

    std::unique_lock<std::mutex> lock(m_heartBeatMutex);

    while (!m_stop && !m_condition.wait_until(lock, fireTime, [this] { return m_stop; }))
    {
        std::wstring url = reqURLPrefix;
        url.append(std::to_wstring(Context::instance().sdkAppID()));
        if (Context::instance().isLogin())
        {
            url.append(L"&identifier=")
                .append(TXCHttpRequest::a2w(Context::instance().userId()))
                .append(L"&user_token=")
                .append(TXCHttpRequest::a2w(Context::instance().token()));
        }

        HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
        headers->set_content_type(L"application/json; charset=utf-8");

        HeartBeatReq req(Context::instance().roomID());
        std::string body = req.GenReq();

        m_http->asyn_post(url, headers, body, [this](DWORD code, std::string& resp) {   // todo旧版Demo心跳也失败
            if (200 != code)
            {
                m_stop = true;
            }
            else
            {
                HeartBeatRsp rsp;
                rsp.Parse(resp);
                if (0 != rsp.GetCode())
                {
                    m_stop = true;
                }
            }
        });

        fireTime = std::chrono::steady_clock::now() + interval;
    }
	//m_heartBeatMutex.unlock();
	//m_condition.notify_all();
}

/**************************************************************************/
