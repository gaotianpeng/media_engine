#pragma once

enum UserType
{
	TEACHER_USER_TYPE = 1,
	STUDENT_USER_TYPE = 2,
};

enum EnterType
{
    NoType,
    CreateRoomType,     // 创建，不需要Room name
    JoinRoomType,       // 加入，指定Room name
	RetryRoomType,      //重试
};

// 登录传递过来的信息
struct AuthInfo
{
    UserType m_userType;

    int m_sdkAppId;
    int m_accountType;
    std::string m_nickName;
    std::string m_userId;
    std::string m_token;
    std::string m_userSig;

    int m_roomID;
    std::string m_roomSig;
    std::string m_roomName;
    std::string m_roomOwner;
    std::string m_chatGroudID;
    std::string m_whiteBoardGroudID;
    std::string m_joinConfURL;
};

enum InteractPermission
{
    WhiteboardPermission = 1, 
    CameraPermission = 1 << 1,
    MicPermission = 1 << 2, 
    ScreenPermission = 1 << 3,
    VideoPermission = 1 << 4,
};

struct RoomItem
{
    int roomId;
    std::string roomName;
    std::string ownerId;
    std::string ownerNickName;
    std::string roomPwd;
    int memberCount;
    std::string roomSig;
    long long createTime;
    int state;
};

struct MemberItem
{
	std::string id;
    int roomId;
	int status;
	int mic;
	int camera;
	int screen;
    int video;
    std::string joinType;
	std::string nickName;
    long long activeTime;
};

struct ModifyMemberInfoItem
{
	std::string id;
	int status;
	int mic;
	int camera;
	int screen;
	int video;
};

struct VideoHandsupItem
{
	std::string id;
	int status;
	int mic;
	int camera;
	int screen;
	int video;
	int index;
	std::string nickName;
};

enum VideoHandsStatus
{
	STATUS_HANDING = 0,
	STATUS_INVITING = 1,
	STATUS_ONSTAGE = 2,
};