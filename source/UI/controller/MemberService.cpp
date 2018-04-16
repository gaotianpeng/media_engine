#include "MemberService.h"
#include "TXCHttpClient.h"
#include "json.h"
#include "ConfReq.h"
#include "ConfRsp.h"

#include <assert.h>

static const wchar_t* reqURLPrefix = L"https://sxb.qcloud.com/conf_svr_sdk/conference_server/public/api/conference?sdkappid=";

MemberService::MemberService(const AuthInfo& info)
    : m_authInfo(info)
    , m_callback(nullptr)
    , m_http(new TXCHttpClient(L"TXCloudEdu"))
{

}

MemberService::~MemberService()
{
    m_callback = nullptr;
}

void MemberService::setCallback(IMemberService* callback)
{
    m_callback = callback;
}

std::string MemberService::nickName(const std::string& userId)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    for (std::vector<MemberItem>::iterator it = m_memberList.begin(); m_memberList.end() != it; ++it)
    {
        if (userId == it->id)
        {
            return it->nickName;
        }
    }

    return "";
}

int MemberService::queryMember(const std::string& userId, MemberItem& member)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	for (std::vector<MemberItem>::iterator it = m_memberList.begin(); m_memberList.end() != it; ++it)
	{
		if (userId == it->id)
		{
			member = *it;
			return 0;
		}
	}
	return -1;
}

std::vector<MemberItem> MemberService::memberList()
{
    std::unique_lock<std::mutex> lock(m_mutex);

	return m_memberList;
}

std::vector<VideoHandsupItem> MemberService::videoList()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	return m_videoList;
}

void MemberService::addMember(const MemberItem& item)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    m_memberList.push_back(item);
}

void MemberService::removeMember(const std::string& userId)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    for (std::vector<MemberItem>::iterator it = m_memberList.begin(); m_memberList.end() != it; ++it)
    {
        if (userId == it->id)
        {
            m_memberList.erase(it);
            break;
        }
    }

	for (std::vector<VideoHandsupItem>::iterator iter = m_videoList.begin(); m_videoList.end() != iter; ++iter)
	{
		if (userId == iter->id)
		{
			m_videoList.erase(iter);
			break;
		}
	}
}

bool MemberService::modifyMember(const std::map<std::string, ModifyMemberInfoItem>& items)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    bool modify = false;
    for (std::vector<MemberItem>::iterator it = m_memberList.begin(); m_memberList.end() != it; )
    {
        std::map<std::string, ModifyMemberInfoItem>::const_iterator findIt = items.find(it->id);
        if (items.end() != findIt)
        {
			if (findIt->second.status == 0)
			{
				it = m_memberList.erase(it);
				modify = true;
				continue;
			}
            else if (it->mic != findIt->second.mic
                || it->camera != findIt->second.camera
                || it->screen != findIt->second.screen
				|| it->video != findIt->second.video)
            {
                it->mic = findIt->second.mic;
                it->camera = findIt->second.camera;
                it->screen = findIt->second.screen;
				it->video = findIt->second.video;

                modify = true;
            }
        }
		++it;
    }

    return modify;
}

void MemberService::modifyMemberList(const std::string& userId, ModifyMemberInfoItem& member)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	for (int i = 0; i < m_memberList.size(); i++)
	{
		if (userId == m_memberList[i].id)
		{
			MemberItem item = m_memberList[i];
			item.camera = member.camera;
			item.screen = member.screen;
			item.video = member.video;
			item.mic = member.mic;
			m_memberList[i] = item;
			break;
		}
	}
}

int MemberService::queryVideoMember(const std::string& userId)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	for (std::vector<VideoHandsupItem>::iterator it = m_videoList.begin(); m_videoList.end() != it; ++it)
	{
		if (userId == it->id)
		{
			return 0;
		}
	}
	return -1;
}

void MemberService::addVideo(const MemberItem& item)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	VideoHandsupItem newitem;
	newitem.id = item.id;
	newitem.nickName = item.nickName;
	newitem.mic = item.mic;
	newitem.camera = item.camera;
	newitem.screen = item.screen;
	newitem.video = item.video;
	newitem.status = STATUS_HANDING;

	m_videoList.push_back(newitem);
}

void MemberService::addVideo(const VideoHandsupItem& item)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_videoList.push_back(item);
}

