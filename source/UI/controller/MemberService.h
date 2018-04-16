#ifndef __MEMBERSERVICE_H__
#define __MEMBERSERVICE_H__

#include "TXCHttpClient.h"
#include "ConfRsp.h"
#include "commonType.h"

/**************************************************************************/

class IMemberService
{
public:
    virtual void onGetMemberList(int total, const std::vector<MemberItem>& member_list) = 0;
    virtual void onModifyMemberInfo(int code, const char *desc) = 0;
	virtual void onModifyConfInfo(int code, const char *desc) = 0;
    virtual void onInviteInteract(int code, const char *desc, const std::string& userId) = 0;
    virtual void onApplyInteract(int code, const char *desc) = 0;
    virtual void onGrandPermission(int code, const char *desc) = 0;
};

class MemberService
{
public:
    MemberService(const AuthInfo& info);
    ~MemberService();

    void setCallback(IMemberService* callback);

    std::string nickName(const std::string& userId);
	int queryMember(const std::string& userId, MemberItem& member);
    std::vector<MemberItem> memberList();
    void addMember(const MemberItem& item);
    void removeMember(const std::string& userId);
    bool modifyMember(const std::map<std::string, ModifyMemberInfoItem>& items);
	void modifyMemberList(const std::string& userId, ModifyMemberInfoItem& member);
    void queryMemberList();
    void modifyMemberInfo(const std::vector<ModifyMemberInfoItem>& infos);

    // 权限InteractPermission组合，对端从IM得到Notify
    void inviteInteract(const std::string& userId, int permission);
    void applyInteract(int permission);
    void grandPermission(const std::string& userId, int permission);
	void modifyConfInfo(const std::string& home_screen);

	//视频位列表操作
	std::vector<VideoHandsupItem> videoList();
	int queryVideoMember(const std::string& userId);
	void addVideo(const MemberItem& item);
	void addVideo(const VideoHandsupItem& item);
	void removeVideo(const std::string& userId);
	void modifyVideoList(const std::string& userId, int status);
	void refreshVideoList(const std::map<std::string, ModifyMemberInfoItem> items);
	void refreshVideoList(const std::vector<MemberItem>& memberList);
	void refreshVideoList();
private:
    AuthInfo m_authInfo;
    std::vector<MemberItem> m_memberList;
	std::vector<VideoHandsupItem> m_videoList;
    std::mutex m_mutex;

    IMemberService* m_callback;
    std::shared_ptr<TXCHttpClient> m_http;
};

#endif /* __MEMBERSERVICE_H__ */
