#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "commonType.h"

class Notify
{
public:
    Notify(const std::string& cmd, const std::string& subcmd)
        : cmd_(cmd)
        , subcmd_(subcmd)
    {

    }

    virtual bool Parse(const std::string& buf) = 0;

    virtual ~Notify()
    {
    }

protected:
    std::string cmd_;
    std::string subcmd_;
};

class MemberJoinNotify : public Notify
{
public:
    MemberJoinNotify()
        : Notify("open_conf_svc", "member_join_notify")
        , item_()
    {

    }

    virtual bool Parse(const std::string& buf);
    MemberItem GetMemberItem() const;
protected:
    MemberItem item_;
};

class MemberQuitNotify : public Notify
{
public:
    MemberQuitNotify()
        : Notify("open_conf_svc", "member_quit_notify")
        , room_id_(0)
        , user_id_("")
        , reason_("")
    {

    }

    virtual bool Parse(const std::string& buf);
    int GetRoomId() const;
    std::string GetUserId() const;
    std::string GetReason() const;
protected:
    int room_id_;
    std::string user_id_;
    std::string reason_;
};

class DestoyRoomNotify : public Notify
{
public:
    DestoyRoomNotify()
        : Notify("open_conf_svc", "destroy_notify")
        , room_id_(0)
        , time_(0)
        , desc_("")
    {

    }

    virtual bool Parse(const std::string& buf);
    int GetRoomId() const;
    long long GetTime() const;
    std::string GetDesc() const;
protected:
    int room_id_;
    long long time_;
    std::string desc_;
};

class ModifyMemberInfoNotify : public Notify
{
public:
    ModifyMemberInfoNotify()
        : Notify("open_conf_svc", "modify_member_info_notify")
        , member_modify_list_()
    {

    }

    virtual bool Parse(const std::string& buf);
    std::map<std::string, ModifyMemberInfoItem> GetMemberMoidfyList() const;
protected:
    std::map<std::string ,ModifyMemberInfoItem> member_modify_list_;
};

class InviteInteractNotify : public Notify
{
public:
    InviteInteractNotify()
        : Notify("open_conf_svc", "invite_interact_notify")
        , room_id_(0)
        , permission_(0)
    {

    }

    virtual bool Parse(const std::string& buf);
    int GetRoomId() const;
    int GetPermission() const;
protected:
    int room_id_;
    int permission_;
};

class ApplyInteractNotify : public Notify
{
public:
    ApplyInteractNotify()
        : Notify("open_conf_svc", "apply_permission_notify")
        , room_id_(0)
        , permission_(0)
    {

    }

    virtual bool Parse(const std::string& buf);
    int GetRoomId() const;
    std::string GetMember() const;
    int GetPermission() const;
protected:
    int room_id_;
    std::string member_;
    int permission_;
};

class GrandPermissionNotify : public Notify
{
public:
    GrandPermissionNotify()
        : Notify("open_conf_svc", "grant_permission_notify")
        , room_id_(0)
        , permission_(0)
    {

    }

    virtual bool Parse(const std::string& buf);
    int GetRoomId() const;
    int GetPermission() const;
protected:
    int room_id_;
    int permission_;
};

class ModifyRoomInfoNotify : public Notify
{
public:
    ModifyRoomInfoNotify()
        : Notify("open_conf_svc", "modify_conf_info_notify")
        , user_id_("")
        , room_id_(0)
        , home_screen_("")
        , room_name_("")
    {

    }

    virtual bool Parse(const std::string& buf);
    std::string GetUserId() const;
    int GetRoomId() const;
    std::string homeScreen() const;
    std::string roomName() const;
protected:
    std::string user_id_;
    int room_id_;
    std::string home_screen_;
    std::string room_name_;
};
