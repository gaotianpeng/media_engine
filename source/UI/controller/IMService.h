#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "iLive.h"
#include "lock/TXCLock.h"
#include "tim_c.h"

using namespace ilive;

struct IMAccountInfo
{
	std::string  userID;
	std::string  userSig;
	int  sdkAppID;
	std::string  accType;
};

class IMRecvMsgCallback
{
public:
	virtual ~IMRecvMsgCallback() = default;
	virtual void onRecvC2CTextMsg(const char * userID, const char * msg) = 0;
	virtual void onRecvGroupTextMsg(const char * groupID, const char * userID, const char * msg) = 0;

	virtual void onRecvC2CCustomMsg(const char * userID, const char * msg) = 0;
	virtual void onRecvGroupCustomMsg(const char * groupID, const char * userID, const char * msg) = 0;

	virtual void onRecvGroupSystemMsg(const char * groupID, const char * msg) = 0;
};

class IMRecvWBDataCallback
{
public:
	virtual ~IMRecvWBDataCallback() = default;
	virtual void onRecvWhiteBoardData(const char *data, uint32_t length) = 0;
};

class IMSendMsgCallBack
{
public:
	virtual void onSendMsg(int err, const char * errMsg) = 0;
};

class IMSendWBDataCallBack
{
public:
	virtual void onSendWBData(int err, const char * errMsg) = 0;
};

typedef struct tag_SendTIMMsgSink
{
	TIMConversationHandle  conv;
	IMSendMsgCallBack *    send_msg_sink;
}SendTIMMsgSink;

typedef struct tag_SendWBDataSink
{
	TIMConversationHandle  conv;
	IMSendWBDataCallBack * send_wbdata_sink;
}SendWBDataSink;

class IMService
{
public:
	IMService();
	virtual ~IMService();

	static IMService * shared_instance();


	void registerIM();

	void setRecvMsgCallBack(IMRecvMsgCallback * sink);
	void setRecvWBDataCallBack(IMRecvWBDataCallback * sink);
	void setSendMsgCallBack(IMSendMsgCallBack * sink);
	void setSendWBDataCallBack(IMSendWBDataCallBack * sink);

	void sendC2CTextMsg(const char * dstUser, const char * msg);
	void sendGroupTextMsg(const char * msg);

	void sendC2CCustomMsg(const char * dstUser, const char * msg);
	void sendGroupCustomMsg(const char * msg);

	void sendWhiteBoardData(const char* data, uint32_t length);

public:

private:
	void login(IMAccountInfo & IMInfo);
	void logout();
	int sendMsg(TIMConversationType type, const char * peer, TIMConversationHandle msg);
	void onRecvWBData(const std::string& data);
	static void onTIMNewMessage(TIMMessageHandle* handles, uint32_t msg_num, void* data);
	static void OnGetWBFileDataSuccess(const char* buf, uint32_t len, void* data);
	static void OnGetWBFileDataError(int code, const char* desc, void* data);
	static std::string getOfflineInfoExt(TIMMessageHandle  handle);
	static void getWBDataFromIM(TIMMessageHandle  handle, int elemCount, bool gzip, IMService* pIMService);
	static void getMsgFromIM(TIMMessageHandle  handle, int elemCount, IMService* pIMService);

	IMSendMsgCallBack * m_sendMsgCallback = nullptr;
	IMSendWBDataCallBack * m_sendWBDataCallback = nullptr;
	IMRecvMsgCallback * m_recvMsgCallback = nullptr;
	IMRecvWBDataCallback * m_recvWBDataCallback = nullptr;
	unsigned int m_seq = 1;
};