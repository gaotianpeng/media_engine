/**************************************************************************
    Copyright:      Copyright ? 1998 - 2018 Tencent. All Rights Reserved
    Created:        2018-1-17 17:54:12
    Filename:       Context.h

    Description:
***************************************************************************/

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <string>

/**************************************************************************/

class User;

// 帐号，房间等信息集合，为了解耦，主界面不要引用
class Context
{
public:
	static Context& instance();

    int sdkAppID() const;
    void setSdkAppID(int appid);

    int accountType() const;
    void setAccountType(int type);

    std::string nickName() const;
    void setNickName(const std::string& name);

    std::string userId() const;
    void setUserId(const std::string& id);

    std::string userPwd() const;
    void setUserPwd(const std::string& pwd);

    bool isLogin() const;
    std::string token() const;
    void setToken(const std::string& tokenVal);

    std::string userSig() const;
    void setUserSig(const std::string& sigVal);

    bool isEnterRoom() const;

    int roomID() const;
    void setRoomID(int id);

    std::string roomSig() const;
    void setRoomSig(const std::string& sig);

    std::string roomName() const;
    void setRoomName(const std::string& name);

    std::string roomOwner() const;
    void setRoomOwner(const std::string& owner);

    std::string chatGroudID() const;
    void setChatGroudID(const std::string& id);

    std::string whiteBoardGroudID() const;
    void setWhiteBoardGroudID(const std::string& id);

    int heartbeatInterval() const;
    void setHeartbeatInterval(int interval);

    std::string joinConfURL() const;
    void setJoinConfURL(const std::string& url);

	std::string homeScreen() const;
	void setHomeScreen(const std::string& screen);
private:
	Context();
	~Context();

    int m_sdkAppId;
    int m_accountType;
    std::string m_nickName;
    std::string m_userId;
    std::string m_userPwd;
    std::string m_token;
    std::string m_userSig;

    int m_roomID;
    std::string m_roomSig;
    std::string m_roomName;
    std::string m_roomOwner;

    std::string m_chatGroudID;
    std::string m_whiteBoardGroudID;
    int m_heartbeatInterval;
    std::string m_joinConfURL;
	std::string m_homeScreen;
};

/**************************************************************************/
#endif /* __CONTEXT_H__ */
