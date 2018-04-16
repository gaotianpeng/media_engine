#include "ConfRsp.h"
#include "json.h"


int ConfRsp::GetCode() const
{
	return error_code_;
}

std::string ConfRsp::GetMsg() const
{
	return error_msg_;
}

void* ConfRsp::GetData() const
{
	return data_;
}

bool RegisterRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();

        return true;
	}
}

bool LoginRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();
		sig_ = data["user_sig"].asString();
		token_ = data["user_token"].asString();

        return true;
	}
}

std::string LoginRsp::GetSig() const
{
	return sig_;
}

std::string LoginRsp::GetToken() const
{
	return token_;
}

std::string LoginRsp::GetId() const
{
	return id_;
}

void LoginRsp::SetId(const std::string& id)
{
	id_ = id;
}

bool CreateConfRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
        error_code_ = data["error_code"].asInt();
        error_msg_ = data["error_msg"].asString();
        chat_group_id_ = data["chat_group_id"].asString();
        white_board_group_id_ = data["board_group_id"].asString();
        room_id_ = data["room_id"].asInt();
        room_token_ = data["conf_token"].asString();
        room_name_ = data["conf_name"].asString();
        room_owner_ = data["owner"].asString();
        join_conf_url_ = data["join_conf_url"].asString();
        heartbeat_interval_ = data["heartbeat_interval"].asInt();

        return true;
	}
}

std::string CreateConfRsp::GetChatGroupId() const
{
    return chat_group_id_;
}

std::string CreateConfRsp::GetBoardGroupId() const
{
    return white_board_group_id_;
}

int CreateConfRsp::GetRoomId() const
{
    return room_id_;
}

std::string CreateConfRsp::GetRoomToken() const
{
    return room_token_;
}

std::string CreateConfRsp::GetRoomName() const
{
    return room_name_;
}

std::string CreateConfRsp::GetRoomOwner() const
{
    return room_owner_;
}

int CreateConfRsp::GetHeartbeat() const
{
    return heartbeat_interval_;
}

int CreateConfRsp::GetInterval() const
{
    return heartbeat_interval_;
}

std::string CreateConfRsp::GetInviteUrl() const
{
    return join_conf_url_;
}

bool ReportJoinRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();
		home_screen_ = data["home_screen"].asString();
        return true;
	}
}

std::string ReportJoinRsp::GetHomeScreen() const
{
	return home_screen_;
}

bool JoinConfRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();
		chat_group_id_ = data["chat_group_id"].asString();
		white_board_group_id_ = data["board_group_id"].asString();
		room_id_ = data["room_id"].asInt();
        room_token_ = data["conf_token"].asString();
        room_name_ = data["conf_name"].asString();
        room_owner_ = data["owner"].asString();
		join_conf_url_ = data["join_conf_url"].asString();
		heartbeat_interval_ = data["heartbeat_interval"].asInt();
		home_screen_ = data["home_screen"].asString();

        return true;
	}
}

std::string JoinConfRsp::GetChatGroupId() const
{
	return chat_group_id_;
}

std::string JoinConfRsp::GetBoardGroupId() const
{
	return white_board_group_id_;
}

int JoinConfRsp::GetRoomId() const
{
	return room_id_;
}

std::string JoinConfRsp::GetRoomToken() const
{
    return room_token_;
}

std::string JoinConfRsp::GetRoomName() const
{
    return room_name_;
}

std::string JoinConfRsp::GetRoomOwner() const
{
    return room_owner_;
}

int JoinConfRsp::GetHeartbeat() const
{
	return heartbeat_interval_;
}

int JoinConfRsp::GetInterval() const
{
	return heartbeat_interval_;
}

std::string JoinConfRsp::GetInviteUrl() const
{
	return join_conf_url_;
}

std::string JoinConfRsp::GetHomeScreen() const
{
	return home_screen_;
}

bool RoomListRsp::Parse(const std::string& buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
    {
        error_code_ = data["error_code"].asInt();
        error_msg_ = data["error_msg"].asString();

        error_code_ = 0;
        error_msg_ = "";
        Json::Value list_value = data["items"];
        int size = list_value.size();
        for (int i = 0; i < size; ++i)
        {
            RoomItem item;
            Json::Value item_value = list_value[i];
            item.roomId = item_value["conf_id"].asInt();
            item.roomName = item_value["conf_name"].asString();
            item.ownerId = item_value["owner"].asString();
            item.ownerNickName = item_value["owner_nick"].asString();
            item.roomPwd = item_value["conf_pwd"].asString();
            item.memberCount = item_value["member_count"].asInt();
            item.roomSig = item_value["conf_token"].asString();
            item.createTime = item_value["create_time"].asInt64();
            item.state = item_value["state"].asInt();
            room_list_.push_back(item);
        }

        return true;
    }
}

