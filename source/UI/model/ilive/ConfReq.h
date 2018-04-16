#pragma once

#include <string>
#include <vector>
#include <map>
#include "commonType.h"
#include "json.h"

class ConfReq
{
public:
	ConfReq(const std::string &cmd, const std::string &subcmd, const bool verify) : cmd_(cmd), subcmd_(subcmd), verify_(verify) {}
	std::string GetCmd() const;
	std::string GetSubCmd() const;
	bool GetVerify() const;
	virtual std::string GenReq() const = 0;
	virtual ~ConfReq() {}
protected:
	std::string cmd_;
	std::string subcmd_;
	bool verify_;
};


class ConfListReq : public ConfReq
{
public:
	ConfListReq(const std::string &id, const std::string &key, const unsigned index, const unsigned size)
		: ConfReq("open_conf_svc", "query_conf", true), id_(id), key_(key), index_(index), size_(size) {}
	virtual std::string GenReq() const override;
private:
	std::string id_;
	std::string key_;
	unsigned index_;
	unsigned size_;
};


class RegisterReq : public ConfReq
{
public:
	RegisterReq(const std::string &id, const std::string &password)
		: ConfReq("open_account_svc", "register", false), id_(id), password_(password) {}
	virtual std::string GenReq() const override;
private:
	std::string id_;
	std::string password_;
};


class LoginReq : public ConfReq
{
public:
	LoginReq(const std::string &id, const std::string &password)
		: ConfReq("open_account_svc", "verify", false), id_(id), password_(password) {}
	virtual std::string GenReq() const override;
private:
	std::string id_;
	std::string password_;
};


class CreateConfReq : public ConfReq
{
public:
	CreateConfReq(const std::string &name, const std::string &password)
		: ConfReq("open_conf_svc", "create_conf", true), conf_name_(name), conf_psw_(password), home_screen_("camera"){}
	virtual std::string GenReq() const override;
private:
	std::string conf_name_;
	std::string conf_psw_;
	std::string home_screen_;
};


class JoinConfReq : public ConfReq
{
public:
	JoinConfReq(const int conf_id, const std::string &conf_psw, const std::string &nick)
		: ConfReq("open_conf_svc", "join_conf", true), conf_id_(conf_id), conf_pwd_(conf_psw), nick_(nick) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
	std::string conf_pwd_;
	std::string nick_;
};


class ReportJoinReq : public ConfReq
{
public:
	ReportJoinReq(const int conf_id)
		: ConfReq("open_conf_svc", "report_join_conf", true), conf_id_(conf_id) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
};

class RoomListReq : public ConfReq
{
public:
    RoomListReq()
        : ConfReq("open_conf_svc", "get_conf_list", true) {}
    virtual std::string GenReq() const override;
};

class HeartBeatReq : public ConfReq
{
public:
	HeartBeatReq(const int conf_id)
		: ConfReq("open_conf_svc", "heart_beat", true), conf_id_(conf_id) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
};

class ModifyMemInfoReq : public ConfReq
{
public:
	ModifyMemInfoReq(const int conf_id, const std::vector<ModifyMemberInfoItem>& infos)
		: ConfReq("open_conf_svc", "modify_member_info", true), conf_id_(conf_id), infos_(infos) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
	std::vector<ModifyMemberInfoItem> infos_;
};


class ReportWhiteBoardReq : public ConfReq
{
public:
	ReportWhiteBoardReq(const int conf_id, Json::Value &board_data)
		: ConfReq("open_conf_svc", "report_board_data", true), conf_id_(conf_id), board_data_(board_data) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
	Json::Value board_data_;
};


class GetWhiteBoardReq : public ConfReq
{
public:
	GetWhiteBoardReq(const int conf_id)
		: ConfReq("open_conf_svc", "get_board_data", true), conf_id_(conf_id) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
};


class StartRecordReq : public ConfReq
{
public:
	StartRecordReq(const int conf_id, unsigned time_stamp)
		: ConfReq("open_conf_svc", "start_record_conf", true), conf_id_(conf_id), time_stamp_(time_stamp) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
	long time_stamp_;

};


class StopRecordReq : public ConfReq
{
public:
	StopRecordReq(const int conf_id, unsigned time_stamp)
		: ConfReq("open_conf_svc", "end_up_record_conf", true), conf_id_(conf_id), time_stamp_(time_stamp) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
	long time_stamp_;

};


class RecordListReq : public ConfReq
{
public:
	RecordListReq()
		: ConfReq("open_conf_svc", "get_replay_idx_list", true) {}
	virtual std::string GenReq() const override;

};


class CosSigReq : public ConfReq
{
public:
	CosSigReq(const std::string &path, const std::string &bucket)
		: ConfReq("open_cos_svc", "get_cos_sign", true), type_(1), path_(path), bucket_(bucket) {}
	virtual std::string GenReq() const override;
private:
	int type_;
	std::string path_;
	std::string bucket_;
};

class QuitConfReq : public ConfReq
{
public:
	QuitConfReq(const int conf_id)
		: ConfReq("open_conf_svc", "quit_conf", true), conf_id_(conf_id) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
};

class DestroyConfReq : public ConfReq
{
public:
	DestroyConfReq(const int conf_id, const std::string &reason)
		: ConfReq("open_conf_svc", "destroy_conf", true), conf_id_(conf_id), reason_(reason){}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
	std::string reason_;
};

class DownloadBoardMessageReq : public ConfReq
{
public:
	DownloadBoardMessageReq(const int conf_id)
		:ConfReq("open_conf_svc", "download_board_message", true), conf_id_(conf_id) {}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
};

class GetMemberListReq : public ConfReq
{
public:
    GetMemberListReq(const int conf_id)
        :ConfReq("open_conf_svc", "get_member_list", true), conf_id_(conf_id) {}
    virtual std::string GenReq() const override;
private:
    int conf_id_;
};

class ModifyConfInfoReq : public ConfReq
{
public:
	ModifyConfInfoReq(const int conf_id, const std::string& home)
		:ConfReq("open_conf_svc", "modify_conf_info", true), conf_id_(conf_id), home_screen_(home){}
	virtual std::string GenReq() const override;
private:
	int conf_id_;
	std::string home_screen_;
};

class InviteInteractReq : public ConfReq
{
public:
    InviteInteractReq(const int conf_id, const std::string& member, int permission)
        :ConfReq("open_conf_svc", "invite_interact", true), conf_id_(conf_id), member_(member), permission_(permission) {}
    virtual std::string GenReq() const override;
private:
    int conf_id_;
    std::string member_;        // 被邀请成员userId
    int permission_;
};

class ApplyInteractReq : public ConfReq
{
public:
    ApplyInteractReq(const int conf_id, int permission)
        :ConfReq("open_conf_svc", "apply_permission", true), conf_id_(conf_id), permission_(permission) {}
    virtual std::string GenReq() const override;
private:
    int conf_id_;
    int permission_;
};

class GrandPermissionReq : public ConfReq
{
public:
    GrandPermissionReq(const int conf_id, const std::string& member, int permission)
        :ConfReq("open_conf_svc", "grant_permission", true), conf_id_(conf_id), member_(member), permission_(permission) {}
    virtual std::string GenReq() const override;
private:
    int conf_id_;
    std::string member_;        // 被赋予权限的成员userId
    int permission_;
};