void MemberService::removeVideo(const std::string& userId)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	for (std::vector<VideoHandsupItem>::iterator it = m_videoList.begin(); m_videoList.end() != it; ++it)
	{
		if (userId == it->id)
		{
			m_videoList.erase(it);
			break;
		}
	}
}

void MemberService::modifyVideoList(const std::string& userId, int status)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	for (int i = 0; i < m_videoList.size(); i++)
	{
		if (userId == m_videoList[i].id)
		{
			VideoHandsupItem member = m_videoList[i];
			member.status = status;
			if (status == STATUS_ONSTAGE)
			{
				member.camera = true;
				member.mic = true;
			}
			m_videoList[i] = member;
			break;
		}
	}
}

void MemberService::refreshVideoList(const std::vector<MemberItem>& memberList)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_videoList.clear();
	for (int i = 0; i < memberList.size(); i++)
	{
		MemberItem item = memberList[i];
		if (!item.status)
		{
			continue;
		}
		if (item.camera || item.screen || item.mic || item.video)
		{
			VideoHandsupItem newitem;
			newitem.id = item.id;
			newitem.nickName = item.nickName;
			newitem.mic = item.mic;
			newitem.camera = item.camera;
			newitem.screen = item.screen;
			newitem.video = item.video;
			newitem.status = STATUS_ONSTAGE;
			m_videoList.push_back(newitem);
		}
	}
}

void MemberService::refreshVideoList()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_videoList.clear();
	for (int i = 0; i < m_memberList.size(); i++)
	{
		MemberItem item = m_memberList[i];
		if (!item.status)
		{
			continue;
		}
		if (item.camera || item.screen || item.mic || item.video)
		{
			VideoHandsupItem newitem;
			newitem.id = item.id;
			newitem.nickName = item.nickName;
			newitem.mic = item.mic;
			newitem.camera = item.camera;
			newitem.screen = item.screen;
			newitem.video = item.video;
			newitem.status = STATUS_ONSTAGE;
			m_videoList.push_back(newitem);
		}
	}
}

void MemberService::refreshVideoList(const std::map<std::string, ModifyMemberInfoItem> items)
{
	for (std::map<std::string, ModifyMemberInfoItem>::const_iterator it = items.begin(); it != items.end(); ++it)
	{
		ModifyMemberInfoItem item = it->second;
		if (item.status == 0)
		{
			removeVideo(it->first);
		}
		else if (it->second.camera || it->second.screen || it->second.mic || it->second.video)
		{
			if (queryVideoMember(it->first))
			{
				VideoHandsupItem newitem;
				newitem.id = item.id;
				newitem.nickName = nickName(item.id);
				newitem.mic = item.mic;
				newitem.camera = item.camera;
				newitem.screen = item.screen;
				newitem.video = item.video;
				newitem.status = STATUS_ONSTAGE;
				addVideo(newitem);
			}
			else
			{
				modifyVideoList(it->first, STATUS_ONSTAGE);
			}
		}
		else
		{
			removeVideo(it->first);
		}

	}

	//for (std::vector<VideoHandsupItem>::iterator it = m_videoList.begin(); m_videoList.end() != it; ++it)
	//{
	//	std::map<std::string, ModifyMemberInfoItem>::const_iterator findIt = items.find(it->id);
	//	if (items.end() != findIt)
	//	{
	//		it->mic = findIt->second.mic;
	//		it->camera = findIt->second.camera;
	//		it->screen = findIt->second.screen;
	//		it->video = findIt->second.video;
	//		if (it->mic || it->camera || it->screen || it->video)
	//		{
	//			it->status = STATUS_ONSTAGE;
	//		}
	//	}
	//}
}

void MemberService::queryMemberList()
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(m_authInfo.m_sdkAppId));
    url.append(L"&identifier=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_userId))
        .append(L"&user_token=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_token));

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    GetMemberListReq req(m_authInfo.m_roomID);
    std::string body = req.GenReq();

    m_http->asyn_post(url, headers, body, [this](DWORD code, std::string& resp) {
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onGetMemberList(0, std::vector<MemberItem>());
            }
        }
        else
        {
            GetMemberListRsp rsp;
            rsp.Parse(resp);

            std::vector<MemberItem> memberList = rsp.getMemberList();

            if (m_callback)
            {
                m_callback->onGetMemberList(rsp.getTotal(), memberList);
            }

            std::unique_lock<std::mutex> lock(m_mutex);
            m_memberList.clear();
            m_memberList.insert(m_memberList.begin(), memberList.begin(), memberList.end());
			
        }
    });
}

