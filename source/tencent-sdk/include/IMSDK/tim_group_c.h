#ifndef _TIM_GROUP_C_H_
#define _TIM_GROUP_C_H_

#include "tim_comm_c.h"
#include "tim_msg_c.h"

#define TIM_PRIVATE_GROUP "Private"
#define TIM_PUBLIC_GROUP  "Public"
#define TIM_CHATROOM_GROUP "ChatRoom"
#define TIM_AVCHATROOM_GROUP "AVChatRoom"

//����ʧ��
#define TIM_GROUP_MEMBER_STATUS_FAIL 0
//�����ɹ�
#define TIM_GROUP_MEMBER_STATUS_SUCC 1

extern "C"
{
//handle
	//��չ�ֶ���� handle
	typedef void* TIMGroupCustomInfoHandle;

	//������Ⱥ�����Ϣ handle
	typedef void* TIMNewGroupMemberInfoHandle;
	typedef void* TIMNewGroupInfoHandle;
	
	//��ȡȺ��Ϣ���ѡ��
	typedef void* TIMGetGroupDetailInfoOptionHandle;
	typedef void* TIMGetGroupMemberInfoOptionHandle;
	typedef void* TIMGetGroupPendencyOptHandle;

	//�޸�Ⱥ��Ϣ���ѡ��
	typedef void* TIMModifyGroupMemberInfoOptionHandle;
	typedef void* TIMModifyGroupBaseInfoOptionHandle;

	//�ӿڻص����������Ϣ
	typedef void* TIMGroupBaseInfoHandle;
	typedef void* TIMGroupDetailInfoHandle;
	typedef void* TIMGroupMemberResultHandle;
	typedef void* TIMGroupPendencyItemHandle;
	typedef void* TIMGroupPendencyMetaHandle;

//struct and const num

	typedef enum _E_TIMGetGroupBaseInfoFlag
	{
		kGetNone =			0x00,
		kGetGroupName =		0x01,
		kGetCreateTime =	0x01 << 1,
		kGetOwnerUin =		0x01 << 2,
		kGetInfoSeq =		0x01 << 3,
		kGetLastInfoTime =	0x01 << 4,
		kGetNextMsgSeq =	0x01 << 5,
		kGetLastMsgTime =	0X01 << 6,
		kGetAppId =			0x01 << 7,
		kGetMemberNum =		0x01 << 8,
		kGetMaxMemberNum =	0x01 << 9,
		kGetNotification =	0x01 << 10,
		kGetIntroDuction =	0x01 << 11,
		kGetFaceUrl =		0x01 << 12,
		kGetAddOpton =		0x01 << 13,
		kGetGroupType =		0x01 << 14,
		kGetGroupLastMsg =  0x01 << 15,
		kGetGroupBaseInfoOnlineNum = 0x01 << 16,
		kGetGroupBaseInfoVisible = 0x01 << 17,
		kGetGroupBaseInfoSearchable = 0x01 << 18
	}TIMGetGroupBaseInfoFlag;

	typedef enum _E_TIMGetGroupMemInfoFlag
	{
		kJionTime	= 0x01,
		kMsgFlag	= 0x01 << 1,
		kRole		= 0x01 << 3,
		kShutupUntill= 0x01 << 4,
		kNameCard	= 0x01 << 5,
	}TIMGetGroupMemInfoFlag;

	typedef enum _E_TIMGetGroupMemRoleFilter
	{
		kGetGroupMemberAll = 0x00,
		kGetGroupMemRoleOwner = 0x01,
		kGetGroupMemRoleAdmin = 0x01 << 1,
		kGetGroupMemRoleCommon_member = 0x01 << 2,
	}TIMGetGroupMemRoleFilter;

	typedef enum _E_TIMModifyGroupMemberFlag
	{
		kModifyMsgFlag = 0x01,		// ��Ϣ����ѡ�0�����գ�1���ܾ���. ��СȨ��: ROOT    �Լ������޸�
		kModifyRole = 0x01 << 1,	// Ⱥ����ݣ�300��ʾ������Ϊ����Ա.  ��СȨ��: ROOT    �Լ������޸�
		kModifyShutupTime = 0x01 << 2,  // ����ʱ��,��λ:��.           ��СȨ��: ����Ա  �Լ������޸�
		kModifyNameCard = 0x01 << 3 // Ⱥ��Ƭ
	}TIMModifyGroupMemberFlag;

	typedef enum _T_TIMModifyGroupFlag
	{
		kModifyNone	=			0x00,
		kModifyGroupName =		0x01,
		kModifyNotification =	0x01 << 1,
		kModifyIntroduction =	0x01 << 2,
		kModifyFaceUrl =		0x01 << 3,
		kModifyAddOption =		0x01 << 4,  // �����Ⱥѡ�0��ֹ���룻1��Ҫ������2������룻
		kModifyMaxMmeberNum =	0x01 << 5,  // ���Ⱥ��Ա��������ROOT���Բ�����
		kModifyGroupVisible =	0x01 << 6,	// ����Ⱥ�Ƿ�ɼ�
		kModifyGroupSearchable = 0x01 << 7, // ����Ⱥ�Ƿ�ɱ�����
	}TIMModifyGroupFlag;

	//��Ⱥѡ��
	typedef enum _E_TIMGroupAddOpt
	{
		TIM_GROUP_ADD_FORBID    = 0, //��ֹ��Ⱥ
		TIM_GROUP_ADD_AUTH		= 1, //��Ҫ����Ա����
		TIM_GROUP_ADD_ANY       = 2, // �κ��˿��Լ���
	}TIMGroupAddOpt;

	typedef enum _E_TIMGroupMemRoleFilter
	{
		TIMGroupMemberAll		= 0x00,
		TIMGroupMemRoleOwner	= 0x01,
		TIMGroupMemRoleAdmin	= 0x01 << 1,
		TIMGroupMemRoleCommon_member = 0x01 << 2,
	}TIMGroupMemRoleFilter;

	typedef enum _E_TIMGroupPendencyType
	{
		TIMGroupApplyJoinPendency = 0,	//�����Ⱥ��0
		TIMGroupInvitePendency = 1,		//�����Ⱥ��1
		TIMGroupBothPendency = 2,		//���������ģ�2
	}TIMGroupPendencyType;

	typedef enum _E_TIMGroupPendencyHandleFlag
	{
		TIMGroupPendencyFlagUndecide = 0,
		TIMGroupPendencyFlagOtherDecide = 1,
		TIMGroupPendencyFlagOperatorDecide = 2,
	}TIMGroupPendencyHandleFlag;

	typedef enum _E_TIMComStatus{ 
		kNotSet = 0, 
		kClose = 1, 
		kOpen = 2 
	}TIMComStatus;

// handle operation
	TIM_DECL TIMGroupCustomInfoHandle CreateGroupCustomInfoHandle();
	TIM_DECL void DestroyGroupCustomInfoHandle(TIMGroupCustomInfoHandle handle);
	TIM_DECL TIMGroupCustomInfoHandle CloneGroupCustomInfoHandle(TIMGroupCustomInfoHandle handle);
	//value ��������512�ֽ�
	TIM_DECL bool AddGroupCustomInfo(TIMGroupCustomInfoHandle handle, const char* key, const uint32_t key_len, const char* value, const uint32_t value_len);
	TIM_DECL uint32_t GetGroupCustomInfoNum(TIMGroupCustomInfoHandle handle);
	TIM_DECL int GetGroupCustomInfo(TIMGroupCustomInfoHandle handle, char* key, uint32_t key_len, char* value, uint32_t* value_len);
	TIM_DECL int GetGroupCustomInfoByIndex(TIMGroupCustomInfoHandle handle, int index, char* key, uint32_t* key_len, char* value, uint32_t* value_len);
	

	TIM_DECL TIMNewGroupMemberInfoHandle CreateNewGroupMemberInfoHandle();
	TIM_DECL void DestroyNewGroupMemberInfoHandle(TIMNewGroupMemberInfoHandle handle);
	TIM_DECL void SetNewGroupMemberInfoIdentifier(TIMNewGroupMemberInfoHandle handle, const char* identifier);
	TIM_DECL void SetNewGroupMemberInfoMemberRole(TIMNewGroupMemberInfoHandle handle, int member_role);
	TIM_DECL void SetNewGroupMemberInfoCustomInfo(TIMNewGroupMemberInfoHandle handle, TIMGroupCustomInfoHandle custom_info);

	TIM_DECL TIMNewGroupInfoHandle CreateNewGroupInfoHandle();
	TIM_DECL void DestroyNewGroupInfoHandle(TIMNewGroupInfoHandle handle);
	TIM_DECL void SetNewGroupInfoGroupType(TIMNewGroupInfoHandle handle, const char* group_type);
	TIM_DECL void SetNewGroupInfoGroupName(TIMNewGroupInfoHandle handle, const char* group_name, const uint32_t name_len);
	TIM_DECL void SetNewGroupInfoGroupMembers(TIMNewGroupInfoHandle handle, TIMNewGroupMemberInfoHandle* members, const uint32_t num);
	TIM_DECL void SetNewGroupInfoGroupNotification(TIMNewGroupInfoHandle handle, const char* notification, const uint32_t nofi_len);
	TIM_DECL void SetNewGroupInfoGroupIntroduction(TIMNewGroupInfoHandle handle, const char* introduction, const uint32_t intro_len);
	TIM_DECL void SetNewGroupInfoFaceUrl(TIMNewGroupInfoHandle handle, const char* face_url, const uint32_t face_url_len);
	TIM_DECL void SetNewGroupInfoId(TIMNewGroupInfoHandle handle, const char* id);
	TIM_DECL void SetNewGroupInfoAddOption(TIMNewGroupInfoHandle handle, const TIMGroupAddOpt opt);
	TIM_DECL void SetNewGroupInfoMaxMemberNum(TIMNewGroupInfoHandle handle, const uint32_t max_member_num);
	TIM_DECL void SetNewGroupInfoGroupCustomInfo(TIMNewGroupInfoHandle handle, TIMGroupCustomInfoHandle custom_info_handle);

	TIM_DECL TIMGetGroupDetailInfoOptionHandle CreateGetGroupDetailInfoOptionHandle();
	TIM_DECL void DestoryGetGroupDetailInfoOptionHandle(TIMGetGroupDetailInfoOptionHandle handle);
	TIM_DECL int SetFlag4GetGroupDetailInfoOptionHandle(TIMGetGroupDetailInfoOptionHandle handle, TIMGetGroupBaseInfoFlag flag);
	TIM_DECL int SetCustomInfo4GetGroupDetailnfoOptionHandle(TIMGetGroupDetailInfoOptionHandle handle, TIMGroupCustomInfoHandle custom_info_handle);
	TIM_DECL int SetGroups4GetGroupDetailInfoOptionHandle(TIMGetGroupDetailInfoOptionHandle handle, char** group_id, uint32_t group_num);

	TIM_DECL TIMGetGroupMemberInfoOptionHandle CreateGetGroupMemberInfoOptionHandle();
	TIM_DECL void DestroyGetGroupMemberInfoOptionHandle(TIMGetGroupMemberInfoOptionHandle handle);
	TIM_DECL void SetGroupMemFilter4GetGroupMemberInfoOptionHandle(TIMGetGroupMemberInfoOptionHandle handle, TIMGetGroupMemRoleFilter filter);
	TIM_DECL int SetGroupMember4GetGroupMemberInfoOptionHandle(TIMGetGroupMemberInfoOptionHandle handle, char** members_id, uint32_t member_num);
	TIM_DECL int SetCustomInfo4GetGroupMemberInfoOptionHandle(TIMGetGroupDetailInfoOptionHandle handle, TIMGroupCustomInfoHandle custom_info_handle);
	TIM_DECL int SetGroupID4GetGroupMemberInfoOptionHandle(TIMGetGroupMemberInfoOptionHandle handle, char* group_id);
	TIM_DECL int SetFlag4GetGroupMemberInfoOptionHandle(TIMGetGroupMemberInfoOptionHandle handle, TIMGetGroupMemInfoFlag flag);

	TIM_DECL TIMModifyGroupMemberInfoOptionHandle CreateModifyGroupMemberInfoOptionHandle();
	TIM_DECL void DestroyModifyGroupMemberInfoOptionHandle(TIMModifyGroupMemberInfoOptionHandle handle);
	TIM_DECL int SetGroupID4ModifyGroupMemberInfoOptionHandle(TIMModifyGroupMemberInfoOptionHandle handle, char* group_id);
	TIM_DECL int SetMemberID4ModifyGroupMemberInfoOptionHandle(TIMModifyGroupMemberInfoOptionHandle handle, char* member_id);
	TIM_DECL int SetFlag4ModifyGroupMemberInfoOptionHandle(TIMModifyGroupMemberInfoOptionHandle handle, TIMModifyGroupMemberFlag flag);
	TIM_DECL int SetMsgFlag4ModifyGroupMemberInfoOptionHandle(TIMModifyGroupMemberInfoOptionHandle handle, uint32_t flag);
	TIM_DECL int SetRole4ModifyGroupMemberInfoOptionHandle(TIMModifyGroupMemberInfoOptionHandle handle, uint32_t role);
	TIM_DECL int SetShutupTime4ModifyGroupMemberInfoOptionHandle(TIMModifyGroupMemberInfoOptionHandle handle, uint32_t time);
	TIM_DECL int SetNameCard4ModifyGroupMemberInfoOptionHandle(TIMModifyGroupMemberInfoOptionHandle handle, const char* name_card, uint32_t len);
	TIM_DECL int SetCustomInfo4ModifyGroupMemberInfoOptionHandle(TIMModifyGroupMemberInfoOptionHandle handle, TIMGroupCustomInfoHandle custom_info_handle);

	TIM_DECL TIMModifyGroupBaseInfoOptionHandle CreateModifyGroupBaseInfoOptionHandle();
	TIM_DECL void DestroyModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle);
	TIM_DECL int SetGroupId4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, char* group_id);
	TIM_DECL int SetFlag4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, TIMModifyGroupFlag flag);
	TIM_DECL int SetGroupName4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, char* group_name);
	TIM_DECL int SetNotification4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, const char* notification, uint32_t nofi_len);
	TIM_DECL int SetIntroduction4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, const char* introduction, uint32_t intro_len);
	TIM_DECL int SetFaceUrl4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, const char* face_url, uint32_t face_url_len);
	TIM_DECL int SetVisuableFlag4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, TIMComStatus flag);
	TIM_DECL int SetSearchableFlag4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, TIMComStatus flag);
	//�����Ⱥѡ�0��ֹ���룻1��Ҫ������2������룻
	TIM_DECL int SetAddOption4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, const TIMGroupAddOpt add_option);
	TIM_DECL int SetMaxMemberNum4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, uint32_t max_member_num);
	TIM_DECL int SetCustomInfo4ModifyGroupBaseInfoOptionHandle(TIMModifyGroupBaseInfoOptionHandle handle, TIMGroupCustomInfoHandle custom_info_handle);

	TIM_DECL TIMGroupMemberResultHandle CreateGroupMemberResultHandle();
	TIM_DECL void DestroyGroupMemberResultHandle(TIMGroupMemberResultHandle handle);
	TIM_DECL TIMGroupMemberResultHandle CloneGroupMemberResultHandle(TIMGroupMemberResultHandle handle);
	TIM_DECL int GetGroupMemberResultID(TIMGroupMemberResultHandle handle, char* id, uint32_t* len);
	TIM_DECL uint32_t GetGroupMemberResult(TIMGroupMemberResultHandle handle);
	TIM_DECL void SetGroupMemberResultID(TIMGroupMemberResultHandle handle, const char* open_id);
	TIM_DECL void SetGroupMemberResult(TIMGroupMemberResultHandle handle, uint32_t result);

	TIM_DECL TIMGroupBaseInfoHandle CreateGroupBaseInfoHandle();
	TIM_DECL void DestroyGroupBaseInfoHandle(TIMGroupBaseInfoHandle handle);
	TIM_DECL TIMGroupBaseInfoHandle CloneGroupBaseInfoHandle(TIMGroupBaseInfoHandle handle);
	//Ⱥ��ID
	TIM_DECL int GetGroupBaseInfoID(TIMGroupBaseInfoHandle handle, char* id, uint32_t* len);
	//Ⱥ����
	TIM_DECL int GetGroupBaseInfoName(TIMGroupBaseInfoHandle handle, char* name, uint32_t* len);
	//Ⱥ����
	TIM_DECL int GetGroupBaseInfoType(TIMGroupBaseInfoHandle handle, char* type, uint32_t* len);
	//�Լ�����Ϣ����λ 0�����գ�1������
	TIM_DECL uint32_t GetGroupBaseInfoMsgFlag(TIMGroupBaseInfoHandle handle);

	TIM_DECL TIMGroupDetailInfoHandle CreateGroupDetailInfoHandle();
	TIM_DECL void DestroyGroupDetailInfoHandle(TIMGroupDetailInfoHandle handle);
	// Ⱥ��Id
	TIM_DECL int GetGroupDetailInfoID(TIMGroupDetailInfoHandle handle, char* id, uint32_t* len);
	// Ⱥ��
	TIM_DECL int GetGroupDetailInfoName(TIMGroupDetailInfoHandle handle, char* name, uint32_t* len);
	// ������
	TIM_DECL int GetGroupDetailInfoOwner(TIMGroupDetailInfoHandle handle, char* owner, uint32_t* len);
	// Ⱥ����
	TIM_DECL int GetGroupDetailInfoNotification(TIMGroupDetailInfoHandle handle, char* buf, uint32_t* len);
	// Ⱥ���
	TIM_DECL int GetGroupDetailInfoIntroduction(TIMGroupDetailInfoHandle handle, char* buf, uint32_t* len);
	TIM_DECL int GetGroupDetailInfoFaceURL(TIMGroupDetailInfoHandle handle, char* url, uint32_t* len);
	// Ⱥ������
	TIM_DECL int GetGroupDetailInfoType(TIMGroupDetailInfoHandle handle, char* type, uint32_t* len);
	TIM_DECL uint32_t GetGroupDetailInfoCreateTime(TIMGroupDetailInfoHandle handle);
	// ���һ���޸�����ʱ��
	TIM_DECL uint32_t GetGroupDetailInfoLastInfoTime(TIMGroupDetailInfoHandle handle);
	// ���һ�η���Ϣʱ��
	TIM_DECL uint32_t GetGroupDetailInfoLastMsgTime(TIMGroupDetailInfoHandle handle);
	// Ⱥ��Ա����
	TIM_DECL uint32_t GetGroupDetailInfoMemberNum(TIMGroupDetailInfoHandle handle);
	TIM_DECL uint32_t GetGroupDetailInfoMaxMemberNum(TIMGroupDetailInfoHandle handle);
	TIM_DECL uint32_t GetGroupDetailInfoOnlineNum(TIMGroupDetailInfoHandle handle);
	TIM_DECL TIMComStatus GetGroupDetailInfoVisiableFlag(TIMGroupDetailInfoHandle handle);
	TIM_DECL TIMComStatus GetGroupDetailInfoSearchableFlag(TIMGroupDetailInfoHandle handle);
	TIM_DECL TIMGroupCustomInfoHandle GetGroupDetailInfoCustomInfo(TIMGroupDetailInfoHandle handle);
	TIM_DECL TIMMessageHandle CloneMessageHandleFromGroupDetailInfo(TIMGroupDetailInfoHandle handle);

	TIM_DECL TIMGroupMemberInfoHandle CreateGroupMemberInfoHandle();
	TIM_DECL void DestroyGroupMemberInfoHandle(TIMGroupMemberInfoHandle handle);
	//Ⱥ��ԱID
	TIM_DECL int GetGroupMemberID(TIMGroupMemberInfoHandle handle, char* id, uint32_t* len);
	//����Ⱥ��ʱ��
	TIM_DECL uint32_t GetGroupMemberInfoJoinTime(TIMGroupMemberInfoHandle handle);
	//��Ա����
	TIM_DECL uint32_t GetGroupMemberInfoRole(TIMGroupMemberInfoHandle handle);
	TIM_DECL uint32_t GetGroupMemberInfoMsgFlag(TIMGroupMemberInfoHandle handle);
	TIM_DECL uint32_t GetGroupMemberInfoShutupTime(TIMGroupMemberInfoHandle handle);
	TIM_DECL int GetGroupMemberInfoNameCard(TIMGroupMemberInfoHandle handle, char* name_card, uint32_t* len);
	TIM_DECL TIMGroupCustomInfoHandle GetGroupMemberInfoCustomInfo(TIMGroupMemberInfoHandle handle);

	TIM_DECL TIMGetGroupPendencyOptHandle CreateGetGroupPendencyOptHandle();
	TIM_DECL void DestroyGetGroupPendencyOptHandle(TIMGetGroupPendencyOptHandle opt);
	TIM_DECL int SetStartTime4GetGroupPendencyOptHandle(TIMGetGroupPendencyOptHandle opt, uint64_t start_time);
	TIM_DECL int SetMaxCount4GetGroupPendencyOptHandle(TIMGetGroupPendencyOptHandle opt, uint32_t max);

	//��ȡ���ʱ�� ��λ��ms δ0ʱȫ������
	TIM_DECL TIMGroupPendencyMetaHandle CloneGroupPendencyMetaHandle(TIMGroupPendencyMetaHandle meta);
	TIM_DECL void DestroyGroupPendencyMetaHandle(TIMGroupPendencyMetaHandle meta);
	TIM_DECL uint64_t GetNextStartTime4GroupPendencyMetaHandle(TIMGroupPendencyMetaHandle meta);
	//�Ѷ�ʱ�� ��λ:ms
	TIM_DECL uint64_t GetReportTimeStamp4GroupPendencyMetaHandle(TIMGroupPendencyMetaHandle meta);
	TIM_DECL uint32_t GetUnreadCount4GroupPendencyMetaHandle(TIMGroupPendencyMetaHandle meta);

	TIM_DECL TIMGroupPendencyItemHandle CloneGroupPendencyItemHandle(TIMGroupPendencyItemHandle handle);
	TIM_DECL void DestroyGroupPendencyItemHandle(TIMGroupPendencyItemHandle handle);
	TIM_DECL int GetGroupId4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle, char* id, uint32_t* id_len);
	//��ȡ������ID
	TIM_DECL int GetReqId4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle, char* id, uint32_t* id_len);
	//��ȡ�о���ID
	TIM_DECL int GetRspId4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle, char* id, uint32_t* id_len);
	//δ������ʱ�� ��λ:ms
	TIM_DECL uint64_t GetTime4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle);
	TIM_DECL TIMGroupPendencyType GetPendencyType4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle);
	TIM_DECL TIMGroupPendencyHandleFlag GetHandleFlag4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle);
	TIM_DECL TIMGroupPendencyHandleResult GetHandleResult4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle);

	TIM_DECL int GetApplyInviteMsg4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle, char* msg, uint32_t* msg_len);
	TIM_DECL int GetReqUserData4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle, char* data, uint32_t* data_len);
	TIM_DECL int GetApprovalMsg4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle, char* msg, uint32_t* msg_len);
	TIM_DECL int GetRspUserData4GroupPendencyItemHandle(TIMGroupPendencyItemHandle handle, char* data, uint32_t* data_len);

