#include "ConfReq.h"
#include <chrono>
#include <Windows.h>
#include "json.h"
#include "avsdkTime.h"

std::string ConfListReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["id"] = id_;
	data["key"] = key_;
	data["index"] = index_;
	data["size"] = size_;
	Json::FastWriter writer;
	return writer.write(data);	
}

std::string ConfReq::GetCmd() const
{
	return cmd_;
}

std::string ConfReq::GetSubCmd() const
{
	return subcmd_;
}

bool ConfReq::GetVerify() const
{
	return verify_;
}

std::string RegisterReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["id"] = id_;
	data["password"] = password_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string LoginReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["id"] = id_;
	data["password"] = password_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string CreateConfReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_name"] = conf_name_;
	data["conf_pwd"] = conf_psw_;
	data["home_screen"] = home_screen_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string ReportJoinReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	data["local_timestamp"] = getAvsdkTime();
	Json::FastWriter writer;
	return writer.write(data);
}

std::string RoomListReq::GenReq() const
{
    Json::Value data;
    data["cmd"] = cmd_;
    data["sub_cmd"] = subcmd_;
    Json::FastWriter writer;
    return writer.write(data);
}

std::string JoinConfReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	data["conf_pwd"] = conf_pwd_;
	data["nick"] = nick_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string HeartBeatReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string ModifyMemInfoReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	Json::Value info_array;
	for (std::vector<ModifyMemberInfoItem>::const_iterator it = infos_.begin(); it != infos_.end(); ++it)
	{
		Json::Value info;
        info["id"] = it->id;
		info["camera"] = it->camera;
		info["mic"] = it->mic;
		info["screen"] = it->screen;
		info["video"] = it->video;
		info_array.append(info);
	}
	data["modify_infos"] = info_array;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string ReportWhiteBoardReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	Json::Value board_data_list;
	board_data_list.append(board_data_);
	data["board_data_list"] = board_data_list;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string GetWhiteBoardReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string StartRecordReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	data["local_timestamp"] = time_stamp_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string StopRecordReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	data["local_timestamp"] = time_stamp_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string RecordListReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string CosSigReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["type"] = type_;
	data["file_path"] = path_;
	if (!bucket_.empty())
		data["bucket"] = bucket_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string QuitConfReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string DownloadBoardMessageReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string GetMemberListReq::GenReq() const
{
    Json::Value data;
    data["cmd"] = cmd_;
    data["sub_cmd"] = subcmd_;
    data["conf_id"] = conf_id_;
    Json::FastWriter writer;
    return writer.write(data);
}

std::string InviteInteractReq::GenReq() const
{
    Json::Value data;
    data["cmd"] = cmd_;
    data["sub_cmd"] = subcmd_;
    data["conf_id"] = conf_id_;
    data["member"] = member_;
    data["permission"] = permission_;
    Json::FastWriter writer;
    return writer.write(data);
}

std::string ApplyInteractReq::GenReq() const
{
    Json::Value data;
    data["cmd"] = cmd_;
    data["sub_cmd"] = subcmd_;
    data["conf_id"] = conf_id_;
    data["permission"] = permission_;
    Json::FastWriter writer;
    return writer.write(data);
}

std::string GrandPermissionReq::GenReq() const
{
    Json::Value data;
    data["cmd"] = cmd_;
    data["sub_cmd"] = subcmd_;
    data["conf_id"] = conf_id_;
    data["member"] = member_;
    data["permission"] = permission_;
    Json::FastWriter writer;
    return writer.write(data);
}

std::string ModifyConfInfoReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	Json::Value info;
	//info["conf_name"] = conf_name_;
	info["home_screen"] = home_screen_;
	data["modify_infos"] = info;
	Json::FastWriter writer;
	return writer.write(data);
}

std::string DestroyConfReq::GenReq() const
{
	Json::Value data;
	data["cmd"] = cmd_;
	data["sub_cmd"] = subcmd_;
	data["conf_id"] = conf_id_;
	data["reason"] = reason_;
	Json::FastWriter writer;
	return writer.write(data);
}
