#ifndef _TIM_FRIENDSHIP_PROXY_H_
#define _TIM_FRIENDSHIP_PROXY_H_

#include "tim_friend_c.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
* ���Ѵ���״̬
*/
typedef enum {
	/**
	*  δ��ʼ��
	*/
	TIM_FRIENDSHIP_STATUS_NONE = 0x0,
		/**
		*  ���ںͷ�����ͬ������
		*/
		TIM_FRIENDSHIP_STATUS_SYNCING = 0x1,
		/**
		*  �Ѿ��������ͬ��
		*/
		TIM_FRIENDSHIP_STATUS_SYNCED = 0x2,
		/**
		*  ����ͬ��ʧ��
		*/
		TIM_FRIENDSHIP_STATUS_FAILED = 0x3,
}TIM_FRIENDSHIP_PROSY_STATUS;




typedef void(*CBOnProxyStatusChange) (TIM_FRIENDSHIP_PROSY_STATUS status);
typedef void(*CBOnAddFriends)(const char** id, uint32_t num);
typedef void(*CBOnDelFriends)(const char**id, uint32_t num);
typedef void(*CBOnFriendProfileUpdate)(const char** id, uint32_t num);


typedef struct 
{
	CBOnProxyStatusChange onProxyStatusChange;
	CBOnAddFriends onAddFriends;
	CBOnDelFriends onDelFriends;
	CBOnFriendProfileUpdate onFriendProfileUpdate;
}TIMFriendshipPorxyListener;

typedef struct _T_TIMFriendshipProxyConfig
{
	uint64_t flags;
	const char** key;
	uint32_t key_num;
}TIMFriendshipProxyConfig;

TIM_DECL void TIMInitFriendshipSetting(TIMFriendshipProxyConfig* config);
/**
Description:	���ú��Ѵ���ص�
@param	[in]	listener	�ص�
@return			void
@exception      none
*/
TIM_DECL void TIMSetFriendshipProxyListener(TIMFriendshipPorxyListener* listener);
/**
Description:	�����洢
@return			void
@exception      none
*/
TIM_DECL void TIMEnableFriendshipProxy();

//sdk����һ���ڴ�
/**
Description:	�ڴ���ͬ����ȡ��ϵ����������
@param	[in/out]	handles profile handle ����
@param	[in/out]	num	��ȡ����
@return			int
@exception      none
*/
TIM_DECL int TIMFriendProxyGetFriendList(TIMProfileHandle* handles, uint32_t* num);


#ifdef __cplusplus
}
#endif

#endif