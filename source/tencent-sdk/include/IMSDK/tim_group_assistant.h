#ifndef _TIM_GROUP_ASSISTANT_H_
#define _TIM_GROUP_ASSISTANT_H_

#include "tim_comm_c.h"
#include "tim_group_c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void(*CBOnMemberJoin)(const char* groupId, const char** id, uint32_t num);
typedef void(*CBOnMemberQuit)(const char* groupId, const char** id, uint32_t num);
typedef void(*CBOnMemberUpdate)(const char* groupId, const char** id, uint32_t num);

//group info notify
typedef void(*CBOnGroupAdd)(const char* groupId);
typedef void(*CBOnGroupDelete)(const char* groupId);
typedef void(*CBOnGroupUpdate)(const char* groupId);

typedef struct _T_TIMGroupAssistantCallBack
{
	CBOnMemberJoin onMemberJoin;	//�����û�����Ⱥʱ��֪ͨ�ص�
	CBOnMemberQuit onMemberQuit;	//��Ⱥ��Ա��Ⱥʱ��֪ͨ�ص�
	CBOnMemberUpdate onMemberUpdate;//Ⱥ��Ա��Ϣ���µ�֪ͨ�ص�
	CBOnGroupAdd onAdd;				//����Ⱥ��֪ͨ�ص�
	CBOnGroupDelete onQuit;			//��ɢȺ��֪ͨ�ص�
	CBOnGroupUpdate onUpdate;		//Ⱥ���ϸ��µ�֪ͨ�ص�
}TIMGroupAssistantCallBack;

struct TIMUpdateInfoOpt{
	uint64_t flag;
	const char** tag_name;
	uint32_t num;
};

struct TIMGroupSettings{
	TIMUpdateInfoOpt memberInfoOpt;
	TIMUpdateInfoOpt groupInfoOpt;
};

/**
Description:	������ȡ�ֶ�
@param	[in]	config	��ȡ����
@return			void
@exception      none
*/
TIM_DECL void TIMInitGroupSetting(TIMGroupSettings* config);
/**
Description:	����Ⱥ���ϴ洢
@return			void
@exception      none
*/
TIM_DECL void TIMEnableGroupAssistantStorage();
/**
Description:	Ⱥ֪ͨ�ص�
@param	[in]	cb		�ص�
@return			void
@exception      none
*/
TIM_DECL void TIMSetGroupAssistantCallBack(TIMGroupAssistantCallBack* cb);

typedef void* TIMGroupAssistantGroupInfo;
TIM_DECL TIMGroupAssistantGroupInfo CreateTIMGroupAssistantGroupInfo();
TIM_DECL void DestroyTIMGroupAssistantGroupInfo(TIMGroupAssistantGroupInfo info);

TIM_DECL uint32_t TIMGetGroupNumber4TIMGroupAssistantGroupInfo(TIMGroupAssistantGroupInfo info);
TIM_DECL TIMGroupDetailInfoHandle TIMGetGroupDetailInfo4TIMGroupAssistantGroupInfo(TIMGroupAssistantGroupInfo info, uint32_t index);
/**
Description:	ͬ����ȡȺ��Ϣ
@param	[in]	info	ͬ����Ϣ
@return			void
@exception      none
*/
TIM_DECL void TIMGroupAssistantGetGroups(TIMGroupAssistantGroupInfo info);

#ifdef __cplusplus
}
#endif

#endif

