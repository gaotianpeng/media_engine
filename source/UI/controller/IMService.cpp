#include "IMService.h"
#include "json.h"
#include "gzip.h"
#include "Context.h"
#include "avsdkTime.h"
#include "txCloudLog.h"

#define  WHITEBOARD_DATA_MAX 7000
#define  MAX_ID_LEN          100
#define  IM_TMP_BUF_SIZE     256

std::string getMessageElemData(void * handle, uint32_t(*getDataLen)(void *), int(*getData)(void *, char *, uint32_t *))
{
	std::string buffer;
	buffer.resize(getDataLen(handle) + 32);

	uint32_t len = buffer.size();

	getData(handle, (char*)buffer.c_str(), &len);
	buffer.resize(len);

	return buffer;
}

void IMService::onTIMNewMessage(TIMMessageHandle* handles, uint32_t msg_num, void* data)
{
	IMService* pIMService = (IMService *)data;
	if (pIMService) 
	{
		for (int msg_idx = 0; msg_idx < msg_num; msg_idx++)
		{
			TIMMessageHandle handle = handles[msg_idx];
			int elemCount = GetElemCount(handle);

			std::string offline_info_ext = getOfflineInfoExt(handle);
			if (offline_info_ext.size() >= 15 && offline_info_ext.substr(0, 15) == "TXWhiteBoardExt")
			{
				if (!pIMService->m_recvWBDataCallback)
				{
					continue;
				}
				bool gzip = (offline_info_ext != "TXWhiteBoardExt");
				getWBDataFromIM(handle, elemCount, gzip, pIMService);
			}
			else
			{
				if (!pIMService->m_recvMsgCallback)
				{
					continue;
				}
				getMsgFromIM(handle, elemCount, pIMService);
			}
		}
	}
}

void IMService::OnGetWBFileDataSuccess(const char * buf, uint32_t len, void * data)
{
	std::string str_data = std::string(buf, len);
	IMService* pIMService = reinterpret_cast<IMService*>(data);
	pIMService->onRecvWBData(str_data);
}

void IMService::OnGetWBFileDataError(int code, const char * desc, void * data)
{
	txCloudEduLog_i("txcloud", "OnGetWBFileDataError code = %d", code);
	IMService* pIMService = reinterpret_cast<IMService*>(data);
}

void onSendTIMMsgSuccess(void* data)
{
	SendTIMMsgSink * sink = (SendTIMMsgSink *)data;
	if (sink != NULL)
	{
		if (sink->send_msg_sink != NULL) sink->send_msg_sink->onSendMsg(0, NULL);

		DestroyConversation(sink->conv);
		delete sink;
	}
}

void onSendTIMMsgError(int code, const char* desc, void* data)
{
	SendTIMMsgSink * sink = (SendTIMMsgSink *)data;
	if (sink != NULL)
	{
		if (sink->send_msg_sink != NULL) sink->send_msg_sink->onSendMsg(code, desc);

		DestroyConversation(sink->conv);
		delete sink;
	}
}

void OnSendWBDataSuccess(void* data)
{
	SendWBDataSink * sink = (SendWBDataSink *)data;
	if (sink != NULL)
	{
		if (sink->send_wbdata_sink != NULL) sink->send_wbdata_sink->onSendWBData(0, NULL);

		DestroyConversation(sink->conv);
		delete sink;
	}
}

void OnSendWBDataError(int code, const char* desc, void* data)
{
	txCloudEduLog_i("txcloud", "OnSendWBDataError code = %d", code);
	SendWBDataSink * sink = (SendWBDataSink *)data;
	if (sink != NULL)
	{
		if (sink->send_wbdata_sink != NULL) sink->send_wbdata_sink->onSendWBData(code, desc);

		DestroyConversation(sink->conv);
		delete sink;
	}
}

IMService::IMService()
{ 
	//TIMSetMode(1);
	//TIMDisableStorage();
	//TIMInit();

	//GetILive()->setMessageCallBack(NULL, NULL); //用于防止ilive的消息回调覆盖掉IMService的回调
	//TIMMessageCB cb = { 0 };
	//cb.data = this;
	//cb.OnNewMessage = onTIMNewMessage;
	//TIMSetMessageCallBack(&cb);
}