std::vector<RoomItem> RoomListRsp::GetRoomList() const
{
    return room_list_;
}

bool HeartBeatRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();

        return true;
	}
}

bool ModifyMemInfoRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();

        return true;
	}
}

bool ReportWhiteBoardRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
	if (reader.parse(buf, data))
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();
		return true;
	}
	return false;
}

bool GetWhiteBoardRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
	if (reader.parse(buf, data))
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();
		board_data_list_ = data["board_data_list"];
		return true;
	}
	return false;
}

Json::Value& GetWhiteBoardRsp::GetBoardDataList()
{
	return board_data_list_;
}

bool StartRecordRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();

        return true;
	}
}

bool StopRecordRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();

        return true;
	}
}

bool CosSigRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();
		sig_ = data["sign"].asString();
		bucket_ = data["bucket"].asString();
		region_ = data["region"].asString();

        return true;
	}
}

std::string CosSigRsp::GetSig() const
{
	return sig_;
}

std::string CosSigRsp::GetBucket() const
{
	return bucket_;
}

std::string CosSigRsp::GetRegion() const
{
	if(region_ == "tj")
	{
		return "ap-beijing-1";
	}
	else if(region_ == "bj")
	{
		return "ap-beijing";
	}
	else if(region_ == "sh")
	{
		return "ap-shanghai";
	}
	else if(region_ == "gz")
	{
		return "ap-guangzhou";
	}
	else if (region_ == "cd")
	{
		return "ap-chengdu";
	}
	else if (region_ == "sgp")
	{
		return "ap-singapore";
	}
	else if (region_ == "hk")
	{
		return "ap-hongkong";
	}
	else if (region_ == "ca")
	{
		return "na-toronto";
	}
	else if (region_ == "ger")
	{
		return "eu-frankfurt";
	}
	else {
		return region_;
	}
}

bool QuitConfRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();

        return true;
	}
}

bool DestroyConfRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
	if (!reader.parse(buf, data))
	{
		return false;
	}
	else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();

		return true;
	}
}


bool DownloadBoardMessageRsp::Parse(const std::string &buf)
{
	Json::Reader reader;
	Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();
		Json::FastWriter writer;
		board_string_ = writer.write(data["msg_list"]);

        return true;
	}
}

const std::string DownloadBoardMessageRsp::getBoardString()
{
	return board_string_;
}

bool GetMemberListRsp::Parse(const std::string &buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
    {
        error_code_ = 0;
        error_msg_ = "";
        total_ = data["total"].asInt();
        Json::Value list_value = data["member_list"];
        int size = list_value.size();
        for (int i = 0; i < size; ++i)
        {
            MemberItem item;
            Json::Value item_value = list_value[i];
            item.id = item_value["id"].asString();
            item.roomId = item_value["conf_id"].asInt();
            item.status = item_value["status"].asInt();
            item.mic = item_value["mic"].asInt();
            item.camera = item_value["camera"].asInt();
            item.screen = item_value["screen"].asInt();
            item.video = item_value["video"].asInt();
            item.joinType = item_value["join_type"].asString();
            item.nickName = item_value["nick"].asString();
            item.activeTime = item_value["active_time"].asInt64();
            member_list_.push_back(item);
        }

        return true;
    }
}

int GetMemberListRsp::getTotal() const
{
    return total_;
}

std::vector<MemberItem> GetMemberListRsp::getMemberList() const
{
    return member_list_;
}

bool InviteInteractRsp::Parse(const std::string &buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
    {
        error_code_ = data["error_code"].asInt();
        error_msg_ = data["error_msg"].asString();

        return true;
    }
}

bool ApplyInteractRsp::Parse(const std::string &buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
    {
        error_code_ = data["error_code"].asInt();
        error_msg_ = data["error_msg"].asString();

        return true;
    }
}

bool GrandPermissionRsp::Parse(const std::string &buf)
{
    Json::Reader reader;
    Json::Value data;
    if (!reader.parse(buf, data))
    {
        return false;
    }
    else
    {
        error_code_ = data["error_code"].asInt();
        error_msg_ = data["error_msg"].asString();

        return true;
    }
}

bool ModifyConfInfoRsp::Parse(const std::string& buf)
{
	Json::Reader reader;
	Json::Value data;
	if (!reader.parse(buf, data))
	{
		return false;
	}
	else
	{
		error_code_ = data["error_code"].asInt();
		error_msg_ = data["error_msg"].asString();

		return true;
	}
}

