#ifndef _TIM_CONV_C_H_
#define _TIM_CONV_C_H_

#include "tim_comm_c.h"
#include "tim_msg_c.h"

typedef void* TIMDraftHandle;
extern "C"
{
	TIM_DECL TIMConversationHandle	CreateConversation();
	TIM_DECL void					DestroyConversation(TIMConversationHandle handle);
	TIM_DECL TIMDraftHandle			CreateDraft();
	TIM_DECL void					DestroyDraft(TIMDraftHandle handle);
	TIM_DECL void					SetDraftElems(TIMDraftHandle handle, TIMMsgElemHandle* elems, uint32_t num);
	TIM_DECL void					SetDraftText(TIMDraftHandle handle, const char* text, uint32_t len);
	TIM_DECL int					GetDraftElems(TIMDraftHandle handle, TIMMsgElemHandle* elems, uint32_t* num);
	TIM_DECL int					GetDraftText(TIMDraftHandle handle, char* text, uint32_t* len);



	/**
	Description:	������ͨ��Ϣ
	@param	[in]	conv_handle		conversation handle
	@param	[in]	msg_handle		msg handle
	@param	[in]	callback		�û��ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void SendMsg(TIMConversationHandle conv_handle, TIMMessageHandle msg_handle, TIMCommCB *callback);
	/**
	Description:	���ͺ����Ϣ
	@param	[in]	conv_handle		conversation handle
	@param	[in]	msg_handle		msg handle
	@param	[in]	callback		�û��ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void SendRedPacketMsg(TIMConversationHandle conv_handle, TIMMessageHandle msg_handle, TIMCommCB *callback);
	
	/**
	Description:	���͵�����Ϣ
	@param	[in]	conv_handle		conversation handle
	@param	[in]	msg_handle		msg handle
	@param	[in]	callback		�û��ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void SendLikeMsg(TIMConversationHandle conv_handle, TIMMessageHandle msg_handle, TIMCommCB *callback);

	/**
	Description:	����������Ϣ����������������Ϣ��
	@param	[in]	conv_handle		conversation handle
	@param	[in]	msg_handle		msg handle
	@param	[in]	callback		�û��ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void SendOnlineMsg(TIMConversationHandle conv_handle, TIMMessageHandle msg_handle, TIMCommCB *callback);
	TIM_DECL int  SaveMsg(TIMConversationHandle conv_handle, TIMMessageHandle msg_handle, const char* sender, bool readed);
	/**
	Description:	������ʷ��Ϣ
	@param	[in]	conv_handle	conversation handle
	@param	[in]	msg_handles msg handle����
	@param	[in]	msg_num		msg����
	@return			int	0���ɹ� ��0��ʧ��
	@exception      none
	*/
	TIM_DECL int	ImportMsgs(TIMConversationHandle conv_handle, TIMMessageHandle* msg_handles, uint32_t msg_num);
	/**
	Description:	��ȡ�Ự��Ϣ
	@param	[in]	conv_handle	TIMConversationHandle
	@param	[in]	count		��ȡ����
	@param	[in]	last_msg	�ϴ����һ����Ϣ
	@param	[in]	callback	�û��ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void GetMsgs(TIMConversationHandle conv_handle, int count, TIMMessageHandle last_msg, TIMGetMsgCB * callback);
	/**
	Description:	ͬ����ȡ���N����Ϣ
	@param	[in]	conv_handle	TIMConversationHandle
	@param	[in\out]count		��ȡ����
	@param	[in]	last_msg	�ϴ����һ����Ϣ
	@param	[in]	msgs		TIMMessageHandle buffer
	@return			int
	@exception      none
	*/
	TIM_DECL int  GetMsgsFromCache(TIMConversationHandle conv_handle, int* count, TIMMessageHandle last_msg, TIMMessageHandle* msgs);
	TIM_DECL void GetMsgsForward(TIMConversationHandle conv_handle, int count, TIMMessageHandle from_msg, TIMGetMsgCB * callback);
	/**
	Description:	��ȡ���ػỰ��Ϣ
	@param	[in]	conv_handle	TIMConversationHandle
	@param	[in]	count		��ȡ����
	@param	[in]	last_msg	�ϴ����һ����Ϣ
	@param	[in]	callback	�û��ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void GetLocalMsgs(TIMConversationHandle conv_handle, int count, TIMMessageHandle last_msg, TIMGetMsgCB * callback);
	TIM_DECL void DeleteLocalMsg(TIMConversationHandle conv_handle, TIMCommCB *callback);
	/**
	Description:	�Ѷ��ϱ� ImSDK��ѱ�last_read_msgʱ��������Ϣ���Ϊ�Ѷ���Ϣ
	@param	[in]	conv_handle		TIMConversationHandle
	@param	[in]	last_read_msg	�ϱ��Ѷ�����Ϣ
	@return			void
	@exception      none
	*/
	TIM_DECL void SetReadMsg(TIMConversationHandle conv_handle, TIMMessageHandle last_read_msg);
 	/**
 	Description:	��ȡ��ǰ�Ự��δ����Ϣ������
 	@param	[in]	conv_handle	TIMConversationHandle
 	@return			uint64_t	δ����
 	@exception      none
 	*/
 	TIM_DECL uint64_t GetUnreadMessageNum(TIMConversationHandle conv_handle);

	TIM_DECL int GetConversationPeer(TIMConversationHandle conv_handle, char* buf, uint32_t* len);
	TIM_DECL const TIMConversationType GetConversationType(TIMConversationHandle conv_handle);

	TIM_DECL void SetConversationPeer(TIMConversationHandle conv_handle, const char* peer);
	TIM_DECL void SetConversationType(TIMConversationHandle conv_handle, const TIMConversationType type);

	TIM_DECL int ReplaceMessage(TIMConversationHandle conv_handle, TIMMessageHandle msg_handle, TIMMessageHandle *msg_handles, uint32_t msg_num);

	TIM_DECL int DeleteRambleMessage(TIMConversationHandle conv_handle, TIMMessageHandle *msg_handles, int msg_num, TIMRambleCB * cb);

	/**
	Description:	���ûỰ�ݸ�
	@param	[in]	conv_handle
	@param	[in]	draft	�ݸ�����	
	@param	[in]	draft_len	�ݸ峤��
	@return			TIM_DECL void
	@exception      none
	*/
	TIM_DECL void SetConversationDraft(TIMConversationHandle conv_handle, TIMDraftHandle draft );

	//TIM_DECL void SetConversationDraft(TIMConversationHandle conv_handle, TIMMsgElemHandle, uint32_t draft_len);

	/**
	Description:	��ȡ�Ự�ݸ�
	@param	[in]	conv_handle
	@param	[in]	draft	�ݸ�����
	@param	[in/out]	len		�ݸ峤��
	@return			TIM_DECL int
	@exception      none
	*/
	TIM_DECL int GetConversationDraft(TIMConversationHandle conv_handle, TIMDraftHandle draft);

	/**
	Description:	���ûỰ�洢
	@param	[in]	conv_handle
	@return			TIM_DECL void
	@exception      none
	*/
	TIM_DECL void DisableConversationStorage(TIMConversationHandle conv_handle);

	/**
	Description:	���ݶ�λ�� ������Ϣ
	@param	[in]	conv_handle
	@param	[in]	pLocatorArray ��Ϣ��λ������
	@param	[in]	arrSize       �����С
	@param	[in]	callback	�û��ص�,���ز��ҵ�����Ϣ
	@return			TIM_DECL void
	@exception      none
	*/
	TIM_DECL void FindMessages(TIMConversationHandle conv_handle, TIMMessageLocator* pLocatorArray, int arrSize, TIMGetMsgCB * callback);

};



#endif