#pragma once

#include <string>
#include <vector>
#include "commonType.h"
#include "json.h"

class ConfRsp
{
public:
	ConfRsp() : error_code_(-1), error_msg_("")
	{
	}

	ConfRsp(const int code, const std::string& msg) : error_code_(code), error_msg_(msg)
	{
	}

	int GetCode() const;
	std::string GetMsg() const;
	void* GetData() const;
	virtual bool Parse(const std::string& buf) = 0;

	virtual ~ConfRsp()
	{
	}

	void* data_;
protected:
	int error_code_;
	std::string error_msg_;
};


class RegisterRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
};

class LoginRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
	std::string GetSig() const;
	std::string GetToken() const;
	std::string GetId() const;
	void SetId(const std::string& id);
private:
	std::string sig_;
	std::string token_;
	std::string id_;
};

class CreateConfRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
    std::string GetChatGroupId() const;
    std::string GetBoardGroupId() const;
    int GetRoomId() const;
    std::string GetRoomToken() const;
    std::string GetRoomName() const;
    std::string GetRoomOwner() const;
    int GetHeartbeat() const;
    int GetInterval() const;
    std::string GetInviteUrl() const;
private:
    std::string chat_group_id_;
    std::string white_board_group_id_;
    int room_id_;
    std::string room_token_;
    std::string room_name_;
    std::string room_owner_;
    int heartbeat_interval_;
    std::string join_conf_url_;
};

class ReportJoinRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;

	std::string GetHomeScreen() const;
private:
	std::string home_screen_;
};

class JoinConfRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
	std::string GetChatGroupId() const;
	std::string GetBoardGroupId() const;
	int GetRoomId() const;
    std::string GetRoomToken() const;
    std::string GetRoomName() const;
    std::string GetRoomOwner() const;
	int GetHeartbeat() const;
	int GetInterval() const;
	std::string GetInviteUrl() const;
	std::string GetHomeScreen() const;
private:
	std::string chat_group_id_;
	std::string white_board_group_id_;
	int room_id_;
    std::string room_token_;
    std::string room_name_;
    std::string room_owner_;
	int heartbeat_interval_;
	std::string join_conf_url_;
	std::string home_screen_;
};

class RoomListRsp : public ConfRsp
{
public:
    virtual bool Parse(const std::string& buf) override;
    std::vector<RoomItem> GetRoomList() const;
private:
    std::vector<RoomItem> room_list_;
};

class HeartBeatRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
};


class ModifyMemInfoRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
};

class ModifyConfInfoRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
};


class ReportWhiteBoardRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
};


class GetWhiteBoardRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;

	Json::Value& GetBoardDataList();
private:
	Json::Value board_data_list_;
};

class StartRecordRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
};

class StopRecordRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
};

class CosSigRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
	std::string GetSig() const;
	std::string GetBucket() const;
	std::string GetRegion() const;
private:
	std::string sig_;
	std::string bucket_;
	std::string region_;
};

class QuitConfRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
};

class DestroyConfRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string& buf) override;
};

class DownloadBoardMessageRsp : public ConfRsp
{
public:
	virtual bool Parse(const std::string &buf) override;
	const std::string getBoardString();
private:
	std::string board_string_;
};

class GetMemberListRsp : public ConfRsp
{
public:
    GetMemberListRsp() : total_(0), member_list_() {}
    virtual bool Parse(const std::string &buf) override;
    int getTotal() const;
    std::vector<MemberItem> getMemberList() const;
private:
    int total_;
    std::vector<MemberItem> member_list_;
};

class InviteInteractRsp : public ConfRsp
{
public:
    InviteInteractRsp() {}
    virtual bool Parse(const std::string &buf) override;
};

class ApplyInteractRsp : public ConfRsp
{
public:
    ApplyInteractRsp() {}
    virtual bool Parse(const std::string &buf) override;
};

class GrandPermissionRsp : public ConfRsp
{
public:
    GrandPermissionRsp() {}
    virtual bool Parse(const std::string &buf) override;
};