IMService::~IMService()
{
	//TIMUninit();
}

IMService * IMService::shared_instance()
{
	static IMService * pIMService = new IMService;
	return pIMService;
}

void onIMLoginSuccess(void* data)
{
}

void onIMLoginError(int code, const char* desc, void* data)
{
}

void IMService::login(IMAccountInfo & IMInfo)
{
	TIMCommCB cb = { 0 };
	cb.data = this;
	cb.OnError = onIMLoginError;
	cb.OnSuccess = onIMLoginSuccess;

	TIMUserInfo user = { 0 };
	user.account_type = const_cast<char*>(IMInfo.accType.c_str());
	std::string strAppID = std::to_string(IMInfo.sdkAppID);
	user.app_id_at_3rd = const_cast<char*>(strAppID.c_str());
	user.identifier = const_cast<char*>(IMInfo.userID.c_str());

	TIMLogin(IMInfo.sdkAppID, &user, IMInfo.userSig.c_str(), &cb);
}

void onlogoutSuss(void *)
{
}

void onlogoutErr(int, const char *, void *)
{
}

TIMCommCB logoutCB;
void IMService::logout()
{
	logoutCB.data = 0;
	logoutCB.OnError = onlogoutErr;
	logoutCB.OnSuccess = onlogoutSuss;
	TIMLogout(&logoutCB);
}

void IMService::registerIM()
{
	GetILive()->setMessageCallBack(NULL, NULL); //用于防止ilive的消息回调覆盖掉IMService的回调
	TIMMessageCB cb = { 0 };
	cb.data = this;
	cb.OnNewMessage = onTIMNewMessage;
	TIMSetMessageCallBack(&cb);
}

void IMService::setRecvMsgCallBack(IMRecvMsgCallback * sink)
{
	m_recvMsgCallback = sink;
}

void IMService::setRecvWBDataCallBack(IMRecvWBDataCallback * sink)
{
	m_recvWBDataCallback = sink;
}

void IMService::setSendMsgCallBack(IMSendMsgCallBack * sink)
{
	m_sendMsgCallback = sink;
}

void IMService::setSendWBDataCallBack(IMSendWBDataCallBack * sink)
{
	m_sendWBDataCallback = sink;
}

void IMService::sendC2CTextMsg(const char * dstUser, const char * msg)
{
	TIMMessageHandle IMMsg = CreateTIMMessage();
	TIMMsgTextElemHandle elem = CreateMsgTextElem();

	SetContent(elem, msg);
	AddElem(IMMsg, elem);

	int ret = sendMsg(kCnvC2C, dstUser, IMMsg);

	DestroyElem(elem);
	DestroyTIMMessage(IMMsg);
}

void IMService::sendGroupTextMsg(const char * msg)
{
	if (Context::instance().chatGroudID().empty())
	{
		return;
	}
	TIMMessageHandle IMMsg = CreateTIMMessage();
	TIMMsgTextElemHandle elem = CreateMsgTextElem();

	SetContent(elem, msg);
	AddElem(IMMsg, elem);

	int ret = sendMsg(kCnvGroup, Context::instance().chatGroudID().c_str(), IMMsg);

	DestroyElem(elem);
	DestroyTIMMessage(IMMsg);
}

void IMService::sendC2CCustomMsg(const char * dstUser, const char * msg)
{
	TIMMessageHandle IMMsg = CreateTIMMessage();
	TIMMsgTextElemHandle elem = CreateCustomElemHandle();

	SetCustomElemData(elem, msg, strlen(msg));
	AddElem(IMMsg, elem);

	int ret = sendMsg(kCnvC2C, dstUser, IMMsg);

	DestroyElem(elem);
	DestroyTIMMessage(IMMsg);
}

