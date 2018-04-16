/**************************************************************************
    Copyright:      Copyright ? 1998 - 2018 Tencent. All Rights Reserved
    Created:        2018-1-17 17:54:18
    Filename:       Context.cpp

    Description:
***************************************************************************/

#include "Context.h"

/**************************************************************************/

Context::Context()
    : m_sdkAppId(0)
    , m_accountType(0)
    , m_nickName("")
    , m_userId("")
    , m_userPwd("")
    , m_token("")
    , m_userSig("")
    , m_roomID(0)
    , m_roomSig("")
    , m_roomName("")
    , m_roomOwner("")
    , m_chatGroudID("")
    , m_whiteBoardGroudID("")
    , m_heartbeatInterval(30)
    , m_joinConfURL("")
	, m_homeScreen("camera")
{

}

Context::~Context()
{

}

Context& Context::instance()
{
	static Context instance;
	return instance;
}

int Context::sdkAppID() const
{
    return m_sdkAppId;
}

void Context::setSdkAppID(int appid)
{
	m_sdkAppId = appid;
}

int Context::accountType() const
{
    return m_accountType;
}

void Context::setAccountType(int type)
{
	m_accountType = type;
}

std::string Context::nickName() const
{
    return m_nickName;
}

void Context::setNickName(const std::string& name)
{
    m_nickName = name;
}

std::string Context::userId() const
{
    return m_userId;
}

void Context::setUserId(const std::string& id)
{
    m_userId = id;
}

std::string Context::userPwd() const
{
    return m_userPwd;
}

void Context::setUserPwd(const std::string& pwd)
{
    m_userPwd = pwd;
}

bool Context::isLogin() const
{
    return (false == m_token.empty());
}

std::string Context::token() const
{
    return m_token;
}

void Context::setToken(const std::string& tokenVal)
{
    m_token = tokenVal;
}

std::string Context::userSig() const
{
    return m_userSig;
}

void Context::setUserSig(const std::string& sigVal)
{
    m_userSig = sigVal;
}

bool Context::isEnterRoom() const
{
    return (0 != m_roomID && !m_roomSig.empty());
}

int Context::roomID() const
{
    return m_roomID;
}

void Context::setRoomID(int id)
{
    m_roomID = id;
}

std::string Context::roomSig() const
{
    return m_roomSig;
}

void Context::setRoomSig(const std::string& sig)
{
    m_roomSig = sig;
}

std::string Context::roomName() const
{
    return m_roomName;
}

void Context::setRoomName(const std::string& name)
{
    m_roomName = name;
}

std::string Context::roomOwner() const
{
    return m_roomOwner;
}

void Context::setRoomOwner(const std::string& owner)
{
    m_roomOwner = owner;
}

std::string Context::chatGroudID() const
{
    return m_chatGroudID;
}

void Context::setChatGroudID(const std::string& id)
{
    m_chatGroudID = id;
}

std::string Context::whiteBoardGroudID() const
{
    return m_whiteBoardGroudID;
}

void Context::setWhiteBoardGroudID(const std::string& id)
{
    m_whiteBoardGroudID = id;
}

int Context::heartbeatInterval() const
{
    return m_heartbeatInterval;
}

void Context::setHeartbeatInterval(int interval)
{
    m_heartbeatInterval = interval;
}

std::string Context::joinConfURL() const
{
    return m_joinConfURL;
}

void Context::setJoinConfURL(const std::string& url)
{
    m_joinConfURL = url;
}

std::string Context::homeScreen() const
{
	return m_homeScreen;
}

void Context::setHomeScreen(const std::string& screen)
{
	if (!screen.empty())
	{
		m_homeScreen = screen;
	}	
}

/**************************************************************************/
