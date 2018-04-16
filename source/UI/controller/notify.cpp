#include "notify.h"
#include "json.h"

#include <chrono>

bool MemberJoinNotify::Parse(const std::string& buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else if (subcmd_ != data["sub_cmd"].asString())
    {
        return false;
    }
    else
    {
        item_.roomId = data["conf_id"].asInt();

        Json::Value member = data["member"];
        item_.id = member["id"].asString();
        item_.status = member["status"].asInt();
        item_.nickName = member["nick"].asString();
        item_.joinType = member["join_type"].asString();
        item_.mic = member["mic"].asInt();
        item_.camera = member["camera"].asInt();
        item_.screen = member["screen"].asInt();
        item_.video = member["video"].asInt();
        item_.activeTime = member["active_time"].asInt64();

        return true;
    }
}

MemberItem MemberJoinNotify::GetMemberItem() const
{
    return item_;
}

bool MemberQuitNotify::Parse(const std::string& buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else if (subcmd_ != data["sub_cmd"].asString())
    {
        return false;
    }
    else
    {
        room_id_ = data["conf_id"].asInt();

        Json::Value member = data["member"];
        user_id_ = member["id"].asString();
        reason_= member["reason"].asString();

        return true;
    }
}

int MemberQuitNotify::GetRoomId() const
{
    return room_id_;
}

std::string MemberQuitNotify::GetUserId() const
{
    return user_id_;
}

std::string MemberQuitNotify::GetReason() const
{
    return reason_;
}

bool DestoyRoomNotify::Parse(const std::string& buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else if (subcmd_ != data["sub_cmd"].asString())
    {
        return false;
    }
    else
    {
        room_id_ = data["conf_id"].asInt();
        time_ = data["time"].asInt();
        desc_ = data["desc"].asString();

        return true;
    }
}

int DestoyRoomNotify::GetRoomId() const
{
    return room_id_;
}

long long DestoyRoomNotify::GetTime() const
{
    return time_;
}

std::string DestoyRoomNotify::GetDesc() const
{
    return desc_;
}

bool ModifyMemberInfoNotify::Parse(const std::string& buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else if (subcmd_ != data["sub_cmd"].asString())
    {
        return false;
    }
    else
    {
        int roomId = data["conf_id"].asInt();

        Json::Value list_value = data["modify_infos"];
        int size = list_value.size();
        for (int i = 0; i < size; ++i)
        {
            ModifyMemberInfoItem item;
            Json::Value item_value = list_value[i];
            item.id = item_value["id"].asString();
			if (item_value["status"].isInt())
			{
				item.status = item_value["status"].asInt();
			}
			else
				item.status = 1;			
            item.mic = item_value["mic"].asInt();
            item.camera = item_value["camera"].asInt();
            item.screen = item_value["screen"].asInt();
			item.video = item_value["video"].asInt();
            member_modify_list_[item.id] = item;
        }

        return true;
    }
}

std::map<std::string, ModifyMemberInfoItem> ModifyMemberInfoNotify::GetMemberMoidfyList() const
{
    return member_modify_list_;
}

bool InviteInteractNotify::Parse(const std::string& buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else if (subcmd_ != data["sub_cmd"].asString())
    {
        return false;
    }
    else
    {
        room_id_ = data["conf_id"].asInt();
        permission_ = data["permission"].asInt();

        return true;
    }
}

int InviteInteractNotify::GetRoomId() const
{
    return room_id_;
}

int InviteInteractNotify::GetPermission() const
{
    return permission_;
}

bool ApplyInteractNotify::Parse(const std::string& buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else if (subcmd_ != data["sub_cmd"].asString())
    {
        return false;
    }
    else
    {
        room_id_ = data["conf_id"].asInt();
        member_ = data["member"].asString();
        permission_ = data["permission"].asInt();

        return true;
    }
}

int ApplyInteractNotify::GetRoomId() const
{
    return room_id_;
}

std::string ApplyInteractNotify::GetMember() const
{
    return member_;
}

int ApplyInteractNotify::GetPermission() const
{
    return permission_;
}

bool GrandPermissionNotify::Parse(const std::string& buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else if (subcmd_ != data["sub_cmd"].asString())
    {
        return false;
    }
    else
    {
        room_id_ = data["conf_id"].asInt();
        permission_ = data["permission"].asInt();

        return true;
    }
}

int GrandPermissionNotify::GetRoomId() const
{
    return room_id_;
}

int GrandPermissionNotify::GetPermission() const
{
    return permission_;
}

bool ModifyRoomInfoNotify::Parse(const std::string& buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else if (subcmd_ != data["sub_cmd"].asString())
    {
        return false;
    }
    else
    {
        user_id_ = data["id"].asString();
        room_id_ = data["conf_id"].asInt();

        Json::Value modify_value = data["modify_infos"];
        home_screen_ = modify_value["home_screen"].asString();
        room_name_ = modify_value["conf_name"].asString();

        return true;
    }
}

std::string ModifyRoomInfoNotify::GetUserId() const
{
    return user_id_;
}

int ModifyRoomInfoNotify::GetRoomId() const
{
    return room_id_;
}

std::string ModifyRoomInfoNotify::homeScreen() const
{
    return home_screen_;
}

std::string ModifyRoomInfoNotify::roomName() const
{
    return room_name_;
}