// callback for interface
	typedef void (*CBCreateGroupOnSuccess) (const char* group_id, void* data);
	typedef void (*CBCreateGroupOnError) (int code, const char* desc, void* data);
	typedef struct _TIMCallBack_CreateGroup
	{
		CBCreateGroupOnSuccess OnSuccess;
		CBCreateGroupOnError OnError;
		void* data;
	}TIMCreateGroupCB;

	typedef void (*CBDeleteGroupMemberOnSuccess) (TIMGroupMemberResultHandle* handle_array, uint32_t array_size, void* data);
	typedef void (*CBDeleteGroupMemberOnError) (int code, const char* desc, void* data);

	typedef struct _TIMCallBack_DeleteGroupMember
	{
		CBDeleteGroupMemberOnSuccess OnSuccess;
		CBDeleteGroupMemberOnError OnError;
		void* data;
	}TIMDeleteGroupMemberCB;

	typedef TIMDeleteGroupMemberCB TIMInviteGroupMemberCB;

	typedef void (*CBGetGroupListOnSuccess) (TIMGroupBaseInfoHandle* handle_array, uint32_t array_size, void* data);
	typedef void (*CBGetGroupListOnError) (int code, const char* desc, void* data);

	typedef struct _TIMCallBack_GetGroupList
	{
		CBGetGroupListOnSuccess OnSuccess;
		CBGetGroupListOnError OnError;
		void* data;
	}TIMGetGroupListCB;


	typedef void (*CBGetGroupDetailInfoOnSuccess) (TIMGroupDetailInfoHandle* handle_array, uint32_t array_size, void* data);
	typedef void (*CBGetGroupDetailInfoOnError) (int code, const char* desc, void* data);

	typedef struct _TIMCallBack_GetGroupDetailInfo
	{
		CBGetGroupDetailInfoOnSuccess OnSuccess;
		CBGetGroupDetailInfoOnError OnError;
		void* data;
	}TIMGetGroupDetailInfoCB;

	typedef void (*CBGetGroupMemberInfoOnSuccess) (TIMGroupMemberInfoHandle* handle_array, uint32_t array_size, void* data);
	typedef void (*CBGetGroupMemberInfoOnError) (int code, const char* desc, void* data);

	typedef void(*CBGetGroupMemberInfoOnSuccessV2) (uint64_t next_seq, TIMGroupMemberInfoHandle* handle_array, uint32_t array_size, void* data);
	typedef void(*CBGetGroupMemberInfoOnErrorV2) (int code, const char* desc, void* data);

	typedef struct _TIMCallBack_GetGroupMemberInfo
	{
		CBGetGroupMemberInfoOnSuccess OnSuccess;
		CBGetGroupMemberInfoOnError OnError;
		void* data;
	}TIMGetGroupMemberInfoCB;

	typedef struct _TIMCallBack_GetGroupMemberInfoV2
	{
		CBGetGroupMemberInfoOnSuccessV2 OnSuccess;
		CBGetGroupMemberInfoOnErrorV2 OnError;
		void* data;
	}TIMGetGroupMemberInfoCBV2;

	typedef void(*CBSearcgGroupByNameOnSuccess) (uint64_t total_num, TIMGroupDetailInfoHandle* handle_array, uint32_t array_size, void* data);
	typedef void(*CBSearcgGroupByNameOnError) (int code, const char* desc, void* data);

	typedef struct _TIMCallBack_SearcgGroupByName
	{
		CBSearcgGroupByNameOnSuccess OnSuccess;
		CBSearcgGroupByNameOnError OnError;
		void* data;
	}TIMSearcgGroupByNameCB;

	typedef void(*CBGetGroupPendencyOnSuccess)(TIMGroupPendencyMetaHandle meta, TIMGroupPendencyItemHandle* items, uint32_t item_size, void* data);
	typedef void(*CBGetGroupPendencyOnError)(int code, const char* desc, void* data);
	typedef struct _TIMCallBack_GetGroupPendency
	{
		CBGetGroupPendencyOnSuccess OnSuccess;
		CBGetGroupPendencyOnError OnError;
		void* data;
	}TIMGetGroupPendencyCB;