void IMService::sendGroupCustomMsg(const char * msg)
{
	if (Context::instance().chatGroudID().empty())
	{
		return;
	}
	TIMMessageHandle IMMsg = CreateTIMMessage();
	TIMMsgTextElemHandle elem = CreateCustomElemHandle();

	SetCustomElemData(elem, msg, strlen(msg));
	AddElem(IMMsg, elem);

	int ret = sendMsg(kCnvGroup, Context::instance().chatGroudID().c_str(), IMMsg);

	DestroyElem(elem);
	DestroyTIMMessage(IMMsg);
}

void IMService::sendWhiteBoardData(const char * data, uint32_t length)
{
	if (Context::instance().whiteBoardGroudID().empty())
	{
		return;
	}

	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(data, value))
	{
		return;
	}

	Json::Value json;
	json["seq"] = m_seq++;
	json["timestamp"] = getAvsdkTime();
	json["value"] = value;
	Json::FastWriter writer;
	std::string sendData = writer.write(json);

	TIMConversationHandle conv = CreateConversation();
	TIMGetConversation(conv, kCnvGroup, Context::instance().whiteBoardGroudID().c_str());
	TIMMessageHandle msg = CreateTIMMessage();

	std::string infoStr = "TXWhiteBoardExt";

	txCloudEduLog_i("txcloud", "send whiteboard data size = %d", sendData.size());
	if (sendData.size() <= WHITEBOARD_DATA_MAX)
	{
		TIMMsgCustomElemHandle elem = CreateCustomElemHandle();
		SetCustomElemData(elem, sendData.c_str(), sendData.size());
		SetCustomElemExt(elem, infoStr.c_str(), infoStr.size());
		AddElem(msg, elem);
	}
	else
	{
		if (sendData.size() < 100 * 1024)
		{
			txCloudEduLog_d("txcloud", "%s", sendData.c_str());
		}
		//infoStr = "TXWhiteBoardExt_gzip_" + std::to_string(sendData.size());
		//sendData = gzip::compress(sendData);

		TIMMsgFileElemHandle elem = CreateFileElemHandle();
		SetFileElemData(elem, sendData.c_str(), sendData.size());
		SetFileElemFileName(elem, infoStr.c_str(), infoStr.size());
		AddElem(msg, elem);
	}

	TIMOfflineInfoHandle offline_info = CreateTIMTIMOfflineInfo();
	SetExt4TIMTIMOfflineInfo(offline_info, infoStr.c_str(), infoStr.size());
	SetMsgOfflinePushInfo(msg, offline_info);

	SendWBDataSink * cbsink = new SendWBDataSink();
	cbsink->conv = conv;
	cbsink->send_wbdata_sink = m_sendWBDataCallback;

	TIMCommCB cb;
	cb.data = cbsink;
	cb.OnSuccess = OnSendWBDataSuccess;
	cb.OnError = OnSendWBDataError;
	SendMsg(conv, msg, &cb);
}

int IMService::sendMsg(TIMConversationType type, const char * peer, TIMConversationHandle msg)
{
	TIMConversationHandle conv = CreateConversation();

	int iRet = TIMGetConversation(conv, type, peer);

	if (iRet != 0)
	{
		DestroyConversation(conv);
		return iRet;
	}

	SendTIMMsgSink * cbsink = new SendTIMMsgSink();
	cbsink->conv = conv;
	cbsink->send_msg_sink = m_sendMsgCallback;

	TIMCommCB cb = { 0 };
	cb.data = cbsink;
	cb.OnSuccess = onSendTIMMsgSuccess;
	cb.OnError = onSendTIMMsgError;

	SendMsg(conv, msg, &cb);

	return 0;
}

void IMService::onRecvWBData(const std::string& data)
{
	Json::Reader reader;
	Json::FastWriter writer;
	Json::Value json;
	if (reader.parse(data, json))
	{
		const Json::Value value = json["value"];
		const std::string str_data = writer.write(value);
		if(m_recvWBDataCallback)
		{
			m_recvWBDataCallback->onRecvWhiteBoardData(str_data.c_str(), str_data.size());
		}
	}
}

