#ifndef  _TIM_C_H_
#define _TIM_C_H_

#include "tim_comm_c.h"
#include "tim_conv_c.h"
#include "tim_msg_c.h"
#include "tim_status_c.h"


extern "C" 
{
	TIM_DECL int	TIMSwitchEnv(int env);
	TIM_DECL void	TIMSetEnv(int env);
	TIM_DECL int	TIMEnv();
	/**
	Description:	���ý���ģʽ
	@param	[in]	mode	1 �C IM����ģʽ / 0�򲻵��ô˽ӿ� �C ֻ��½ģʽ 
	@return			void
	@exception      none
	*/
	TIM_DECL void	TIMSetMode(int mode);
	TIM_DECL int	TIMMode();
	TIM_DECL int	TIMSetSdkAPPId(int sdk_app_id);
	/**
	Description:	����SDK��־������SDK��д�ļ�������·��
	@param	[in]	path	·��
	@return			void
	@exception      none
	*/
	TIM_DECL void	TIMSetPath(const char* path);
	TIM_DECL int	TIMGetPath(char* path, uint32_t* len);
	TIM_DECL void	TIMSetLogLevel(TIMLogLevel level);
	TIM_DECL void	TIMSetLogPrint(bool flag);
	TIM_DECL bool	TIMGetLogPrint();
	TIM_DECL TIMLogLevel TIMGetLogLevel();
	
	TIM_DECL int	TIMReady();

	/**
	Description:	��ʼ��SDK
	@return			int 0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int	TIMInit();

	/**
	Description:	���ʼ��SDK ж��DLL���˳�����ǰ�����
	@return			int 0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int	TIMUninit();

	/**
	Description:	��¼IM��
	@param	[in]	sdk_app_id		Ӧ��SDKAPPID
	@param	[in]	tim_user		�û���Ϣ
	@param	[in]	user_sig		�û���Կ
	@param	[in]	callback		��¼�ص�
	@return			int	0
	@exception      none
	*/
	TIM_DECL int	TIMLogin(int sdk_app_id, const TIMUserInfo *tim_user, const char* user_sig, TIMCommCB *callback);
	/**
	Description:	�ǳ�IM��
	@param	[in]	callback		�û��ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void	TIMLogout(TIMCommCB *callback);


	/**
	Description:	��ȡ�Ự����
	@return			uint64_t �ػ�����
	@exception      none
	*/
	TIM_DECL uint64_t	TIMGetConversationCount();
	/**
	Description:	��ȡ�Ự���
	@param	[in]	handle		�Ự���
	@param	[in]	type		�Ự����
	@param	[in]	peer		�Ự�Է�ID(������ΪȺ���ͣ���IDΪȺID)
	@return			int 0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int		TIMGetConversation(TIMConversationHandle handle, TIMConversationType type, const char* peer);
	/**
	Description:	ɾ���Ự
	@param	[in]	type	�Ự���ͣ�kCnvC2C ��ʾ���� kCnvGroup ��ʾȺ��
	@param	[in]	peer	�û�identifier ���� Ⱥ��Id
	@return			bool	TRUE:ɾ���ɹ�  FALSE:ɾ��ʧ��
	@exception      none
	*/
	TIM_DECL bool		TIMDeleteConversation(TIMConversationType type, const char* peer);
	/**
	Description:	ɾ���Ự����Ϣ
	@param	[in]	type	�Ự���ͣ�kCnvC2C ��ʾ���� kCnvGroup ��ʾȺ��
	@param	[in]	peer	�û�identifier ���� Ⱥ��Id
	@return			bool	TRUE:ɾ���ɹ�  FALSE:ɾ��ʧ��
	@exception      none
	*/
	TIM_DECL bool		TIMDeleteConversationAndMsgs(TIMConversationType type, const char* peer);
	TIM_DECL int		TIMGetConversationByIndex(TIMConversationHandle handle, uint64_t index);
	TIM_DECL void		TIMUploadLogFile(const char* filename, const char* tag);
	/**
	Description:	���ô洢
	@return			void
	@exception      none
	*/
	TIM_DECL void		TIMDisableStorage();
	/**
	Description:	��¼ʱ��ֹ��ȡ�����ϵ���б�
	@return			void
	@exception      none
	*/
	TIM_DECL void		TIMDisableRecentContact();

	/**
	Description:	��ȡ�����ϵ���б�ʱ��ֹ�׳�onNewMessage
	@return			void
	@exception      none
	*/
	TIM_DECL void		TIMDisableRecentContactNotify();

	/**
	Description:	�����Զ��Ѷ��ϱ���������ã������߱�����ʽ���ûػ����Ѷ��ϱ���
	@return			TIM_DECL void
	@exception      none
	*/
	TIM_DECL void		TIMDisableAutoReport();

	/**
	Description:	�����û�����Ϣ�ص�����
	@param	[in]	callback	����Ϣ�ص�����
	@return			void
	@exception      none
	*/
	TIM_DECL void			TIMSetMessageCallBack(TIMMessageCB *callback);
	TIM_DECL TIMMessageCB *	TIMGetMessageCallBack();

	TIM_DECL void			TIMSetMessageUpdateCallBack(TIMMessageCB *callback);
	TIM_DECL TIMMessageCB *	TIMGetMessageUpdateCallBack();

	TIM_DECL void		TIMSetConnCallBack(TIMConnCB *callback);
	TIM_DECL TIMConnCB*	TIMGetConnCallBack();

	TIM_DECL void		TIMSetKickOfflineCallBack(TIMForceOfflineCB* callback);
	TIM_DECL TIMForceOfflineCB* TIMGetKickOfflineCallBack();

	TIM_DECL void		TIMSetUserSigExpiredCallBack(TIMUserSigExpiredCB* callback);
	TIM_DECL TIMUserSigExpiredCB* TIMGetUserSigExpiredCallBack();

	TIM_DECL void		TIMSetStatusChangeCallBack(TIMStatusChangeCB *callback);
	TIM_DECL TIMStatusChangeCB* TIMGetStatusChangeCallBack();

	TIM_DECL void TIMSetGroupEventListener(TIMGroupEventListener* callback);
	TIM_DECL TIMGroupEventListener* TIMGetGroupEventListener();

	TIM_DECL TIMNetworkStatus TIMGetNetWorkStatus();
	TIM_DECL int64_t TIMGetServerTimeDiff();

	// return �����½�����û���identifier�����δ��¼���ط���
	TIM_DECL int TIMGetLoginUser(char* id, uint32_t* len);

	TIM_DECL const char*TIMGetSDKVersion();

	TIM_DECL void TIMSendBatchC2CMsg(const char** ids, uint32_t num, TIMMessageHandle msg_handle, TIMBBatchOprCB* callback);

};


#endif