void MemberService::modifyMemberInfo(const std::vector<ModifyMemberInfoItem>& infos)
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(m_authInfo.m_sdkAppId));
    url.append(L"&identifier=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_userId))
        .append(L"&user_token=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_token));

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    ModifyMemInfoReq req(m_authInfo.m_roomID, infos);
    std::string body = req.GenReq();

    m_http->asyn_post(url, headers, body, [this](DWORD code, std::string& resp) {
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onModifyMemberInfo(-1, "");
            }
        }
        else
        {
            ModifyMemInfoRsp rsp;
            rsp.Parse(resp);

            if (m_callback)
            {
                m_callback->onModifyMemberInfo(rsp.GetCode(), rsp.GetMsg().c_str());
            }
        }
    });
}

void MemberService::modifyConfInfo(const std::string& home_screen)
{
	std::wstring url = reqURLPrefix;
	url.append(std::to_wstring(m_authInfo.m_sdkAppId));
	url.append(L"&identifier=")
		.append(TXCHttpRequest::a2w(m_authInfo.m_userId))
		.append(L"&user_token=")
		.append(TXCHttpRequest::a2w(m_authInfo.m_token));

	HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
	headers->set_content_type(L"application/json; charset=utf-8");

	ModifyConfInfoReq req(m_authInfo.m_roomID, home_screen);
	std::string body = req.GenReq();

	m_http->asyn_post(url, headers, body, [this](DWORD code, std::string& resp) {
		if (200 != code)
		{
			if (m_callback)
			{
				m_callback->onModifyConfInfo(-1, "");
			}
		}
		else
		{
			ModifyConfInfoRsp rsp;
			rsp.Parse(resp);

			if (m_callback)
			{
				m_callback->onModifyConfInfo(rsp.GetCode(), rsp.GetMsg().c_str());
			}
		}
	});
}

void MemberService::inviteInteract(const std::string& userId, int permission)
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(m_authInfo.m_sdkAppId));
    url.append(L"&identifier=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_userId))
        .append(L"&user_token=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_token));

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    InviteInteractReq req(m_authInfo.m_roomID, userId, permission);
    std::string body = req.GenReq();

    m_http->asyn_post(url, headers, body, [this, userId](DWORD code, std::string& resp) {
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onInviteInteract(-1, "", userId);
            }
        }
        else
        {
            InviteInteractRsp rsp;
            rsp.Parse(resp);

            if (m_callback)
            {
                m_callback->onInviteInteract(rsp.GetCode(), rsp.GetMsg().c_str(), userId);
            }
        }
    });
}

void MemberService::applyInteract(int permission)
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(m_authInfo.m_sdkAppId));
    url.append(L"&identifier=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_userId))
        .append(L"&user_token=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_token));

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    ApplyInteractReq req(m_authInfo.m_roomID, permission);
    std::string body = req.GenReq();

    m_http->asyn_post(url, headers, body, [this](DWORD code, std::string& resp) {
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onApplyInteract(-1, "");
            }
        }
        else
        {
            ApplyInteractRsp rsp;
            rsp.Parse(resp);

            if (m_callback)
            {
                m_callback->onApplyInteract(rsp.GetCode(), rsp.GetMsg().c_str());
            }
        }
    });
}

void MemberService::grandPermission(const std::string& userId, int permission)
{
    std::wstring url = reqURLPrefix;
    url.append(std::to_wstring(m_authInfo.m_sdkAppId));
    url.append(L"&identifier=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_userId))
        .append(L"&user_token=")
        .append(TXCHttpRequest::a2w(m_authInfo.m_token));

    HttpHeadersPtr headers = std::make_shared<HttpHeaders>();
    headers->set_content_type(L"application/json; charset=utf-8");

    GrandPermissionReq req(m_authInfo.m_roomID, userId, permission);
    std::string body = req.GenReq();

    m_http->asyn_post(url, headers, body, [this](DWORD code, std::string& resp) {
        if (200 != code)
        {
            if (m_callback)
            {
                m_callback->onGrandPermission(-1, "");
            }
        }
        else
        {
            GrandPermissionRsp rsp;
            rsp.Parse(resp);

            if (m_callback)
            {
                m_callback->onGrandPermission(rsp.GetCode(), rsp.GetMsg().c_str());
            }
        }
    });
}