std::string IMService::getOfflineInfoExt(TIMMessageHandle handle)
{
	TIMOfflineInfoHandle offline_info = CreateTIMTIMOfflineInfo();
	GetMsgOfflinePushInfo(handle, offline_info);

	char tmp_buf[IM_TMP_BUF_SIZE] = { 0 };
	uint32_t len = IM_TMP_BUF_SIZE;
	GetExt4TIMTIMOfflineInfo(offline_info, tmp_buf, &len);
	std::string offline_info_ext(tmp_buf);
	return offline_info_ext;
}

void IMService::getWBDataFromIM(TIMMessageHandle handle, int elemCount, bool gzip, IMService* pIMService)
{
	for (int i = 0; i < elemCount; i++)
	{
		auto elem = GetElem(handle, i);
		auto type = GetElemType(elem);
		std::string str_data;
		if (type == kElemCustom) {
			str_data = getMessageElemData(elem, GetCustomElemDataLen, GetCustomElemData);

			//if (gzip)
			//{
			//	str_data = gzip::decompress(str_data);
			//}
			pIMService->onRecvWBData(str_data);
		}
		else if (type == kElemFile)
		{
			TIMGetMsgDataCB cb;
			cb.OnSuccess = OnGetWBFileDataSuccess;
			cb.OnError = OnGetWBFileDataError;
			cb.data = pIMService;
			GetFileFromFileElem(elem, &cb);
		}
	}
}

void IMService::getMsgFromIM(TIMMessageHandle handle, int elemCount, IMService * pIMService)
{
	char group_id[MAX_ID_LEN] = { 0 };
	uint32_t group_id_len = MAX_ID_LEN - 1;
	char user_id[MAX_ID_LEN] = { 0 };
	uint32_t user_id_len = MAX_ID_LEN - 1;

	TIMConversationHandle conv = CreateConversation();

	GetMsgSender(handle, user_id, &user_id_len);
	user_id[user_id_len] = 0;

	GetConversationFromMsg(conv, handle);

	for (int i = 0; i < elemCount; i++)
	{
		auto elem = GetElem(handle, i);
		auto type = GetElemType(elem);
		switch (type)
		{
		case kElemText:
		{
			std::string text_msg = getMessageElemData(elem, GetContentLen, GetContent);

			TIMConversationType convType = GetConversationType(conv);
			group_id[0] = 0;
			if (convType == kCnvGroup)
			{
				GetConversationPeer(conv, group_id, &group_id_len);
				group_id[group_id_len] = 0;
				pIMService->m_recvMsgCallback->onRecvGroupTextMsg(group_id, user_id, text_msg.c_str());
			}
			else if (convType == kCnvC2C)
			{
				pIMService->m_recvMsgCallback->onRecvC2CTextMsg(user_id, text_msg.c_str());
			}
		}
			break;
		case kElemCustom:
		{
			std::string text_msg = getMessageElemData(elem, GetCustomElemDataLen, GetCustomElemData);

			TIMConversationType convType = GetConversationType(conv);
			group_id[0] = 0;
			if (convType == kCnvGroup)
			{
				GetConversationPeer(conv, group_id, &group_id_len);
				group_id[group_id_len] = 0;
				pIMService->m_recvMsgCallback->onRecvGroupCustomMsg(group_id, user_id, text_msg.c_str());
			}
			else if (convType == kCnvC2C)
			{
				pIMService->m_recvMsgCallback->onRecvC2CCustomMsg(user_id, text_msg.c_str());
			}
		}
			break;
		case kElemGroupReport:
		{
			E_TIM_GROUP_SYSTEM_TYPE sys_type = GetGroupReportType(elem);
			if (sys_type == TIM_GROUP_SYSTEM_CUSTOM_INFO)
			{
				GetGroupReportID(elem, group_id, &group_id_len);
				group_id[group_id_len] = 0;

				std::string text_msg = getMessageElemData(elem, GetGroupReportUserDataLen, GetGroupReportUserData);

				pIMService->m_recvMsgCallback->onRecvGroupSystemMsg(group_id, text_msg.c_str());
			}
		}
			break;
		default:
			break;
		}
	}
	DestroyConversation(conv);
}