//GROUPS INTERFACE
	/**
	Description:	����Ⱥ��
	@param	[in]	group_type		Ⱥ��
	@param	[in]	members			��ԱID����
	@param	[in]	members_count	��Ա����
	@param	[in]	group_name		Ⱥ����
	@param	[in]	callback		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMCreateGroup(const char* group_type, const char** members, uint32_t members_count, const char* group_name, TIMCreateGroupCB* callback);
	/**
	Description:	����ָ������Ⱥ��
	@param	[in]	handle		����Ⱥָ������
	@param	[in]	callback	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMCreateGroupV2(TIMNewGroupInfoHandle handle, TIMCreateGroupCB* callback);

	/**
	Description:	��ɢȺ��
	@param	[in]	groupid		Ⱥ��ID
	@param	[in]	callback	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMDeleteGroup(const char* groupid, TIMCommCB * callback);
	/**
	Description:	ɾ��Ⱥ���Ա 
	@param	[in]	groupid		Ⱥ��ID
	@param	[in]	members		Ⱥ��ԱID
	@param	[in]	members_count	Ⱥ��Ա����
	@param	[in]	reason			����ԭ������
	@param	[in]	reason_len		ԭ�򳤶�
	@param	[in]	callback		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMDeleteGroupMember(const char* groupid, const char** members, uint32_t members_count, const char* reason, uint32_t reason_len, TIMDeleteGroupMemberCB* callback);
	/**
	Description:	�����û���Ⱥ
	@param	[in]	groupid			ȺID
	@param	[in]	members			��ԱID����
	@param	[in]	members_count	��Ա����
	@param	[in]	callback		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMInviteGroupMember(const char* groupid, const char** members, uint32_t members_count, TIMInviteGroupMemberCB* callback);

	/**
	Description:	�������Ⱥ��
	@param	[in]	groupid		Ⱥ��ID
	@param	[in]	hello_msg	������Ϣ
	@param	[in]	callback	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMApplyJoinGroup(const char* groupid, const char* hello_msg, TIMCommCB* callback);
	/**
	Description:	�˳�Ⱥ��
	@param	[in]	groupid		Ⱥ��ID	
	@param	[in]	callback	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMQuitGroup(const char* groupid, TIMCommCB* callback);
	/**
	Description:	��ȡ�����Ⱥ���б�
	@param	[in]	callback �ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetGroupList(TIMGetGroupListCB *callback);
	/**
	Description:	��ȡȺ������
	@param	[in]	groupids	Ⱥ��ID����
	@param	[in]	group_num	Ⱥ��ID����
	@param	[in]	flag		Ⱥ��Ϣ���λ
	@param	[in]	custom		Ⱥ��չ��Ϣ
	@param	[in]	callback	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetGroupPublicInfo(const char** groupids, const uint32_t group_num, const TIMGetGroupBaseInfoFlag flag, TIMGroupCustomInfoHandle custom, TIMGetGroupDetailInfoCB *callback);
	/**
	Description:	�޸�Ⱥ��
	@param	[in]	groupid		ȺID
	@param	[in]	groupname	�޸ĺ��Ⱥ��
	@param	[in]	name_len	�޸ĺ��Ⱥ������
	@param	[in]	callback	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMModifyGroupName(const char* groupid, const char* groupname, uint32_t name_len, TIMCommCB * callback);
	/**
	Description:	�޸�Ⱥ��� 
	@param	[in]	groupid			ȺID
	@param	[in]	introduction	Ⱥ���
	@param	[in]	introduction_lenȺ��鳤��
	@param	[in]	callback		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMModifyGroupIntroduction(const char* groupid, const char* introduction, uint32_t introduction_len, TIMCommCB * callback);
	/**
	Description:	�޸�Ⱥ����
	@param	[in]	groupid			ȺID
	@param	[in]	notification	Ⱥ����
	@param	[in]	notification_lenȺ���泤��
	@param	[in]	callback		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMModifyGroupNotification(const char* groupid, const char* notification, uint32_t notification_len, TIMCommCB * callback);
	/**
	Description:	�޸�Ⱥͷ��
	@param	[in]	groupid			ȺID
	@param	[in]	face_url		Ⱥͷ��URL
	@param	[in]	face_url_len	Ⱥͷ��URL����
	@param	[in]	callback		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMModifyGroupFaceUrl(const char* groupid, const char* face_url, uint32_t face_url_len, TIMCommCB * callback);
	/**
	Description:	�޸ļ�Ⱥѡ��
	@param	[in]	groupid		ȺID
	@param	[in]	opt			��Ⱥѡ��
	@param	[in]	callback	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMModifyGroupAddOpt(const char* groupid, const TIMGroupAddOpt opt, TIMCommCB * callback);
	/**
	Description:	ת��Ⱥ��
	@param	[in]	groupid		Ⱥ��ID
	@param	[in]	new_owner_id	��Ⱥ��ID
	@param	[in]	callback		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMModifyGroupOwner(const char* groupid, const char* new_owner_id, TIMCommCB* callback);

	TIM_DECL void TIMModifyGroupDatilInfoV2(TIMModifyGroupBaseInfoOptionHandle opt, TIMCommCB * callback);
 	/**
 	Description:	��ȡȺ��Ϣ
 	@param	[in]	groupids		Ⱥ��ID����
 	@param	[in]	groupid_count	Ⱥ��ID����
 	@param	[in]	cb				�ص�
 	@return			void
 	@exception      none
 	*/
 	TIM_DECL void TIMGetGroupDetailInfo(const char** groupids, uint32_t groupid_count, TIMGetGroupDetailInfoCB * cb);
	TIM_DECL void TIMGetGroupDetailInfoV2(TIMGetGroupDetailInfoOptionHandle opt, TIMGetGroupDetailInfoCB * cb);
 	/**
 	Description:	��ȡȺ��Ա�б� 
 	@param	[in]	groupid	Ⱥ��ID
 	@param	[in]	cb		�ص�
 	@return			void
 	@exception      none
 	*/
 	TIM_DECL void TIMGetGroupMembers(const char* groupid, TIMGetGroupMemberInfoCB *cb);
	TIM_DECL void TIMGetGroupMembersV2(const char* groupid, TIMGetGroupMemInfoFlag flag, TIMGroupCustomInfoHandle custom, uint64_t next_seq, TIMGetGroupMemberInfoCBV2 *cb);
	/**
	Description:	��ȡָ�����ͳ�Ա
	@param	[in]	groupid		ȺID
	@param	[in]	flag		��ȡ��־ʹ��λ
	@param	[in]	role_filter	filter��־ ����ȡȫ�� ֻȺ�� ֻ����Ա ֻ��ͨ��Ա��
	@param	[in]	custom		Ⱥ��Ա�Զ�����Ϣ��־
	@param	[in]	next_seq	��ȡseq �û���ҳ
	@param	[in]	cb
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetGroupMembersByFilter(const char* groupid, TIMGetGroupMemInfoFlag flag, TIMGroupMemRoleFilter role_filter, TIMGroupCustomInfoHandle custom, uint64_t next_seq, TIMGetGroupMemberInfoCBV2 *cb);
	/**
	Description:	�޸�ȺԱ��Ϣ
	@param	[in]	opt			�޸�ȺԱ��Ϣѡ��
	@param	[in]	callback	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMModifyGroupMemberInfo(TIMModifyGroupMemberInfoOptionHandle opt, TIMCommCB * callback);
	/**
	Description:	ͨ��������Ϣ��ȡȺ���ϣ���ָ���ֶ���ȡ��
	@param	[in]	name		Ⱥ��
	@param	[in]	name_len	Ⱥ������
	@param	[in]	flag		Ҫ��ȡ��Ⱥ������Ϣflag
	@param	[in]	custom		Ҫ��ȡ��Ⱥ��չ��Ϣ
	@param	[in]	page_num	��ҳ��
	@param	[in]	num_perpage	ÿҳȺ����Ŀ
	@param	[in]	cb			�ص�
	@return			TIM_DECL void
	@exception      none
	*/
	TIM_DECL void TIMSearchGroupByName(const char* name, uint32_t name_len, TIMGetGroupBaseInfoFlag flag, TIMGroupCustomInfoHandle custom, 
		uint32_t page_num, uint32_t num_perpage, TIMSearcgGroupByNameCB* cb);

	/**
	Description:	��ȡȺδ�������Ϣ
	@param	[in]	opt			��ȡȺδ�����ѡ��
	@param	[in]	callback
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetGroupPendency(TIMGetGroupPendencyOptHandle opt, TIMGetGroupPendencyCB* callback);
	/**
	Description:	�ϱ�Ⱥδ���Ѷ�
	@param	[in]	report_time	�ϱ��Ѷ�ʱ���
	@param	[in]	cb			�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGroupPendencyReport(uint64_t report_time, TIMCommCB* cb);
	/**
	Description:	����Ⱥδ������
	@param	[in]	pendency_item_handle	δ����Ŀ
	@param	[in]	result					�������
	@param	[in]	approval_msg			������Ϣ
	@param	[in]	msg_len					������Ϣ��
	@param	[in]	user_data				�����Զ�������
	@param	[in]	data_len				�����Զ������ݳ���
	@param	[in]	cb						�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGroupHandlePendency(TIMGroupPendencyItemHandle pendency_item_handle, TIMGroupPendencyHandleResult result, const char* approval_msg, uint32_t msg_len, const char* user_data, uint32_t data_len, TIMCommCB* cb);
};

#endif

