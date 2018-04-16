#ifndef _TIM_MSG_C_H_
#define _TIM_MSG_C_H_


#include "tim_comm_c.h"

// ��Ϣ������
#define TIM_MSG_STATUS_SENDING 1
// ��Ϣ���ͳɹ�
#define TIM_MSG_STATUS_SEND_SUCC 2
// ��Ϣ����ʧ��
#define TIM_MSG_STATUS_SEND_FAIL 3
// ��Ϣ��ɾ��
#define TIM_MSG_STATUS_HAS_DELETED 4
// ��ϢΪ���ص�����Ϣ
#define TIM_MSG_STATUS_LOCAL_IMPORTED 5

#define TIM_MSG_FLAG_NONE		0
#define TIM_MSG_FLAG_GAP		1
#define TIM_MSG_FLAG_DELETED	2


extern "C"
{
	typedef void* TIMOfflineInfoHandle;
	typedef void* TIMMessageHandle;
	typedef void* TIMMessageOptResultHandle;
	/**
	Description:	�û�����Ϣ�ص�����
	@param	[in]	handles		TIMMessageHandle ����ָ��
	@param	[in]	msg_num		TIMMessageHandle �����С
	@param	[in]	data		�û��Զ�������
	@return			void
	@exception      none
	*/
	typedef void (*CBOnNewMessage) (TIMMessageHandle* handles, uint32_t msg_num, void* data);
	typedef struct _TIMMessageCB_C
	{
		CBOnNewMessage OnNewMessage;
		void* data;
	}TIMMessageCB;

	typedef void (*CBGetMsgOnSuccess) (TIMMessageHandle* handle_array, int size, void* data);
	typedef void (*CBGetMsgOnError) (int code, const char* desc, void* data);
	typedef struct _TIMGetMsgCallBack_c
	{
		CBGetMsgOnSuccess OnSuccess;
		CBGetMsgOnError OnError;
		void* data;
	}TIMGetMsgCB;

	typedef void(*CBGetMsgDataOnSuccess) (const char* buf, uint32_t len, void* data);
	typedef void(*CBGetMsgDataOnError) (int code, const char* desc, void* data);

	typedef struct _TIMGetMsgDataCallBack_c
	{
		CBGetMsgDataOnSuccess  OnSuccess;
		CBGetMsgDataOnError	OnError;
		void*		data;
	}TIMGetMsgDataCB;

	typedef void (*CBRambleSuccess) (TIMMessageOptResultHandle* handles, int size, void* data);
	typedef void (*CBRambleOnError) (int code, const char* desc, void* data);
	typedef struct _TIMRambleCallBack_c
	{
		CBRambleSuccess  OnSuccess;
		CBRambleOnError	OnError;
		void*		data;
	}TIMRambleCB;


	typedef void* TIMMsgElemHandle;
	typedef void* TIMMsgTextElemHandle;
	typedef void* TIMMsgImageElemHandle;
	typedef void* TIMMsgCustomElemHandle;
	typedef void* TIMMsgFaceElemHandle;
	typedef void* TIMMsgSoundElemHandle; 
	typedef void* TIMMsgFileElemHandle;
	typedef void* TIMMsgGroupTipsElemHandle; //For Only Recv Group Tips Msg
	typedef void* TIMMsgGroupReportElemHandle;//For Only Recv Group Report Msg
	typedef void* TIMMsgSNSSystemElemHandle; //SNS��ϵ�����ϵͳ֪ͨ
	typedef void* TIMMsgProfileSystemElemHandle; //���ϱ��ϵͳ֪ͨ
	typedef void* TIMMsgVideoElemHandle;//��Ƶ��Ϣ

	TIM_DECL TIMOfflineInfoHandle CreateTIMTIMOfflineInfo();
	TIM_DECL void DestroyTIMTIMOfflineInfo(TIMOfflineInfoHandle handle);
	TIM_DECL int GetExt4TIMTIMOfflineInfo(TIMOfflineInfoHandle handle, char* ext, uint32_t* len);
	TIM_DECL int GetDesc44TIMTIMOfflineInfo(TIMOfflineInfoHandle handle, char* desc, uint32_t* len);
	TIM_DECL TIMOfflinePushFlag GetFlag4TIMTIMOfflineInfo(TIMOfflineInfoHandle handle);
	TIM_DECL int GetSound4AndroidTIMTIMOfflineInfo(TIMOfflineInfoHandle handle, char* sound, uint32_t* len);
	TIM_DECL int GetSound4iOSTIMTIMOfflineInfo(TIMOfflineInfoHandle handle, char* sound, uint32_t* len);

	TIM_DECL void SetExt4TIMTIMOfflineInfo(TIMOfflineInfoHandle handle, const char* ext, uint32_t len);
	TIM_DECL void SetDesc4TIMTIMOfflineInfo(TIMOfflineInfoHandle handle, const char* desc, uint32_t len);
	TIM_DECL void SetFlag4TIMTIMOfflineInfo(TIMOfflineInfoHandle handle, TIMOfflinePushFlag flag);

	//Android������������
	TIM_DECL void SetSound4AndroidTIMTIMOfflineInfo(TIMOfflineInfoHandle handle, const char* sound, uint32_t len);
	TIM_DECL void SetTitle4AndroidTIMTIMOfflineInfo(TIMOfflineInfoHandle handle, const char* title, uint32_t len);
	TIM_DECL void SetNotifyMode4AndroidTIMTIMOfflineInfo(TIMOfflineInfoHandle handle, uint32_t notifyMode);       //����֪ͨģʽ��δ���û�0-��֪ͨͨ����Ϣ(���֪ͨ��Ϣ�󲻻��Ӧ�ûص�); 0x1-�Զ�����Ϣ�����֪ͨ��Ϣ����Ӧ�ûص���
	
	//iOS������������
	TIM_DECL void SetSound4iOSTIMTIMOfflineInfo(TIMOfflineInfoHandle handle, const char* sound, uint32_t len);
	TIM_DECL void SetBadge4AndroidTIMTIMOfflineInfo(TIMOfflineInfoHandle handle, uint32_t badgeMode);            // 0:������ʾ 1:������badge


	
	TIM_DECL TIMMessageHandle	CreateTIMMessage();
	TIM_DECL int				DestroyTIMMessage(TIMMessageHandle handle);
	TIM_DECL TIMMessageHandle	CloneTIMMessage(TIMMessageHandle handle);
	TIM_DECL int				AddElem(TIMMessageHandle msg, TIMMsgElemHandle elem);
	/**
	Description:	��ȡMessage�а�����elem����
	@param	[in]	handle		��Ϣ���
	@return			int			elem����
	@exception      none
	*/
	TIM_DECL int				GetElemCount(TIMMessageHandle handle);
	/**
	Description:	��ȡMessage�а�����ָ��elem���
	@param	[in]	handle		��Ϣ���
	@param	[in]	index		elem����
	@return			TIMMsgElemHandle	elem���,ʹ�����,�����DestroyElem�ͷ�
	@exception      none
	*/
	TIM_DECL TIMMsgElemHandle	GetElem(TIMMessageHandle handle, int index);
	TIM_DECL int GetConversationFromMsg(TIMConversationHandle conv, TIMMessageHandle msg);
	/**
	Description:	��ǰ��Ϣ��ʱ���
	@param	[in]	handle	TIMMessageHandle
	@return			uint32_t	ʱ���
	@exception      none
	*/
	TIM_DECL uint32_t			GetMsgTime(TIMMessageHandle handle);
	/**
	Description:	�Ƿ��ͷ�
	@param	[in]	handle	TIMMessageHandle
	@return			bool	TRUE ��ʾ�Ƿ�����Ϣ    FALSE ��ʾ�ǽ�����Ϣ
	@exception      none
	*/
	TIM_DECL bool				IsMsgFromSelf(TIMMessageHandle handle);
	/**
	Description:	�Ƿ��Ѷ�
	@param	[in]	handle	TIMMessageHandle
	@return			bool	�Ѷ���־
	@exception      none
	*/
	TIM_DECL bool				IsMsgRead(TIMMessageHandle handle);
	/**
	Description:	��ȡ��Ϣ״̬
	@param	[in]	handle TIMMessageHandle
	@return			int
	@exception      none
	*/
	TIM_DECL int				GetMsgStatus(TIMMessageHandle handle);
	TIM_DECL uint64_t			GetMsgFlag(TIMMessageHandle handle);
	/**
	Description:	��ȡ���ͷ�
	@param	[in]	handle	TIMMessageHandle
	@param	[in]	buf		���ͷ�ID buffer
	@param	[in]	len		���ͷ�ID ��
	@return			int		0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int				GetMsgSender(TIMMessageHandle handle, char* buf, uint32_t* len);
	/**
	Description:	��ϢId ��󳤶�30
	@param	[in]	handle	TIMMessageHandle
	@param	[in]	id		id buffer
	@param	[in\out]len		id ����
	@return			int		0:�ɹ�
	@exception      none
	*/
	TIM_DECL int				GetMsgID(TIMMessageHandle handle, char* id, uint32_t* len);
	/**
	Description:	��ȡ��Ϣ�Ĳ��Ҳ���(��λ��)���ö�λ���������Ҹ���Ϣ
	@param	[in]	handle	 TIMMessageHandle
	@param	[in\out]pLocator ��λ��
	@return			int		 0:�ɹ�
	@exception      none
	*/
	TIM_DECL int				GetMsgLocator(TIMMessageHandle handle, TIMMessageLocator* pLocator);
	/**
	Description:	ɾ����Ϣ��ע��������޸�״̬
	@param	[in]	handle	TIMMessageHandle
	@return			bool	TRUE �ɹ�
	@exception      none
	*/
	TIM_DECL bool				RemoveMsg(TIMMessageHandle handle);
	/**
	Description:	�ӱ������ݿ�ɾ����Ϣ��ע��Ⱥ����Ϣͨ��getMessage�ӿڻ��svrͬ��������
	@param	[in]	handle	TIMMessageHandle
	@return			bool	TRUE �ɹ�
	@exception      none
	*/
	TIM_DECL bool				DelMsgFromStorage(TIMMessageHandle handle);
	/**
	Description:	��ȡCustomInt
	@param	[in]	handle	TIMMessageHandle
	@return			int		CustomInt
	@exception      none
	*/
	TIM_DECL int				GetMsgCustomInt(TIMMessageHandle handle);
	/**
	Description:	��ȡCustomData
	@param	[in]	handle	TIMMessageHandle
	@param	[in]	str		CustomData buffer
	@param	[in]	len		CustomData ����
	@return			int		0:�ɹ� ��0:ʧ��
	@exception      none	
	*/
	TIM_DECL int				GetMsgCustomStr(TIMMessageHandle handle, char* str, uint32_t* len);
	/**
	Description:	�����Զ������ݣ�Ĭ��Ϊ0
	@param	[in]	handle	TIMMessageHandle
	@param	[in]	custom	�Զ�������
	@return			bool	TRUE �ɹ�
	@exception      none
	*/
	TIM_DECL bool				SetMsgCustomInt(TIMMessageHandle handle, int custom);
	/**
	Description:	�����Զ������ݣ�Ĭ��Ϊ""
	@param	[in]	handle	TIMMessageHandle
	@param	[in]	custom	CustomData buffer
	@param	[in]	len		CustomData ����
	@return			bool	TRUE �ɹ�
	@exception      none
	*/
	TIM_DECL bool				SetMsgCustomStr(TIMMessageHandle handle, const char* custom, uint32_t len);
	/**
	Description:	��ȡ��ϢuniqueId
	@param	[in]	handle		TIMMessageHandle
	@return			uint64_t	uniqueId
	@exception      none
	*/
	TIM_DECL uint64_t			GetMsgUniqueID(TIMMessageHandle handle);
	/**
	Description:	��ȡ����������
	@param	[in]	handle	TIMMessageHandle
	@param	[in]	profile	���������� Ŀǰֻ���ֶΣ�identifier��nickname��faceURL��customInfo
	@return			int		0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int				GetSenderProfile(TIMMessageHandle handle, TIMProfileHandle profile);
	/**
	Description:	��ȡ������Ⱥ������
	@param	[in]	handle	TIMMessageHandle
	@param	[in]	member_profile	������Ⱥ������ Ŀǰֻ���ֶΣ�member��nameCard��role��customInfo
	@return			int		0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int				GetSenderGroupMemberProfile(TIMMessageHandle handle, TIMGroupMemberInfoHandle member_profile);

	/**
	Description:	������ϢתΪ������Ϣ�� ���ڵ�����ʷ��Ϣ
					���ò������øı���Ϣ״̬��
	@param	[in]	handle
	@return			bool	TRUE���ɹ� FALSE:ʧ��
	@exception      none
	*/
	TIM_DECL bool	ConvertMsgStatusToLocalImported(TIMMessageHandle handle);
	/**
	Description:	���ñ�����Ϣ��ʱ���������ڱ�����Ϣ
	@param	[in]	handle
	@param	[in]	time	ʱ��
	@return			bool	TRUE���ɹ� FALSE:ʧ��
	@exception      none
	*/
	TIM_DECL bool SetMsgTimestamp(TIMMessageHandle handle, uint32_t time);
	/**
	Description:	���ñ�����Ϣ�ķ��ͷ�ID�������ڱ�����Ϣ
	@param	[in]	handle
	@param	[in]	id		�û�ID
	@return			bool	TRUE���ɹ� FALSE:ʧ��
	@exception      none
	*/
	TIM_DECL bool SetMsgSender(TIMMessageHandle handle, const char* id);

	/**
	Description:	��ȡ��Ϣ���ȼ�
	@param	[in]	handle	
	@return			TIMMsgPriority ���ȼ�
	@exception      none
	*/
	TIM_DECL TIMMsgPriority GetMsgPriority(TIMMessageHandle handle);

	/**
	Description:	������Ϣ���ȼ�
	@param	[in]	handle
	@param	[in]	priority	���ȼ�
	@return			TIM_DECL void
	@exception      none
	*/
	TIM_DECL void SetMsgPriority(TIMMessageHandle handle, TIMMsgPriority priority);

	/**
	Description:	��ȡ��Ϣ֪ͨ���͵Ľӿ�
	@param	[in]	handle
	@return			int 0:RecvAndNotify 1:NotRecv 2:RecvNotNotify
	@exception      none
	*/
	TIM_DECL int GetMsgRecvFlag(TIMMessageHandle handle);

	/**
	Description:	������Ϣ������Ϣ
	@param	[in]	handle
	@param	[in]	offline
	@return			TIM_DECL bool
	@exception      none
	*/
	TIM_DECL bool SetMsgOfflinePushInfo(TIMMessageHandle handle, TIMOfflineInfoHandle offline);

	/**
	Description:	��ȡ��Ϣ������Ϣ
	@param	[in]	handle
	@param	[in]	offline
	@return			TIM_DECL int
	@exception      none
	*/
	TIM_DECL int	GetMsgOfflinePushInfo(TIMMessageHandle handle, TIMOfflineInfoHandle offline);

	TIM_DECL int SerializeMessages(TIMMessageHandle *msg_handles, uint32_t msg_num, char* buf, uint32_t* len);
	TIM_DECL void ParseMessageFromBuffer(char* buf, uint32_t len, int* count, TIMMessageHandle* msgs);

	TIM_DECL TIMMsgTextElemHandle	CreateMsgTextElem();
	TIM_DECL TIMMsgImageElemHandle	CreateMsgImageElem();
	TIM_DECL TIMMsgImageElemHandle	CloneMsgImageElem(TIMMsgImageElemHandle handle);
	TIM_DECL TIMMsgCustomElemHandle CreateCustomElemHandle();

	/**
	Description:	����FaceElemHandle
	@return			TIMMsgFaceElemHandle
	@exception      none
	*/
	TIM_DECL TIMMsgFaceElemHandle	CreateFaceElemHandle();
	TIM_DECL TIMMsgSoundElemHandle	CreateSoundElemHandle();
	TIM_DECL TIMMsgFileElemHandle	CreateFileElemHandle();
	TIM_DECL TIMMsgVideoElemHandle	CreateVideoElemHandle();
	
	TIM_DECL void					DestroyElem(TIMMsgElemHandle handle);
	TIM_DECL void					SetElemType(TIMMsgElemHandle handle, TIMElemType type);
	TIM_DECL TIMElemType			GetElemType(TIMMsgElemHandle handle);

	TIM_DECL void	SetContent(TIMMsgTextElemHandle handle, const char* content);

	/**
	Description:	��ȡ�ı�����
	@param	[in]	handle	TIMMsgTextElemHandle
	@return			uint32_t �ı�����	����ֵ������\0
	@exception      none
	*/
	TIM_DECL uint32_t	GetContentLen(TIMMsgTextElemHandle handle);

	/**
	Description:	��ȡ�ı�����
	@param	[in]	handle	TIMMsgTextElemHandle
	@param	[out]	content	�����洢�ı����ݵĻ���������Ҫ�ȷ����ڴ棨�ڴ��С����Ϊ��GetContentLen�ķ���ֵ+1��
	@param	[in-out]	len	content�������Ĵ�С�����content��NULL��len�᷵��ʵ����Ҫ�Ļ����С��ֵ����GetContentLen�ķ���ֵ+1��
	@return			int	0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int	GetContent(TIMMsgTextElemHandle handle, char* content, uint32_t* len);

	typedef void* TIMImageHandle;
	/**
	Description:	��ȡͼƬ���
	@param	[in]	handle	TIMImageHandle
	@return			TIMImageType	ͼƬ���������Thumb��Large��Original���ֱ��������ͼ����ͼ��ԭͼ
	@exception      none
	*/
	TIM_DECL TIMImageType	GetImageType(TIMImageHandle handle);
	/**
	Description:	��ȡͼƬ��С
	@param	[in]	handle		TIMImageHandle
	@return			uint32_t	��С
	@exception      none
	*/
	TIM_DECL uint32_t	GetImageSize(TIMImageHandle handle);
	/**
	Description:	��ȡͼƬ�߶�
	@param	[in]	handle		TIMImageHandle
	@return			uint32_t	ͼƬ�߶�
	@exception      none
	*/
	TIM_DECL uint32_t	GetImageHeight(TIMImageHandle handle);
	/**
	Description:	��ȡͼƬ���
	@param	[in]	handle		TIMImageHandle
	@return			uint32_t	ͼƬ���
	@exception      none
	*/
	TIM_DECL uint32_t	GetImageWidth(TIMImageHandle handle);
	TIM_DECL uint32_t	GetImageURLLen(TIMImageHandle handle);
	TIM_DECL int		GetImageURL(TIMImageHandle handle, char* url, uint32_t* len);
	/**
	Description:	��ȡͼƬ
	@param	[in]	handle		TIMImageHandle
	@param	[in]	filename	�ļ���	
	@param	[in]	cb			�û��ص�
	@return			int 0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int		GetImageFile(TIMImageHandle handle, char* filename, TIMCommCB* cb);

	TIM_DECL int		GetImageElemUUID(TIMMsgImageElemHandle handle, char* buf, uint32_t* len);
	TIM_DECL int		GetImageElemTaskID(TIMMsgImageElemHandle handle);
	TIM_DECL bool		CancelImageElemUploading(TIMMsgImageElemHandle handle);
	TIM_DECL int		GetImageUploadingProgress(TIMMsgImageElemHandle handle);
	TIM_DECL int		GetImageElemFormat(TIMMsgImageElemHandle handle);
	/**
	Description:	��ȡͼƬ·����������Ϣʱ����Ҫ��ע��
	@param	[in]	handle	TIMMsgImageElemHandle
	@param	[in]	path	·��buffer
	@param	[in\out]len		·������
	@return			int 0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int		GetImageElemPath(TIMMsgImageElemHandle handle, char* path, uint32_t* len);
	/**
	Description:	���÷���ͼƬ·��
	@param	[in]	handle	TIMMsgImageElemHandle
	@param	[in]	path	·��
	@param	[in]	len		·������
	@return			void	
	@exception      none
	*/
	TIM_DECL void		SetImageElemPath(TIMMsgImageElemHandle handle, const char* path, uint32_t len);
	TIM_DECL void		SetImageCompressLeval(TIMMsgImageElemHandle handle, const TIM_IMAGE_COMPRESS_TYPE leval);
	/**
	Description:	��ȡ����Image�ĸ���
	@param	[in]	handle		TIMMsgImageElemHandle
	@return			uint32_t	Image����
	@exception      none
	*/
	TIM_DECL uint32_t	GetImageNum(TIMMsgImageElemHandle handle);
	/**
	Description:	��ȡImageElem������ImageHandle
	@param	[in]	handle	TIMMsgImageElemHandle
	@param	[in]	images	image handle��buffer
	@param	[in]	num		image����
	@return			int 0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int		GetImages(TIMMsgImageElemHandle handle, TIMImageHandle* images, uint32_t* num);


	TIM_DECL uint32_t	GetCustomElemDataLen(TIMMsgCustomElemHandle handle);
	TIM_DECL int		GetCustomElemData(TIMMsgCustomElemHandle handle, char* date, uint32_t* len);
	TIM_DECL void		SetCustomElemData(TIMMsgCustomElemHandle handle, const char* date, uint32_t len);
	TIM_DECL int		GetCustomElemDesc(TIMMsgCustomElemHandle handle, char* desc, uint32_t* len);
	TIM_DECL void		SetCustomElemDesc(TIMMsgCustomElemHandle handle, const char* desc, uint32_t len);
	TIM_DECL int		GetCustomElemExt(TIMMsgCustomElemHandle handle, char* ext, uint32_t* len);
	TIM_DECL void		SetCustomElemExt(TIMMsgCustomElemHandle handle, const char* ext, uint32_t len);
	TIM_DECL int		GetCustomElemSound(TIMMsgCustomElemHandle handle, char* sound, uint32_t* len);
	TIM_DECL void		SetCustomElemSound(TIMMsgCustomElemHandle handle, const char* sound, uint32_t len);


	/**
	Description:	��ȡ�����������û��Զ��壩
	@param	[in]	handle		FaceElemHandle
	@return			int			����
	@exception      none
	*/
	TIM_DECL int		GetFaceElemIndex(TIMMsgFaceElemHandle handle);
	/**
	Description:	��ȡ�û��Զ������ݳ���
	@param	[in]	handle		FaceElemHandle
	@return			uint32_t	�û��Զ������ݳ���
	@exception      none
	*/
	TIM_DECL uint32_t	GetFaceElemDataLen(TIMMsgFaceElemHandle handle);
	/**
	Description:	��ȡ�û��Զ�������
	@param	[in]	handle		FaceElemHandle
	@param	[in]	date		�û��Զ�������Buffer
	@param	[in\out]len			�û��Զ������ݳ���
	@return			int 0:�ɹ� ��0:ʧ��
	@exception      none
	*/
	TIM_DECL int		GetFaceElemData(TIMMsgFaceElemHandle handle, char* date, uint32_t* len);
	/**
	Description:	���ñ�������
	@param	[in]	handle		FaceElemHandle
	@param	[in]	index		����
	@return			void
	@exception      none
	*/
	TIM_DECL void		SetFaceElemIndex(TIMMsgFaceElemHandle handle, int index);
	/**
	Description:	���ñ����Զ�������
	@param	[in]	handle		FaceElemHandle
	@param	[in]	date		�û��Զ�������Buffer
	@param	[in]	len			�û��Զ������ݳ���
	@return			void
	@exception      none
	*/
	TIM_DECL void		SetFaceElemIndexData(TIMMsgFaceElemHandle handle, const char* date, uint32_t len);

	/**
	Description:	������������
	@param	[in]	handle	TIMMsgSoundElemHandle
	@param	[in]	data	��������������
	@param	[in]	len		�������ݳ���
	@return			void
	@exception      none
	*/
	TIM_DECL void		SetSoundElemData(TIMMsgSoundElemHandle handle, const char* data, uint32_t len);
	/**
	Description:	���������ļ�·��������pathʱ�������ϴ������ļ���
	@param	[in]	handle	TIMMsgSoundElemHandle
	@param	[in]	path	�ļ�·��
	@param	[in]	len		·������
	@return			void
	@exception      none
	*/
	TIM_DECL void		SetSoundElemPath(TIMMsgSoundElemHandle handle, const char* path, uint32_t len);
	/**
	Description:	�����������ȣ��룩��������Ϣʱ����
	@param	[in]	handle		TIMMsgSoundElemHandle
	@param	[in]	duration	ʱ��
	@return			void
	@exception      none
	*/
	TIM_DECL void		SetSoundElemDuration(TIMMsgSoundElemHandle handle, const uint32_t duration);
	/**
	Description:	��ȡ�������ȣ��룩
	@param	[in]	handle		TIMMsgSoundElemHandle
	@return			uint32_t	ʱ��	
	@exception      none
	*/
	TIM_DECL uint32_t	GetSoundElemDuration(TIMMsgSoundElemHandle handle);
	TIM_DECL int		GetSoundElemUUID(TIMMsgSoundElemHandle handle, char* id, uint32_t* id_len);
	/**
	Description:	��ȡ��������
	@param	[in]	handle	TIMMsgSoundElemHandle
	@param	[in]	cb		�û��ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void		GetSoundFromSoundElem(TIMMsgSoundElemHandle handle, TIMGetMsgDataCB* cb);

	TIM_DECL void		SetFileElemFileName(TIMMsgFileElemHandle handle, const char* name, uint32_t name_len);
	TIM_DECL int		GetFileElemFileName(TIMMsgFileElemHandle handle, char* name, uint32_t* name_len);
	TIM_DECL void		SetFileElemFilePath(TIMMsgFileElemHandle handle, const char* path, uint32_t path_len);
	TIM_DECL void		SetFileElemData(TIMMsgFileElemHandle handle, const char* data, uint32_t data_len);
	TIM_DECL int		GetFileElemUUID(TIMMsgFileElemHandle handle, char* id, uint32_t* len);
	TIM_DECL void		GetFileFromFileElem(TIMMsgFileElemHandle handle, TIMGetMsgDataCB* cb);
	TIM_DECL void		GetFileFromByUUID(const char *uuid, const uint32_t business_id, TIMGetMsgDataCB* cb);
	
	typedef void* TIMVideoHandle;
	TIM_DECL int		GetUUIDFromTIMVideo(TIMVideoHandle handle, char* id, uint32_t* len);
	TIM_DECL int		GetVideoTypeFromTIMVideo(TIMVideoHandle handle, char* type, uint32_t* len);
	TIM_DECL void		SetVideoTypeFroTIMVideo(TIMVideoHandle handle, const char* type, uint32_t len);
	TIM_DECL int		GetVideoSizeFromTIMVideo(TIMVideoHandle handle);
	TIM_DECL int		GetVideoDurationFromTIMVideo(TIMVideoHandle handle);
	TIM_DECL void		SetVideoDurationFromTIMVideo(TIMVideoHandle handle, int duration);
	TIM_DECL void		GetVideoFromTIMVideo(TIMVideoHandle handle, const char* video_path, uint32_t path_len, TIMCommCB* cb);

	typedef void* TIMSnapshotHandle;
	TIM_DECL int		GetUUIDFromTIMSnapshot(TIMSnapshotHandle handle, char* id, uint32_t* len);
	TIM_DECL int		GetSnapshotTypeFromTIMSnapshot(TIMSnapshotHandle handle, char* type, uint32_t* len);
	TIM_DECL void		SetSnapshotTypeForTIMSnapshot(TIMSnapshotHandle handle, const char* type, uint32_t len);
	TIM_DECL int		GetSnapshotSizeForTIMSnapshot(TIMSnapshotHandle handle);
	TIM_DECL int		GetSnapshotWidthFromTIMSnapshot(TIMSnapshotHandle handle);
	TIM_DECL void		SetSnapshotWidthForTIMSnapshot(TIMSnapshotHandle handle, int width);
	TIM_DECL int		GetSnapshotHeightFromTIMSnapshot(TIMSnapshotHandle handle);
	TIM_DECL void		SetSnapshotHeightForTIMSnapshot(TIMSnapshotHandle handle, int height);
	TIM_DECL void		GetSnapshotFromTIMSnapshot(TIMVideoHandle handle, const char* snapshot_path, uint32_t path_len, TIMCommCB* cb);

	TIM_DECL uint32_t	GetVideoElemTask(TIMMsgVideoElemHandle handle);
	TIM_DECL void		SetVideoElemVideoFilePath(TIMMsgVideoElemHandle handle, const char* path, uint32_t file_path_len);
	TIM_DECL void		SetVideoElemSnapshotPath(TIMMsgVideoElemHandle handle, const char* path, uint32_t file_path_len);
	TIM_DECL TIMVideoHandle		GetVideoFromVideoElem(TIMMsgVideoElemHandle handle);
	TIM_DECL TIMSnapshotHandle	GetSnapshotFromVideoElem(TIMMsgVideoElemHandle handle);
	TIM_DECL int		GetVideoUploadingProcess(TIMMsgVideoElemHandle handle);
	
	

	typedef void* TIMGroupChangeInfoHandle;

#define TIM_GROUP_INFO_CHAGE_TYPE_GROUP_NAME	0x1 //Ⱥ������
#define TIM_GROUP_INFO_CHAGE_TYPE_INTRODUCTION	0x2 //Ⱥ������
#define	TIM_GROUP_INFO_CHAGE_TYPE_NOTIFACTION	0x3 //Ⱥ֪ͨ����
#define TIM_GROUP_INFO_CHAGE_TYPE_FACE_URL		0x4 //Ⱥͷ�����
#define TIM_GROUP_INFO_CHAGE_TYPE_OWNER			0x5 //Ⱥ������

	TIM_DECL int	  GetGroupChangeInfoType(TIMGroupChangeInfoHandle handle);
	TIM_DECL uint32_t GetGroupChangeInfoLen(TIMGroupChangeInfoHandle handle);
	TIM_DECL uint32_t GetGroupChangeInfo(TIMGroupChangeInfoHandle handle, char* info, uint32_t *len);

	typedef void* TIMGroupMemberInfoChangeHanlde;
	TIM_DECL int GetGroupMemberChangeInfoID(TIMGroupMemberInfoChangeHanlde handle, char* id, uint32_t * len);
	TIM_DECL uint32_t TIMGetGroupMemberChangeInfoShutTime(TIMGroupMemberInfoChangeHanlde handle);

	typedef void* TIMGroupTipsUserInfoHandle;
	TIM_DECL int GetGroupTipsUserInfoID(TIMGroupTipsUserInfoHandle handle, char* id, uint32_t * len);

	typedef enum _E_TIM_GROUPTIPS_TYPE
	{
		TIM_GROUP_TIPS_TYPE_INVITE              = 0x01, //�������Ⱥ
		TIM_GROUP_TIPS_TYPE_QUIT_GRP            = 0x02, //�˳�Ⱥ
		TIM_GROUP_TIPS_TYPE_KICKED              = 0x03, //�߳�Ⱥ 
		TIM_GROUP_TIPS_TYPE_SET_ADMIN           = 0x04, //���ù���Ա
		TIM_GROUP_TIPS_TYPE_CANCEL_ADMIN        = 0x05, //ȡ������Ա
		TIM_GROUP_TIPS_TYPE_INFO_CHANGE         = 0x06, //Ⱥ���ϱ��
		TIM_GROUP_TIPS_TYPE_MEMBER_INFO_CHANGE  = 0x07, //Ⱥ��Ա���ϱ�� 
	}E_TIM_GROUPTIPS_TYPE;

	typedef void(*CBGroupTipsEventHandleProc) (TIMMsgGroupTipsElemHandle group_tips_elems, void* data);

	typedef struct _TIMCallBack_GroupUpdateCB
	{
		CBGroupTipsEventHandleProc OnGroupTipsEvent;
		void* data;
	}TIMGroupEventListener;

	TIM_DECL TIMMsgGroupTipsElemHandle CloneGroupTipsElem(TIMMsgGroupTipsElemHandle handle);
	TIM_DECL void DestroyGroupTipsElem(TIMMsgGroupTipsElemHandle handle);
	//ȺTips����
	TIM_DECL E_TIM_GROUPTIPS_TYPE	GetGroupTipsInfoType(TIMMsgGroupTipsElemHandle handle);
	//ȺID
	TIM_DECL int	GetGroupTipsInfoGroupID(TIMMsgGroupTipsElemHandle handle, char* id, uint32_t * len);
	//����·�ʱ��
	TIM_DECL uint32_t	GetGroupTipsInfoTime(TIMMsgGroupTipsElemHandle handle);
	//Ⱥ��
	TIM_DECL int	GetGroupTipsInfoGroupName(TIMMsgGroupTipsElemHandle handle, char* group_name, uint32_t * len);
	//������ID
	TIM_DECL int	GetGroupTipsInfoOperatorID(TIMMsgGroupTipsElemHandle handle, char* id, uint32_t * len);
	//���������б�
	TIM_DECL uint32_t	GetGroupTipsInfoUsersNum(TIMMsgGroupTipsElemHandle handle);
	TIM_DECL int	GetGroupTipsInfoUsers(TIMMsgGroupTipsElemHandle handle, TIMGroupTipsUserInfoHandle* handles, uint32_t* num);
	//Ⱥ��Ϣ����б�
	TIM_DECL uint32_t	GetGroupTipsInfoGroupChangeInfoNum(TIMMsgGroupTipsElemHandle handle);
	TIM_DECL int	GetGroupTipsInfoGroupChangeInfo(TIMMsgGroupTipsElemHandle handle, TIMGroupChangeInfoHandle* handles, uint32_t* num);
	//Ⱥ��Ա����б�
	TIM_DECL uint32_t	GetGroupTipsInfoMemberChangeInfoNum(TIMMsgGroupTipsElemHandle handle);
	TIM_DECL int	GetGroupTipsInfoMemberChangeInfo(TIMMsgGroupTipsElemHandle handle, TIMGroupMemberInfoChangeHanlde* handles, uint32_t* num);
	//������profile
	TIM_DECL int	GetProfile4GroupTipsInfo(TIMMsgGroupTipsElemHandle handle, TIMProfileHandle profile);
	//������group member info
	TIM_DECL int	GetGroupMemberInfo4GroupTips(TIMMsgGroupTipsElemHandle handle, TIMGroupMemberInfoHandle member_info);
	//��ȡ���profile
	TIM_DECL uint32_t	GetChangeUserProfileCount4GroupTips(TIMMsgGroupTipsElemHandle handle);
	TIM_DECL int	GetChangeUserProfile4GroupTipsByID(TIMMsgGroupTipsElemHandle handle, const char* id, TIMProfileHandle profile);
	TIM_DECL int GetChangeUserProfile4GroupTipsByIndex(TIMMsgGroupTipsElemHandle handle, uint32_t idx, char* id, uint32_t* len, TIMProfileHandle profile);

	TIM_DECL uint32_t GetChangeMemberInfoCount4GroupTips(TIMMsgGroupTipsElemHandle handle);
	TIM_DECL int	GetChangeMemberInfo4GroupTipsByID(TIMMsgGroupTipsElemHandle handle, const char* id, TIMGroupMemberInfoHandle profile);
	TIM_DECL int	GetChangeMemberInfo4GroupTipsByIndex(TIMMsgGroupTipsElemHandle handle, uint32_t idx, char* id, uint32_t* len, TIMGroupMemberInfoHandle profile);
	// ��ȡ���ƽ̨
	TIM_DECL int	GetPlatform4GroupTipsInfo(TIMMsgGroupTipsElemHandle handle, char* platform, uint32_t* len);
	
	//Ⱥϵͳ��Ϣ����
	typedef enum _E_TIM_GROUP_SYSTEM_TYPE
	{
		TIM_GROUP_SYSTEM_ADD_GROUP_REQUEST_TYPE		= 0x01, //�����Ⱥ����ֻ�й���Ա���յ���
		TIM_GROUP_SYSTEM_ADD_GROUP_ACCEPT_TYPE      = 0x02,//�����Ⱥ��ͬ�⣨ֻ���������ܹ��յ���
		TIM_GROUP_SYSTEM_ADD_GROUP_REFUSE_TYPE      = 0x03,//�����Ⱥ���ܾ���ֻ���������ܹ��յ���
		TIM_GROUP_SYSTEM_KICK_OFF_FROM_GROUP_TYPE	= 0x04,//������Ա�߳�Ⱥ��ֻ�б��ߵ����ܹ��յ���
		TIM_GROUP_SYSTEM_DELETE_GROUP_TYPE			= 0x05,//Ⱥ����ɢ��ȫԱ�ܹ��յ���
		TIM_GROUP_SYSTEM_CREATE_GROUP_TYPE			= 0x06,//����Ⱥ��Ϣ���������ܹ��յ���
		TIM_GROUP_SYSTEM_INVITED_TO_GROUP_TYPE		= 0x07,//�����Ⱥ�����������ܹ��յ���
		TIM_GROUP_SYSTEM_QUIT_GROUP_TYPE			= 0x08,//������Ⱥ��������Ⱥ���ܹ��յ���
		TIM_GROUP_SYSTEM_GRANT_ADMIN_TYPE			= 0x09,//���ù���Ա(�������߽���)
		TIM_GROUP_SYSTEM_CANCEL_ADMIN_TYPE			= 0x0a,//ȡ������Ա(��ȡ���߽���)
		TIM_GROUP_SYSTEM_REVOKE_GROUP_TYPE			= 0x0b,//Ⱥ�ѱ�����(ȫԱ����)
		TIM_GROUP_SYSTEM_INVITED_NEED_CONFIRM		= 0x0c,//�����Ⱥ(ֻ�б������߻���յ�)
		TIM_GROUP_SYSTEM_INVITED_CONFIRMED			= 0x0d,//�����Ⱥ��ͬ��(ֻ�з��������߻���յ�)
		TIM_GROUP_SYSTEM_INVITED_REJECTED			= 0x0e,//�����Ⱥ���ܾ�(ֻ�з��������߻���յ�)
		TIM_GROUP_SYSTEM_CUSTOM_INFO				= 0xff,//�û��Զ���֪ͨ(Ĭ��ȫԱ����)
	}E_TIM_GROUP_SYSTEM_TYPE;

	//��������
	TIM_DECL E_TIM_GROUP_SYSTEM_TYPE	GetGroupReportType(TIMMsgGroupReportElemHandle handle);
	//Ⱥ��Id
	TIM_DECL int	GetGroupReportID(TIMMsgGroupReportElemHandle handle, char* id, uint32_t* len);
	//Ⱥ����
	TIM_DECL int	GetGroupReportName(TIMMsgGroupReportElemHandle handle, char* group_name, uint32_t* len);
	//������
	TIM_DECL int	GetGroupReportOperatorID(TIMMsgGroupReportElemHandle handle, char* id, uint32_t* len);
	//��������
	TIM_DECL int	GetGroupReportRemarkInfoLen(TIMMsgGroupReportElemHandle handle);
	TIM_DECL int	GetGroupReportRemarkInfo(TIMMsgGroupReportElemHandle handle, char* remark_info, uint32_t* len);
	//�û��Զ���֪ͨ
	TIM_DECL uint32_t GetGroupReportUserDataLen(TIMMsgGroupReportElemHandle handle);
	TIM_DECL int	GetGroupReportUserData(TIMMsgGroupReportElemHandle handle, char* user_data, uint32_t* len);
	//������profile
	TIM_DECL int	GetProfile4GroupReport(TIMMsgGroupReportElemHandle handle, TIMProfileHandle profile);
	//������group member info
	TIM_DECL int	GetGroupMemberInfo4GroupReport(TIMMsgGroupReportElemHandle handle, TIMGroupMemberInfoHandle member_info);
	// ��ȡ���ƽ̨
	TIM_DECL int	GetPlatform4GroupReport(TIMMsgGroupReportElemHandle handle, char* platform, uint32_t* len);

	//������Ⱥ���� ������������Ⱥ����
	// 0x00:�ܾ���Ⱥ
	// 0x01:ͬ����Ⱥ
	TIM_DECL int HandleJoinRequest(TIMMsgGroupReportElemHandle handle, TIMGroupPendencyHandleResult result, 
									const char* approval_msg, uint32_t msg_len, 
									const char* user_data, uint32_t data_len, 
									TIMCommCB* cb);

	//��ϵ�������Ϣ����
	typedef enum _E_TIM_SNS_SYSTEM_TYPE
	{
		TIM_SNS_SYSTEM_ADD_FRIEND = 0x01,//��Ӻ���ϵͳ֪ͨ
		TIM_SNS_SYSTEM_DEL_FRIEND = 0x02,//ɾ������ϵͳ֪ͨ
		TIM_SNS_SYSTEM_ADD_FRIEND_REQ = 0x03,//��������ϵͳ֪ͨ
		TIM_SNS_SYSTEM_DEL_FRIEND_REQ = 0x04,//ɾ��δ������֪ͨ
		TIM_SNS_SYSTEM_ADD_BLACKLIST = 0x05,//���������
		TIM_SNS_SYSTEM_DEL_BLACKLIST = 0x06,//������ɾ��
		TIM_SNS_SYSTEM_PENDENCY_REPORT = 0x07, //δ���Ѷ��ϱ�
		TIM_SNS_SYSTEM_SNS_PROFILE_CHANGE = 0x08, //�������ݸ���
		TIM_SNS_SYSTEM_ADD_RECOMMEND = 0x09, //�Ƽ���������
		TIM_SNS_SYSTEM_DEL_RECOMMEND = 0x0a, //�Ƽ�����ɾ��
		TIM_SNS_SYSTEM_ADD_DECIDE = 0x0b, //�Ѿ�����
		TIM_SNS_SYSTEM_DEL_DECIDE = 0x0c, //�Ѿ�ɾ��
		TIM_SNS_SYSTEM_RECOMMEND_REPORT = 0x0d, //�Ƽ��Ѷ��ϱ�
		TIM_SNS_SYSTEM_DECIDE_REPORT = 0x0e, //�Ѿ��Ѷ��ϱ�
	}E_TIM_SNS_SYSTEM_TYPE;

	typedef void* TIMMsgSNSChangeInfoHandle;
	TIM_DECL int GetID4SNSChangeInfoHandle(TIMMsgSNSChangeInfoHandle handle, char* id, uint32_t* len);
	TIM_DECL int GetNick4SNSChangeInfoHandle(TIMMsgSNSChangeInfoHandle handle, char* nick, uint32_t* len);
	TIM_DECL int GetAddWord4SNSChangeInfoHandle(TIMMsgSNSChangeInfoHandle handle, char* word, uint32_t* len);
	TIM_DECL int GetAddSource4SNSChangeInfoHandle(TIMMsgSNSChangeInfoHandle handle, char* source, uint32_t* len);
	TIM_DECL int GetRemark4SNSChangeInfoHandle(TIMMsgSNSChangeInfoHandle handle, char* remark, uint32_t* len);

	TIM_DECL E_TIM_SNS_SYSTEM_TYPE GetType4SNSSystemElem(TIMMsgSNSSystemElemHandle handle);
	TIM_DECL uint32_t GetChangeInfoNum4SNSSystemElem(TIMMsgSNSSystemElemHandle handle);
	TIM_DECL int GetChangeInfo4SNSSystemElem(TIMMsgSNSSystemElemHandle elem_handle, TIMMsgSNSChangeInfoHandle* info_handle, uint32_t* num);
	//��TIM_SNS_SYSTEM_PENDENCY_REPORT
	TIM_DECL uint64_t GetPendencyReportTimestamp4SNSSystemElem(TIMMsgSNSSystemElemHandle elem_handle);
	//��TIM_SNS_SYSTEM_RECOMMEND_REPORT
	TIM_DECL uint64_t GetRecommendReportTimestamp4SNSSystemElem(TIMMsgSNSSystemElemHandle elem_handle);
	//��TIM_SNS_SYSTEM_DECIDE_REPORT ��Ч
	TIM_DECL uint64_t GetDecideReportTimestamp4SNSSystemElem(TIMMsgSNSSystemElemHandle elem_handle);
	
	typedef enum _E_TIM_PROFILE_SYSTEM_TYPE
	{
		TIM_PROFILE_SYSTEM_FRIEND_PROFILE_CHANGE = 0x01,//�������ϱ��
	}E_TIM_PROFILE_SYSTEM_TYPE;

	TIM_DECL E_TIM_PROFILE_SYSTEM_TYPE GetType4ProfileProfileSystemElemHandle(TIMMsgProfileSystemElemHandle handle);
	TIM_DECL int GetID4ProfileProfileSystemElemHandle(TIMMsgProfileSystemElemHandle handle, char* id, uint32_t* len);
	TIM_DECL int GetNick4ProfileProfileSystemElemHandle(TIMMsgProfileSystemElemHandle handle, char* nick, uint32_t* len);
};
#endif



	