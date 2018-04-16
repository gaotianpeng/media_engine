/**************************************************************************
    Copyright:      Copyright ? 1998 - 2018 Tencent. All Rights Reserved
    Created:        2018-1-17 16:59:55
    Filename:       AccountService.h

    Description:
***************************************************************************/

#ifndef __COURTENTRYSERVICE_H__
#define __COURTENTRYSERVICE_H__

#include "TXCHttpClient.h"
#include "iLive.h"
#include "commonType.h"

#include <string>
#include <memory>
#include <vector>

/**************************************************************************/

class Context;
class TXCHttpClient;

class IAccountService
{
public:
    virtual void onQueryRoomList(int code, const char *desc, const std::vector<RoomItem>& roomList) = 0;
    virtual void onRegister(int code, const char *desc) = 0;
    virtual void onLogin(int code, const char *desc) = 0;
    virtual void onLogout(int code, const char *desc) = 0;
    virtual void onCreateRoom(int code, const char *desc) = 0;
    virtual void onJoinRoom(int code, const char *desc) = 0;
    virtual void onReportJoinRoom(int code, const char *desc) = 0;
    virtual void onQuitRoom(int code, const char *desc) = 0;
	virtual void onQuitAVRoom() = 0;
	virtual void onError() = 0;
};

class AccountService
{
public:
    AccountService(int sdkAppId, int accountType);
    ~AccountService();

    void setCallback(IAccountService* callback);

    void queryRoomList();

    void registerAccount(const std::string& id, const std::string& pwd);
    void loginAccount(const std::string& id, const std::string& pwd);
    void loginiLive();
	void logoutiLive();
    void logoutAccount();

    void createRoom(const std::string& courtName, const std::string& pwd, const std::string& nickName);
    void createiLiveRoom();   // 要求在主线程运行
    void joinRoom(int confID, const std::string& pwd, const std::string& nickName);
    void joiniLiveRoom();   // 要求在主线程运行
    void quitRoom();
	void destoryRoom();
	void exitService();
private:
    static void onIliveLoginSuc(void *data);
    static void onIliveLoginErr(const int code, const char *desc, void* data);
	static void onIliveLogoutSuc(void *data);
	static void onIliveLogoutErr(const int code, const char *desc, void* data);
    static void oniLiveQuitRoomSuc(void* data);
    static void oniLiveQuitRoomErr(int code, const char *desc, void* data);
    static void onMemStatusChange(ilive::E_EndpointEventId event_id, const ilive::Vector<ilive::String> &ids, void* data);
    static void onRoomDisconnect(int reason, const char *errorinfo, void* data);
    static void oniLiveJoinRoomSuc(void* data);
    static void oniLiveJoinRoomErr(int code, const char *desc, void* data);

    void startHeartBeat();
    void stopHeartBeat();
    void onHeartBeat();
private:
    IAccountService* m_callback;
    std::shared_ptr<TXCHttpClient> m_http;

    bool m_stop;
    std::mutex m_heartBeatMutex;
    std::condition_variable m_condition;
    std::thread m_thread;
};

/**************************************************************************/
#endif /* __COURTENTRYSERVICE_H__ */
