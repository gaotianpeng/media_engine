#ifndef _TIM_FRIEND_C_H_
#define _TIM_FRIEND_C_H_

#include "tim_comm_c.h"
extern "C"
{
	typedef enum _E_TIMFriendAllowType
	{
		TIM_FRIEND_ALLOW_ANY                    = 0,//ͬ���κ��û��Ӻ���
		TIM_FRIEND_DENY_ANY                     = 1,//�ܾ��κ��˼Ӻ���
		TIM_FRIEND_NEED_CONFIRM                 = 2,//��Ҫ��֤
	}E_TIMFriendAllowType;

	typedef enum _E_TIMFriendGenderType
	{
		TIM_GENDER_UNKOWN	= 0,
		TIM_GENDER_MALE		= 1,
		TIM_GENDER_FEMALE	= 2,
	}E_TIMFriendGenderType;

	typedef enum _E_TIMFriendResponseType
	{
	    TIM_FRIEND_RESPONSE_AGREE				= 0,//ͬ��Ӻ��ѣ�����������ѣ�
		TIM_FRIEND_RESPONSE_AGREE_AND_ADD		= 1,//ͬ��Ӻ��Ѳ��ӶԷ�Ϊ���ѣ�����˫����ѣ�
		TIM_FRIEND_RESPONSE_REJECT				= 2,//�ܾ��Է���������
	}E_TIMFriendResponseType;

	typedef enum _E_TIMProfileFlag
	{
		TIM_PROFILE_FLAG_NONE			= 0x00,
		TIM_PROFILE_FLAG_NICK			= 0x01, //�ǳ�
		TIM_PROFILE_FLAG_ALLOW_TYPE		= 0x01 << 1, //������֤��ʽ
		TIM_PROFILE_FLAG_FACE_URL       = 0x01 << 2, //ͷ��
		TIM_PROFILE_FLAG_REMARK         = 0x01 << 3, //���ѱ�ע
		TIM_PROFILE_FLAG_GROUP			= 0x01 << 4, //���ѷ���
		TIM_PROFILE_FLAG_SELF_SIGNATURE = 0X01 << 5, //����ǩ��
		TIM_PROFILE_FLAG_GENDER			= 0x01 << 6,
		TIM_PROFILE_FLAG_BIRTHDAY		= 0x01 << 7,
		TIM_PROFILE_FLAG_LOCATION		= 0x01 << 8,
		TIM_PROFILE_FLAG_LANGUAGE		= 0x01 << 9,
	}TIMProfileFlag;

	typedef enum _E_TIMPendencyGetType
	{
		TIM_PENDENCY_GET_COME_IN                    = 1, //���˷����ҵ�
		TIM_PENDENCY_GET_SEND_OUT                   = 2, //�ҷ������˵�
		TIM_PENDENCY_GET_BOTH                       = 3, //���˷����ҵ� �� �ҷ������˵�
	}TIMPendencyGetType;

	typedef enum  
	{
		TIM_PAGE_DIRECTION_UP_TYPE		= 1, //���Ϸ�ҳ
		TIM_PAGE_DIRECTION_DOWN_TYPE	= 2, //���·�ҳ
	}TIMPageDirectionType;

	typedef enum  
	{
		TIM_FutureTypePendencyComeIn = 0x01, // �յ���δ������
		TIM_FutureTypePendencySendOut = 0x02, //����ȥ��δ������
		TIM_FutureTypeRecommend = 0x04, //�Ƽ�����
		TIM_FutureTypeDeccide = 0x08, //�Ѿ�����
	}TIMFutureFriendType;

	typedef enum
	{
		TIM_FRIEND_CHECK_TYPE_UNIDIRECTION = 0x1, //�������
		TIM_FRIEND_CHECK_TYPE_BIDIRECTION = 0x2, //��Ϊ����
	}TIMFriendCheckType;

	
	//���ѹ�ϵ����
	typedef enum
	{
		TIM_FRIEND_RELATION_TYPE_NONE	= 0x0, //���Ǻ���
		TIM_FRIEND_RELATION_TYPE_MY_UNI = 0x1, //�Է����ҵĺ����б���
		TIM_FRIEND_RELATION_TYPE_OTHER_UNI = 0x2, //���ڶԷ��ĺ����б���
		TIM_FRIEND_RELATION_TYPE_BOTH	= 0x3, //��Ϊ����
	}TIMFriendRelationType;

#define TIM_FRIEND_STATUS_SUCC								0//�����ɹ�
#define TIM_ADD_FRIEND_STATUS_IN_SELF_BLACK_LIST			30515//�Ӻ���ʱ��Ч�����Ӻ������Լ��ĺ�������
#define TIM_ADD_FRIEND_STATUS_FRIEND_SIDE_FORBID_ADD		30516//�Ӻ���ʱ��Ч�����Ӻ�������Ϊ��ֹ�Ӻ���
#define TIM_ADD_FRIEND_STATUS_SELF_FRIEND_FULL				30519//�Ӻ���ʱ��Ч��������������
#define TIM_ADD_FRIEND_STATUS_ALREADY_FRIEND				30520//�Ӻ���ʱ��Ч���Ѿ��Ǻ���
#define TIM_ADD_FRIEND_STATUS_IN_OTHER_SIDE_BLACK_LIST		30525//�Ӻ���ʱ��Ч���ѱ�����Ӻ�������Ϊ������
#define TIM_ADD_FRIEND_STATUS_PENDING						30539//�Ӻ���ʱ��Ч���ȴ��������ͬ��
#define TIM_DEL_FRIEND_STATUS_NO_FRIEND						31704//ɾ������ʱ��Ч��ɾ������ʱ�Է����Ǻ���
#define TIM_RESPONSE_FRIEND_STATUS_NO_REQ                   30614//��Ӧ��������ʱ��Ч���Է�û�����������
#define TIM_RESPONSE_FRIEND_STATUS_SELF_FRIEND_FULL			30615//��Ӧ��������ʱ��Ч���Լ��ĺ�����
#define TIM_RESPONSE_FRIEND_STATUS_FRIEND_EXIST				30617//��Ӧ��������ʱ��Ч�������Ѿ�����
#define TIM_RESPONSE_FRIEND_STATUS_OTHER_SIDE_FRIEND_FULL   30630//��Ӧ��������ʱ��Ч���Է�������
#define TIM_ADD_BLACKLIST_FRIEND_STATUS_IN_BLACK_LIST		31307//��Ӻ�������Ч���Ѿ��ں�������
#define TIM_DEL_BLACKLIST_FRIEND_STATUS_NOT_IN_BLACK_LIST	31503//ɾ����������Ч���û����ں�������
#define TIM_ADD_FRIEND_GROUP_STATUS_GET_SDKAPPID_FAILED		32207//�������ѷ���ʱ��Ч��û������SDKAppId
#define TIM_ADD_FRIEND_GROUP_STATUS_NOT_FRIEND				32216//�������ѷ���ʱ��Ч��Ҫ���뵽���ѷ����е��û����Ǻ���
#define TIM_UPDATE_FRIEND_GROUP_STATUS_GET_SDKAPPID_FAILED	32511//���º��ѷ���ʱ��Ч��û������SDKAppId
#define TIM_UPDATE_FRIEND_GROUP_STATUS_ADD_NOT_FRIEND		32518//���º��ѷ���ʱ��Ч��Ҫ���뵽���ѷ����е��û����Ǻ���
#define	TIM_UPDATE_FRIEND_GROUP_STATUS_ADD_ALREADY_IN_GROUP	32519//���º��ѷ���ʱ��Ч��Ҫ���뵽���ѷ����еĺ����Ѿ��ڷ�����
#define TIM_UPDATE_FRIEND_GROUP_STATUS_DEL_NOT_IN_GROUP		32520//���º��ѷ���ʱ��Ч��Ҫ�Ӻ��ѷ�����ɾ���ĺ��Ѳ��ں��ѷ�����
#define TIM_UPDATE_FRIEND_GROUP_STATUS_MAX_GROUPS_EXCEED	32521//���º��ѷ���ʱ��Ч���ú��Ѽ���ĺ��ѷ���������������ƣ�ÿ���������ֻ�ܼ���32�����ѷ���

	typedef void* TIMProfileCustomInfoHandle;
	TIM_DECL TIMProfileCustomInfoHandle CreateProfileCustomInfoHandle();
	TIM_DECL void DestroyProfileCustomInfoHandle(TIMProfileCustomInfoHandle handle);
	TIM_DECL TIMProfileCustomInfoHandle CloneProfileCustomInfoHandle(TIMProfileCustomInfoHandle handle);
	//value ��������256�ֽ� key��������8�ֽ�
	TIM_DECL bool AddProfileCustomInfo(TIMProfileCustomInfoHandle handle, const char* key, uint32_t key_len, const char* value, uint32_t value_len);
	TIM_DECL uint32_t GetProfileCustomInfoNum(TIMProfileCustomInfoHandle handle);
	TIM_DECL int GetProfileCustomInfo(TIMProfileCustomInfoHandle handle, char* key, uint32_t key_len, char* value, uint32_t* value_len);
	TIM_DECL int GetProfileCustomInfoByIndex(TIMProfileCustomInfoHandle handle, int index, char* key, uint32_t* key_len, char* value, uint32_t* value_len);

	typedef void* TIMFriendResultHandle;
	TIM_DECL int GetID4FriendResultHandle(TIMFriendResultHandle handle, char* id, uint32_t* len);
	TIM_DECL uint64_t GetResult4FriendResultHandle(TIMFriendResultHandle handle);

	//�û�����
	TIM_DECL TIMProfileHandle CreateProfileHandle();
	TIM_DECL void DestroyProfileHandle(TIMProfileHandle handle);
	TIM_DECL TIMProfileHandle CloneProfileHandle(TIMProfileHandle handle);
	//�û�identifier
	TIM_DECL void	SetID4ProfileHandle(TIMProfileHandle handle, char* id);
	TIM_DECL int	GetID4ProfileHandle(TIMProfileHandle handle, char* id_buf, uint32_t* len);
	//�û��ǳ� ���96�ֽ�
	TIM_DECL void SetNickName4ProfileHandle(TIMProfileHandle handle, char* nick_name, uint32_t len);
	TIM_DECL int	GetNickName4ProfileHandle(TIMProfileHandle handle, char* buf, uint32_t* len);
	//������֤��ʽ
	TIM_DECL void	SetAllowType4ProfileHandle(TIMProfileHandle handle, E_TIMFriendAllowType type);
	TIM_DECL E_TIMFriendAllowType	GetAllowType4ProfileHandle(TIMProfileHandle handle);
	//�û���ע ���120�ֽ�
	TIM_DECL void	SetRemark4ProfileHandle(TIMProfileHandle handle, char* remark, uint32_t len);
	TIM_DECL int	GetRemark4ProfileHandle(TIMProfileHandle handle, char* remark, uint32_t* len);
	//�û��Ա�
	TIM_DECL E_TIMFriendGenderType	GetGenderType4ProfileHandle(TIMProfileHandle handle);
	//�û�location
	TIM_DECL int	GetLocation4ProfileHandle(TIMProfileHandle handle, char* location, uint32_t* len);
	//�û�language
	TIM_DECL uint32_t GetLanguage4ProfileHandle(TIMProfileHandle handle);
	//�û�Birthday
	TIM_DECL uint32_t GetBirthday4ProfileHandle(TIMProfileHandle handle);
	//�û���ɫ
	TIM_DECL uint32_t GetRole4ProfileHandle(TIMProfileHandle handle);
	//�û�����
	TIM_DECL uint32_t GetLevel4ProfileHandle(TIMProfileHandle handle);
	//���ѷ���
	TIM_DECL uint32_t GetGroupCnt4ProfileHandle(TIMProfileHandle handle);
	TIM_DECL int	GetGroupName4ProfileHandle(TIMProfileHandle handle, uint32_t idx, char* group_name, uint32_t* len);
	//�Ӻ���wording ���120�ֽ�
	TIM_DECL void	SetAddWord4ProfileHandle(TIMProfileHandle handle, char* word, uint32_t len);
	TIM_DECL int	GetAddWord4ProfileHandle(TIMProfileHandle handle, char* word, uint32_t* len);
	TIM_DECL void	SetAddSource4ProfileHandle(TIMProfileHandle handle, char* source);
	TIM_DECL int	GetAddSource4ProfileHandle(TIMProfileHandle handle, char* source, uint32_t* len);
	TIM_DECL void	SetFaceURL4ProfileHandle(TIMProfileHandle handle, char* faceurl, uint32_t len);
	TIM_DECL int	GetFaceURL4ProfileHandle(TIMProfileHandle handle, char* faceurl, uint32_t* len);

	TIM_DECL int GetResponse4ProfileHandle(TIMProfileHandle handle, char* response, uint32_t* len);
	TIM_DECL uint64_t GetResult4ProfileHandle(TIMProfileHandle handle);


	typedef void* TIMSelfProfileHandle;
	TIM_DECL TIMSelfProfileHandle CloneSelfProfileHandle(TIMSelfProfileHandle handle);
	TIM_DECL void DestroySelfProfileHandle(TIMSelfProfileHandle handle);
	TIM_DECL int GetNickName4SlefProfileHandle(TIMSelfProfileHandle handle, char* buf, uint32_t* len);
	TIM_DECL E_TIMFriendAllowType GetAllowType4SlefProfileHandle(TIMSelfProfileHandle handle);

	typedef void* TIMFriendProfileHandle;
	TIM_DECL TIMFriendProfileHandle CloneFriendProfileHandle(TIMFriendProfileHandle handle);
	TIM_DECL void DestroyFriendProfileHandle(TIMFriendProfileHandle handle);
	TIM_DECL int GetID4FriendProfileHandle(TIMFriendProfileHandle handle, char* id, uint32_t* len);
	TIM_DECL int GetNickName4FriendProfileHandle(TIMFriendProfileHandle handle, char* buf, uint32_t* len);
	TIM_DECL int GetRemark4FriendProfileHandle(TIMFriendProfileHandle handle, char* remark, uint32_t* len);
	TIM_DECL E_TIMFriendAllowType GetAllowType4FriendProfileHandle(TIMFriendProfileHandle handle);
	TIM_DECL int GetFaceURL4FriendProfileHandle(TIMFriendProfileHandle handle, char* faceurl, uint32_t* len);
	TIM_DECL int GetSelfSignature4FriendProfileHandle(TIMFriendProfileHandle handle, char* self_signature, uint32_t* len);
	TIM_DECL TIMProfileCustomInfoHandle GetCustomInfo4FriendProfileHandle(TIMFriendProfileHandle handle);

	typedef void* TIMFriendListElemHandle;
	TIM_DECL TIMFriendListElemHandle CloneFriendListElemHandle(TIMFriendListElemHandle handle);
	TIM_DECL void DestroyFriendListElemHandle(TIMFriendListElemHandle handle);
	TIM_DECL int GetID4FriendListElemHandle(TIMFriendListElemHandle handle, char* id, uint32_t* len);
	TIM_DECL int GetNickName4FriendListElemHandle(TIMFriendListElemHandle handle, char* buf, uint32_t* len);
	TIM_DECL int GetRemark4FriendListElemHandle(TIMFriendListElemHandle handle, char* remark, uint32_t* len);
	TIM_DECL int GetFaceURL4FriendListElemHandle(TIMFriendListElemHandle handle, char* face_url, uint32_t* len);

	typedef void* TIMAddFriendHandle;
	TIM_DECL TIMAddFriendHandle CreateAddFriendHandle();
	TIM_DECL void DestroyAddFriendHandle(TIMAddFriendHandle handle);
	TIM_DECL void SetID4AddFriendHandle(TIMAddFriendHandle handle, char* id);
	//�û��ǳ� ���96�ֽ�
	TIM_DECL void SetNickName4AddFriendHandle(TIMAddFriendHandle handle, char* nick_name, uint32_t len);
	//���ѱ�ע ���120�ֽ�
	TIM_DECL void SetRemark4AddFriendHandle(TIMAddFriendHandle handle, char* remark, uint32_t len);
	//������Դ
	TIM_DECL void SetAddSource4AddFriendHandle(TIMAddFriendHandle handle, char* source);
	//�Ӻ���wording
	TIM_DECL void SetAddWord4AddFriendHandle(TIMAddFriendHandle handle, char* word, uint32_t len);

	typedef void* TIMFriendResponseHandle;
	TIM_DECL TIMFriendResponseHandle CreateFriendResponseHandle();
	TIM_DECL void DestroyFriendResponseHandle(TIMFriendResponseHandle handle);
	//�û�ID
	TIM_DECL void SetID4FriendResponseHandle(TIMFriendResponseHandle handle, char* id);
	//��Ӧ����
	TIM_DECL void SetResponseType4FriendResponseHandle(TIMFriendResponseHandle handle, E_TIMFriendResponseType type);

	typedef void* TIMSetProfileOptionHandle;
	TIM_DECL TIMSetProfileOptionHandle CreateSetProfileOptionHandle();
	TIM_DECL void DestroySetProfileOptionHandle(TIMSetProfileOptionHandle handle);
	TIM_DECL void SetFlag4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, TIMProfileFlag flag);
	TIM_DECL void SetNick4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, const char* nick, uint32_t len);
	TIM_DECL void SetAllowType4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, E_TIMFriendAllowType type);
	TIM_DECL void SetFaceURL4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, const char* faceurl, uint32_t len);
	TIM_DECL void SetSelfSignature4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, const char* self_signature, uint32_t len);
	TIM_DECL void SetGender4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, E_TIMFriendGenderType gender);
	TIM_DECL void SetBirthday4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, uint32_t birthday);
	TIM_DECL void SetRole4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, uint32_t role);
	TIM_DECL void SetLevel4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, uint32_t level);
	TIM_DECL void SetLocation4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, const char* location, uint32_t len);
	TIM_DECL void SetLanguage4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, uint32_t language);
	TIM_DECL void SetCustomInfo4SetProfileOptionHandle(TIMSetProfileOptionHandle handle, TIMProfileCustomInfoHandle custom_info);

	typedef void* TIMGetProfileOptionHandle;
	TIM_DECL TIMGetProfileOptionHandle CreateGetProfileOptionHandle();
	TIM_DECL void DestroyGetProfileOptionHandle(TIMGetProfileOptionHandle handle);
	TIM_DECL void SetFlag4GetProfileOptionHandle(TIMGetProfileOptionHandle handle, TIMProfileFlag type);
	TIM_DECL void SetCustomInfo4GetProfileOptionHandle(TIMGetProfileOptionHandle handle, TIMProfileCustomInfoHandle custom_info);

	typedef void* TIMCheckFriendResultHandle;
	TIM_DECL int TIMGetID4CheckFriendResultHandle(TIMCheckFriendResultHandle handle, char* id, uint32_t* len);
	TIM_DECL uint64_t TIMGetStatus4CheckFriendResultHandle(TIMCheckFriendResultHandle handle);
	TIM_DECL TIMFriendRelationType TIMGetRelationType4CheckFriendResultHandle(TIMCheckFriendResultHandle handle);

	typedef void* TIMFriendMetaInfoHandle;
	TIM_DECL TIMFriendMetaInfoHandle CreateFriendMetaInfoHandle();
	TIM_DECL void DestroyFriendMetaInfoHandle(TIMFriendMetaInfoHandle handle);
	TIM_DECL TIMFriendMetaInfoHandle CloneFriendMetaInfoHandle(TIMFriendMetaInfoHandle handle);
	//ʱ�������Ҫ���棬�´���ȡʱ���룬��������ʹ��
	TIM_DECL void SetTimestamp4FriendMetaInfoHandle(TIMFriendMetaInfoHandle handle, const uint64_t timestamp);
	TIM_DECL uint64_t GetTimestamp4FriendMetaInfoHandle(TIMFriendMetaInfoHandle handle);
	//���кţ���Ҫ���棬�´���ȡʱ���룬��������ʹ��
	TIM_DECL void SetInfoSeq4FriendMetaInfoHandle(TIMFriendMetaInfoHandle handle, const uint64_t info_seq);
	TIM_DECL uint64_t GetInfoSeq4FriendMetaInfoHandle(TIMFriendMetaInfoHandle handle);
	//��ҳ��Ϣ�����豣�棬����Ϊ0ʱ��������0ʱ�����ٴ���ȡ����һ����ȡʱ��0
	TIM_DECL void SetNextSeq4FriendMetaInfoHandle(TIMFriendMetaInfoHandle handle, const uint64_t next_seq);
	TIM_DECL uint64_t GetNextSeq4FriendMetaInfoHandle(TIMFriendMetaInfoHandle handle);
	//���ǣ�ΪTRUEʱ��Ҫ����timestamp, infoSeq, nextSeqΪ0������ͻ��˴洢��������ȡ����
	TIM_DECL void SetRecover4FriendMetaInfoHandle(TIMFriendMetaInfoHandle handle, const bool recover);
	TIM_DECL bool GetRecover4FriendMetaInfoHandle(TIMFriendMetaInfoHandle handle);
	
	//δ������Ԫ��Ϣ
	typedef void* TIMFriendPendencyMetaHandle;
	TIM_DECL TIMFriendPendencyMetaHandle CreateFriendPendencyMetaHandle();
	TIM_DECL void DestroyFriendPendencyMetaHandle(TIMFriendPendencyMetaHandle handle);
	/**
	*���кţ�δ���б����к�
	*����ͻ��˱���seq��δ���б�����ʱ����server���ص�seq
	*���seq��server���µģ��򲻷�������
	*/
	TIM_DECL void SetSeq4FriendPendencyMetaHandle(TIMFriendPendencyMetaHandle handle, const uint64_t seq);
	TIM_DECL uint64_t GetSeq4FriendPendencyMetaHandle(TIMFriendPendencyMetaHandle handle);
	/**
	* ��ҳʱ�����ֻ������ҳ��server����0ʱ��ʾû�и������ݣ���һ��������0
	*    �ر�ע����ǣ����server���ص�seq�������seq��ͬ����ҳ�����У���Ҫʹ�ÿͻ���ԭʼseq����ֱ������������ϣ����ܸ��±���seq
	*/
	TIM_DECL void SetTimestamp4FriendPendencyMetaHandle(TIMFriendPendencyMetaHandle handle, const uint64_t timestamp);
	TIM_DECL uint64_t GetTimestamp4FriendPendencyMetaHandle(TIMFriendPendencyMetaHandle handle);

	/**
	* ÿҳ������������ʱ��Ч������ֵ��server�ɸ�����Ҫ���ػ����٣�������Ϊ������ı�־��
	*/
	TIM_DECL void SetNumPerPage4FriendPendencyMetaHandle(TIMFriendPendencyMetaHandle handle, const uint64_t num_per_page);
	TIM_DECL uint64_t GetNumPerPage4FriendPendencyMetaHandle(TIMFriendPendencyMetaHandle handle);

	/**
	* δ������δ��������������Ч��
	*/
	TIM_DECL uint64_t GetUnreadCnt4FriendPendencyMetaHandle(TIMFriendPendencyMetaHandle handle);
	

	//δ������
	typedef void* TIMFriendPendencyItemHandle;
	TIM_DECL TIMFriendPendencyItemHandle CloneFriendPendencyItemHandle(TIMFriendPendencyItemHandle handle);
	TIM_DECL void DestroyFriendPendencyItemHandle(TIMFriendPendencyItemHandle handle);
	TIM_DECL int GetID4FriendPendencyItemHandle(TIMFriendPendencyItemHandle handle, char* id, uint32_t* id_len);
	//��ȡ���ʱ��
	TIM_DECL uint64_t GetAddTime4FriendPendencyItemHandle(TIMFriendPendencyItemHandle handle);
	//��ȡ��Դ
	TIM_DECL int GetAddSource4FriendPendencyItemHandle(TIMFriendPendencyItemHandle handle, char* add_source, uint32_t* len);
	//��ȡ�Ӻ��Ѹ���
	TIM_DECL int GetAddWord4FriendPendencyItemHandle(TIMFriendPendencyItemHandle handle, char* add_word, uint32_t* len);
	//��ȡ�ǳ�
	TIM_DECL int GetNick4FriendPendencyItemHandle(TIMFriendPendencyItemHandle handle, char* nick, uint32_t* len);
	//��ȡδ����������
	TIM_DECL TIMPendencyGetType GetPendencyTypeFriendPendencyItemHandle(TIMFriendPendencyItemHandle handle);

	typedef void* TIMFriendFutureMetaHandle;
	TIM_DECL TIMFriendFutureMetaHandle CreateFriendFutureMetaHandle();
	TIM_DECL void DestroyFriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	TIM_DECL TIMFriendFutureMetaHandle CloneFriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	// req info
	TIM_DECL void SetDirection4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle, TIMPageDirectionType type);
	TIM_DECL void SetReqNum4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle, uint64_t reqnum);

	// for req and resp
	TIM_DECL void SetTimeStamp4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle, uint64_t timestamp);
	TIM_DECL uint64_t GetTimeStamp4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	TIM_DECL void SetPendencySequence4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle, uint64_t seq);
	TIM_DECL uint64_t GetPendencySequence4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	TIM_DECL void SetRecommendSequence4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle, uint64_t seq);
	TIM_DECL uint64_t GetRecommendSequence4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	TIM_DECL void SetDecideSequence4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle, uint64_t seq);
	TIM_DECL uint64_t GetDecideSequence4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	TIM_DECL void SetPendencyUnReadCnt4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle, uint64_t cnt);
	TIM_DECL uint64_t GetPendencyUnReadCnt4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	TIM_DECL void SetRecommendUnReadCnt4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle, uint64_t cnt);
	TIM_DECL uint64_t GetRecommendUnReadCnt4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	// just for response
	TIM_DECL uint64_t GetCurrentPendencyTimestamp4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	TIM_DECL uint64_t GetCurrentRecommendTimestamp4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);
	TIM_DECL uint64_t GetCurrentDecideTimestamp4FriendFutureMetaHandle(TIMFriendFutureMetaHandle handle);

	typedef void* TIMFriendRecommendTagsHandle;
	TIM_DECL uint32_t GetTagsNum4FriendRecommendTagsHandle(TIMFriendRecommendTagsHandle handle);
	TIM_DECL int GetTag4FriendRecommendTagsHandle(TIMFriendRecommendTagsHandle handle, char* key, uint32_t key_len, char* value, uint32_t* value_len);
	TIM_DECL int GetTagByIndex4FriendRecommendTagsHandle(TIMFriendRecommendTagsHandle handle, int index, char* key, uint32_t* key_len, char* value, uint32_t* value_len);

	typedef void* TIMFriendFutureItemHandle;
	TIM_DECL TIMFriendFutureItemHandle CloneFriendFutureItemHandle(TIMFriendFutureItemHandle handle);
	TIM_DECL void DestroyFriendFutureItemHandle(TIMFriendFutureItemHandle handle);
	TIM_DECL TIMFutureFriendType GetType4FriendFutureItemHandle(TIMFriendFutureItemHandle handle);
	//����id
	TIM_DECL int GetID4FriendFutureItemHandle(TIMFriendFutureItemHandle handle, char* id, uint32_t* len);
	//�������� //handle������destroy
	TIM_DECL TIMProfileHandle GetProfileHandle4FriendFutureItemHandle(TIMFriendFutureItemHandle handle);
	//���ʱ��
	TIM_DECL uint64_t GetAddTime4FriendFutureItemHandle(TIMFriendFutureItemHandle handle);
	//��Դ����δ��������Ч��
	TIM_DECL int GetAddSource4FriendFutureItemHandle(TIMFriendFutureItemHandle handle, char* source, uint32_t* len);
	//�Ӻ��Ѹ��ԣ���δ��������Ч��
	TIM_DECL int GetAddWording4FriendFutureItemHandle(TIMFriendFutureItemHandle handle, char* wording, uint32_t* len);
	TIM_DECL TIMFriendRecommendTagsHandle GetRecommendTagsHandle44FriendFutureItemHandle(TIMFriendFutureItemHandle handle);

	typedef void* TIMFriendGroupNamesHandle;
	TIM_DECL TIMFriendGroupNamesHandle CreateFriendGroupNamesHandle();
	TIM_DECL void DestroyFriendGroupNamesHandle(TIMFriendGroupNamesHandle handle);
	TIM_DECL void AddGroupNameforFriendGroupNamesHandle(TIMFriendGroupNamesHandle handle, const char* name, uint32_t len);

	typedef void* TIMFriendGroupInfoHandle;
	TIM_DECL int GetName4FriendGroupInfoHandle(TIMFriendGroupInfoHandle handle, char* group_name, uint32_t* len);
	TIM_DECL uint64_t GetFriendsCnt4FriendGroupInfoHandle(TIMFriendGroupInfoHandle handle);
	TIM_DECL int GetFriendID4FriendGroupInfoHandle(TIMFriendGroupInfoHandle handle, uint64_t idx, char* id, uint32_t* len);

	typedef void* TIMFriendCheckInfoHandle;
	TIMFriendCheckInfoHandle CreateFriendCheckInfoHandle();
	void DestroyFriendCheckInfoHandle(TIMFriendCheckInfoHandle check_info);
	TIM_DECL void SetCheckID4FriendCheckInfoHandle(TIMFriendCheckInfoHandle check_info, const char** id, uint32_t id_num);
	TIM_DECL void SetCheckType4FriendCheckInfoHandle(TIMFriendCheckInfoHandle check_info, TIMFriendCheckType check_type);

	// callback
	typedef void (*CBFriendshipActionCallbackOnSuccess) (TIMProfileHandle* handles, uint32_t num, void* data);
	typedef void (*CBFriendshipActionCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMFriendshipActionCallback
	{
		CBFriendshipActionCallbackOnSuccess OnSuccess;
		CBFriendshipActionCallbackOnError OnError;
		void* data;
	}TIMFriendshipActionCB;

	typedef void (*CBGetSelfProfileCallbackOnSuccess) (TIMSelfProfileHandle* handles, uint32_t num, void* data);
	typedef void (*CBGetSelfProfileCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMGetSelfProfileCallback
	{
		CBGetSelfProfileCallbackOnSuccess OnSuccess;
		CBGetSelfProfileCallbackOnError OnError;
		void* data;
	}TIMGetSelfProfileCallback;

	typedef void (*CBGetFriendProfileCallbackOnSuccess) (TIMFriendProfileHandle* handles, uint32_t num, void* data);
	typedef void (*CBGetFriendProfileCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMGetFriendProfileCallback
	{
		CBGetFriendProfileCallbackOnSuccess OnSuccess;
		CBGetFriendProfileCallbackOnError OnError;
		void* data;
	}TIMGetFriendProfileCallback;

	typedef void (*CBGetFriendListCallbackOnSuccess) (TIMFriendListElemHandle* handles, uint32_t num, void* data);
	typedef void (*CBGetFriendListCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMGetFriendListCallback
	{
		CBGetFriendListCallbackOnSuccess OnSuccess;
		CBGetFriendListCallbackOnError OnError;
		void* data;
	}TIMGetFriendListCallback;

	typedef void (*CBGetFriendListV2CallbackOnSuccess) (TIMFriendMetaInfoHandle meta_handle, TIMFriendListElemHandle* handles, uint32_t num, void* data);
	typedef void (*CBGetFriendListV2CallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMGetFriendListV2Callback
	{
		CBGetFriendListV2CallbackOnSuccess OnSuccess;
		CBGetFriendListV2CallbackOnError OnError;
		void* data;
	}TIMGetFriendListV2Callback;

	typedef void (*CBAddFriendCallbackOnSuccess) (TIMFriendResultHandle* handles, uint32_t num, void* data);
	typedef void (*CBAddFriendCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMAddFriendCallback
	{
		CBAddFriendCallbackOnSuccess OnSuccess;
		CBAddFriendCallbackOnError OnError;
		void* data;
	}TIMAddFriendCallback;

	typedef void (*CBDelFriendCallbackOnSuccess) (TIMFriendResultHandle* handles, uint32_t num, void* data);
	typedef void (*CBDelFriendCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMDelFriendCallback
	{
		CBDelFriendCallbackOnSuccess OnSuccess;
		CBDelFriendCallbackOnError OnError;
		void* data;
	}TIMDelFriendCallback;

	typedef void (*CBFriendResponseCallbackOnSuccess) (TIMFriendResultHandle* handles, uint32_t num, void* data);
	typedef void (*CBFriendResponseCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMFriendResponseCallback
	{
		CBFriendResponseCallbackOnSuccess OnSuccess;
		CBFriendResponseCallbackOnError OnError;
		void* data;
	}TIMFriendResponseCallback; 

	typedef void (*CBGetFriendPendencyListCallbackOnSuccess) (TIMFriendPendencyMetaHandle meta, TIMFriendPendencyItemHandle* handles, uint32_t num, void* data);
	typedef void (*CBGetFriendPendencyListCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMGetFriendPendencyListCallback
	{
		CBGetFriendPendencyListCallbackOnSuccess OnSuccess;
		CBGetFriendPendencyListCallbackOnError OnError;
		void* data;
	}TIMGetFriendPendencyListCallback;
	
	typedef void(*CBGetFriendFurtureListCallbackOnSuccess) (TIMFriendFutureMetaHandle meta, TIMFriendFutureItemHandle* handles, uint32_t num, void* data);
	typedef void(*CBGetFriendFurtureListCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMGetFriendFurtureListCallback
	{
		CBGetFriendFurtureListCallbackOnSuccess OnSuccess;
		CBGetFriendFurtureListCallbackOnError OnError;
		void* data;
	}TIMGetFriendFurtureListCallback;

	typedef void(*CBDelPendencyFriendCallbackOnSuccess) (TIMFriendResultHandle* handles, uint32_t num, void* data);
	typedef void(*CBDelPendencyFriendCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMDelPendencyFriendCallback
	{
		CBDelPendencyFriendCallbackOnSuccess OnSuccess;
		CBDelPendencyFriendCallbackOnError OnError;
		void* data;
	}TIMDelPendencyFriendCallback;

	typedef void(*CBDelRecommendFriendCallbackOnSuccess) (TIMFriendResultHandle* handles, uint32_t num, void* data);
	typedef void(*CBDelRecommendFriendCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMDelRecommendFriendCallback
	{
		CBDelRecommendFriendCallbackOnSuccess OnSuccess;
		CBDelRecommendFriendCallbackOnError OnError;
		void* data;
	}TIMDelRecommendFriendCallback;

	typedef void(*CBDelDecideFriendCallbackOnSuccess) (TIMFriendResultHandle* handles, uint32_t num, void* data);
	typedef void(*CBDelDecideFriendCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMDelDecideFriendCallback
	{
		CBDelDecideFriendCallbackOnSuccess OnSuccess;
		CBDelDecideFriendCallbackOnError OnError;
		void* data;
	}TIMDelDecideFriendCallback;

	
	typedef void(*CBSearchNickNameCallbackOnSuccess) (uint64_t total_num, TIMProfileHandle* handles, uint32_t num, void* data);
	typedef void(*CBSearchNickNameCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMSearchNickNameCallback
	{
		CBSearchNickNameCallbackOnSuccess OnSuccess;
		CBSearchNickNameCallbackOnError OnError;
		void* data;
	}TIMFriendSearchNickNameCB;

	typedef void(*CBAddFriendGroupCallbackOnSuccess) (TIMFriendResultHandle* handles, uint32_t num, void* data);
	typedef void(*CBAddFriendGroupCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMAddFriendGroupCallback
	{
		CBAddFriendGroupCallbackOnSuccess OnSuccess;
		CBAddFriendGroupCallbackOnError OnError;
		void* data;
	}TIMAddFriendGroupCallback;

	typedef void(*CBAddFriends2GroupCallbackOnSuccess) (TIMFriendResultHandle* handles, uint32_t num, void* data);
	typedef void(*CBAddFriends2GroupCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMAddFriends2GroupCallback
	{
		CBAddFriends2GroupCallbackOnSuccess OnSuccess;
		CBAddFriends2GroupCallbackOnError OnError;
		void* data;
	}TIMAddFriends2GroupCallback;

	typedef void(*CBDelFriendsFromGroupCallbackOnSuccess) (TIMFriendResultHandle* handles, uint32_t num, void* data);
	typedef void(*CBDelFriendsFromGroupCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMDelFriendsFromGroupCallback
	{
		CBDelFriendsFromGroupCallbackOnSuccess OnSuccess;
		CBDelFriendsFromGroupCallbackOnError OnError;
		void* data;
	}TIMDelFriendsFromGroupCallback;

	typedef void(*CBGetFriendsGroupCallbackOnSuccess) (TIMFriendGroupInfoHandle* handles, uint32_t num, void* data);
	typedef void(*CBGetFriendsGroupCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMGetFriendsGroupGroupCallback
	{
		CBGetFriendsGroupCallbackOnSuccess OnSuccess;
		CBGetFriendsGroupCallbackOnError OnError;
		void* data;
	}TIMGetFriendsGroupGroupCallback;

	typedef void(*CBCheckFriendsGroupCallbackOnSuccess) (TIMCheckFriendResultHandle* handles, uint32_t num, void* data);
	typedef void(*CBCheckFriendsGroupCallbackOnError) (int code, const char* desc, void* data);
	typedef struct _T_TIMCheckFriendsGroupGroupCallback
	{
		CBCheckFriendsGroupCallbackOnSuccess OnSuccess;
		CBCheckFriendsGroupCallbackOnError OnError;
		void* data;
	}TIMCheckFriendsGroupGroupCallback;

	//interface
	/**
	Description:	�����Լ����ǳ�
	@param	[in]	nick	�ǳ�
	@param	[in]	len		�ǳƳ���
	@param	[in]	cb		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMSetNickName(char* nick, uint32_t len, TIMCommCB * cb);
	/**
	Description:	���ú�����֤��ʽ
	@param	[in]	type	��֤��ʽ
	@param	[in]	cb		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMSetAllowType(E_TIMFriendAllowType type, TIMCommCB * cb);
	/**
	Description:	�����Լ���ͷ��
	@param	[in]	url		ͷ��url
	@param	[in]	url_len	url����
	@param	[in]	cb		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMSetFaceURL(char* url, uint32_t url_len, TIMCommCB * cb);
	/**
	Description:	���ú��ѱ�ע
	@param	[in]	id		����ID	
	@param	[in]	remark	��ע	
	@param	[in]	remark_len	��ע����
	@param	[in]	cb			�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMSetFriendRemark(const char* id, const char* remark, const uint32_t remark_len, TIMCommCB * cb);
	/**
	Description:	��ȡ�Լ������� 
	@param	[in]	cb	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetSelfProfile(TIMGetSelfProfileCallback* cb);
	/**
	Description:	��ȡ���ѵ�����
	@param	[in]	id		Ҫ��ȡ�ĺ���ID����
	@param	[in]	num		Ҫ��ȡ�ĺ��Ѹ���
	@param	[in]	cb		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetFriendProfile(const char** id, uint32_t num, TIMGetFriendProfileCallback* cb);
	/**
	Description:	��ȡ�κ��˵�����
	@param	[in]	id		Ҫ��ȡ���û�ID����
	@param	[in]	num		Ҫ��ȡ���û�����
	@param	[in]	cb		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetUserProfile(const char** id, uint32_t num, TIMGetFriendProfileCallback* cb);

	TIM_DECL void TIMSetPartialProfile(TIMSetProfileOptionHandle opt, TIMCommCB* cb);
	TIM_DECL void TIMGetPartialProfile(const char** id, uint32_t num, TIMGetProfileOptionHandle handle, TIMGetFriendProfileCallback* cb);


	// resp: sIdentifier, sNickname, sRemark
	/**
	Description:	��ȡ���к���
	@param	[in]	cb	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetFriendList(TIMGetFriendListCallback * cb);
	
	// req: sIdentifier, sRemark, sAddWording, sAddSource
	// resp: sIdentifier, result
	/**
	Description:	������Ӻ���
	@param	[in]	handles		��Ӻ���handle����
	@param	[in]	num			��Ӻ��Ѹ���
	@param	[in]	cb			�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMAddFriend(TIMAddFriendHandle* handles, uint32_t num, TIMAddFriendCallback * cb);
	// req: sIdentifier
	// resp: sIdentifier, result
	// TYPE: 1-������� 2-˫�����
	/**
	Description:	ɾ������
	@param	[in]	type	ɾ�����ͣ�������ѡ�˫����ѣ�
	@param	[in]	id		Ҫɾ���ĺ���ID����
	@param	[in]	num		ɾ���ĺ��Ѹ���
	@param	[in]	cb		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMDelFriend(int type, char** id, uint32_t num, TIMDelFriendCallback * cb);

	// req: sIdentifier, sResponse
	// resp: sIdentifier, result
	/**
	Description:	ͬ��/�ܾ� ��������
	@param	[in]	handles	TIMFriendResponseHandle����
	@param	[in]	num		����Ԫ�ظ���
	@param	[in]	cb		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMFriendResponse(TIMFriendResponseHandle* handles, uint32_t num, TIMFriendResponseCallback * cb);

	// resp: sIdentifier, result
	/**
	Description:	����û���������
	@param	[in]	id	�û�ID����
	@param	[in]	num �û�����
	@param	[in]	cb	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMAddBlackList(const char** id, uint32_t num, TIMFriendshipActionCB * cb);
	// resp: sIdentifier, result
	/**
	Description:	���û��Ӻ�����ɾ��
	@param	[in]	id	�û�ID����
	@param	[in]	num	�û�����
	@param	[in]	cb	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMDelBlackList(const char** id, uint32_t num, TIMFriendshipActionCB * cb);
	// resp: sIdentifier
	/**
	Description:	��ȡ�������б�
	@param	[in]	cb	�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetBlackList(TIMFriendshipActionCB * cb);

	// resp: sNickname
	TIM_DECL void TIMSearchUserUseIdentifier(char* id, TIMFriendshipActionCB * cb);

	/**
	*  ��ȡ�����б�����������ҳ���Զ�����ȡ�ֶΣ�
	*
	*  @param flags ������Ҫ��ȡ���ֶ�
	*  @param custom �Զ����ֶΣ�Ŀǰ����֧�֣�
	*  @param meta ����Ԫ��Ϣ�����TIMFriendMetaInfo˵����
	*  @param cb   �ص�
	*/
	TIM_DECL void TIMGetFriendListV2(TIMProfileFlag flag, TIMProfileCustomInfoHandle custom, TIMFriendMetaInfoHandle meta, TIMGetFriendListV2Callback* cb);

	/**
	*  ͨ�������ȡδ�������б�
	*
	*  @param meta  ������Ϣ����ϸ�ο�TIMFriendPendencyMetaHandle
	*  @param type  ��ȡ���ͣ��ο�TIMPendencyGetType��
	*  @param cb   �ص�
	*/
	TIM_DECL void TIMGetPendencyFromServer(TIMFriendPendencyMetaHandle meta, TIMPendencyGetType type, TIMGetFriendPendencyListCallback* cb);

	/**
	*  δ�������Ѷ��ϱ�
	*
	*  @param timestamp �Ѷ�ʱ�������ʱ�����ǰ����Ϣ������Ϊ�Ѷ�
	*  @param cb  �ص�
	*/
	TIM_DECL void TIMPendencyReport(uint64_t timestamp , TIMCommCB* cb);


	/**
	*  �Ƽ������Ѷ��ϱ�
	*
	*  @param timestamp �Ѷ�ʱ�������ʱ�����ǰ����Ϣ������Ϊ�Ѷ�
	*  @param cb  �ص�
	*/
	TIM_DECL void TIMRecommendReport(uint64_t timestamp, TIMCommCB* cb);

	/**
	Description:	δ������ͺ����Ƽ���ȡ
	@param	[in]	flag			��ȡ�����ϱ�ʶ
	@param	[in]	type			��ȡ�����ͣ���λ����
	@param	[in]	custom_handle	�Զ����ֶΣ�����δʵ�֣���NULL��
	@param	[in]	meta			������Ϣ���μ�TIMFriendFutureMetaHandle
	@param	[in]	cb				�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetFutureFriends(TIMProfileFlag flag, TIMFutureFriendType type, TIMProfileCustomInfoHandle custom_handle, TIMFriendFutureMetaHandle meta, TIMGetFriendFurtureListCallback* cb);


	/**
	*  δ������ɾ��
	*
	*  @param type  δ����������
	*  @param id	Ҫɾ����δ�������б�
	*  @param cb	�ص�
	*/
	TIM_DECL void TIMDeletePendency(TIMPendencyGetType type, const char** id, uint32_t num, TIMDelPendencyFriendCallback* cb);


	/**
	*  �Ƽ�����ɾ��
	*
	*  @param id	Ҫɾ�����Ƽ������б�
	*  @param cb    �ص�
	*/
	TIM_DECL void TIMDeleteRecommend(const char** id, uint32_t num, TIMDelRecommendFriendCallback* cb);

	/**
	*  �Ѿ�ɾ��
	*
	*  @param id	Ҫɾ�����Ѿ��б�
	*  @param cb    �ص�
	*/
	TIM_DECL void TIMDeleteDecide(const char** id, uint32_t num, TIMDelDecideFriendCallback * cb);

	/**
	Description:	ͨ���ǳ�ģ�������û�
	@param	[in]	nick		Ҫ�������ǳƹؼ���
	@param	[in]	nick_len	�û��ǳƳ���
	@param	[in]	page_idx	��ҳ��
	@param	[in]	num_perpage	ÿҳ�û���Ŀ
	@param	[in]	cb			�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMSearchNickName(const char* nick, uint32_t nick_len, uint64_t page_idx, uint64_t num_perpage, TIMFriendSearchNickNameCB* cb);

	/**
	Description:	�������ѷ���
	@param	[in]	handle		���ѷ��������б�
	@param	[in]	friend_ids	����ID����
	@param	[in]	friend_num	���Ѹ���
	@param	[in]	cb			�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMCreateFriendGroup(TIMFriendGroupNamesHandle handle, const char** friend_ids, uint32_t friend_num, TIMAddFriendGroupCallback* cb);

	/**
	Description:	ɾ�����ѷ���
	@param	[in]	handle	���ѷ��������б�
	@param	[in]	cb		�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMDeleteFriendGroup(TIMFriendGroupNamesHandle handle, TIMCommCB * cb);

	/**
	Description:	��Ӻ��ѵ�ĳ����
	@param	[in]	group_name	���ѷ�������
	@param	[in]	name_len	���ѷ���������
	@param	[in]	friend_ids  Ҫ��ӵ������еĺ���ID����
	@param	[in]	friend_num	Ҫ��ӵ������еĺ��Ѹ���
	@param	[in]	cb			�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMAddFriends2Group(const char* group_name, uint32_t name_len, const char** friend_ids, uint32_t friend_num, TIMAddFriends2GroupCallback* cb);

	/**
	Description:	�ӷ�����ɾ������
	@param	[in]	group_name	���ѷ�������
	@param	[in]	name_len	���ѷ���������
	@param	[in]	friend_ids	Ҫ�ӷ�����ɾ���ĺ���ID����
	@param	[in]	friend_num	Ҫ�ӷ�����ɾ���ĺ���ID����
	@param	[in]	cb			�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMDelFriendsFromGroup(const char* group_name, uint32_t name_len, const char** friend_ids, uint32_t friend_num, TIMDelFriendsFromGroupCallback* cb);

	/**
	Description:	���������ѷ���
	@param	[in]	old_name		ԭ���ķ�����
	@param	[in]	old_name_len	ԭ���ķ���������
	@param	[in]	new_name		�µķ�����
	@param	[in]	new_name_len	�µķ���������
	@param	[in]	cb				�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMRenameFriendGroup(const char* old_name, uint32_t old_name_len, const char* new_name, uint32_t new_name_len, TIMCommCB * cb);

	/**
	Description:	��ȡָ���ĺ��ѷ�����Ϣ
	@param	[in]	handle			���ѷ��������б�
	@param	[in]	need_get_id_list�Ƿ���Ҫ��ȡ�����б�
	@param	[in]	cb				�ص�
	@return			void
	@exception      none
	*/
	TIM_DECL void TIMGetFriendGroup(TIMFriendGroupNamesHandle handle, bool need_get_id_list, TIMGetFriendsGroupGroupCallback* cb);

	TIM_DECL void TIMCheckFriends(TIMFriendCheckInfoHandle check_info, TIMCheckFriendsGroupGroupCallback* cb);
}
#endif