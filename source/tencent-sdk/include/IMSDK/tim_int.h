#ifndef __TIM_INT_MANAGER_H__
#define __TIM_INT_MANAGER_H__

#include <list>

#include "tim_comm.h"

namespace imcore {

enum E_TIMStreamEncode
{
    //����FLV�������Ƶ��URL
    HLS = 0x01,
    //����HLS�������Ƶ��URL
    FLV = 0x02,
	//ͬʱ����HLS��FLV�������Ƶ��URL
    HLS_AND_FLV = 0x03,
	//RAW����
	RAW = 0X04,
	//RTMP
	RTMP = 0X05,
	//HLS AND RTMP
	HLS_AND_RTMP = 0X06,
};

enum E_TIMSDKType
{
	SDKType_Normal = 0x01, // ��ͨ����sdk
	SDKType_IOTCamara,	   // ��ͨ��������ͷsdk
	SDKType_CoastCamara,   // ��������ͷsdk
};

enum RateType
{
	RATE_TYPE_ORIGINAL = 0,//ԭʼ����
	RATE_TYPE_550 = 10,//��������550K
	RATE_TYPE_900 = 20,//��������900K
};

struct TIMLiveUrl
{
    int encode_type; //��Ƶ����������
    std::string url; //��Ƶ������URL
	RateType rate_type; //���ʵ�λ��Ϣ
};

struct TIMStreamRsp
{
	TIMStreamRsp(): channel_id(0){}
	std::list<TIMLiveUrl> urls;
	uint64_t channel_id;
	uint32_t tape_task_id; //¼�Ʊ��Ϊ¼�Ƶ�ʱ�����¼��task_id��Ч
};

struct TIMRoomInfo
{
	TIMRoomInfo():relation_id(0), room_id(0) {}
	int relation_id; // ������ID
	int room_id;	 // ����ID
};

struct TIMRecordParam
{
	TIMRecordParam()
		:class_id(0)
		, is_transcode(false)
		, is_screenshot(false)
		, is_watermark(false)
		, sdk_type(SDKType_Normal)
		, record_data_type(0)
		, record_type(0){}
	std::string filename;				//¼�ƺ���ļ���
	int class_id;				 		//��Ƶ����ID
	bool is_transcode;					//�Ƿ�ת��
	bool is_screenshot;				 	//�Ƿ��ͼ
	bool is_watermark;					//�Ƿ��ˮӡ
	std::list<std::string> tags;	 	//��Ƶ��ǩ�б�
	E_TIMSDKType	sdk_type;
	uint32_t		record_data_type; //¼�Ƶ��������ͣ�0.����ͷ��Ƶ 1.����
	uint32_t		record_type; //¼�Ʊ�������ͣ�0.Record Video Media File 1.Record Audio Media File����Ƶ
};



struct TIMStreamParam
{
	TIMStreamParam()
		:encode(HLS), sdk_type(SDKType_Normal), push_data_type(0), tape_flag(0), watermark_flag(0), watermark_id(0) {}
	E_TIMStreamEncode encode;
	E_TIMSDKType sdk_type;
	std::string channel_name;
	std::string channel_desc;
	std::string channel_password;
	uint32_t	push_data_type; //�������������� 0:����ͷ 1������ 
	uint32_t	tape_flag;//�����Ƿ�¼�� 0.Ĭ�ϲ�¼��  1.¼�� ����������ʱ��Я��
	uint32_t	watermark_flag;//ˮӡ���: 0.Ĭ�ϲ���ˮӡ 1.��ˮӡ������������ѡ��Ĭ��ԭʼ����
	uint32_t	watermark_id;//ˮӡid: 0.Ĭ��ˮӡ,����ˮӡID��Ҫͨ������̨�ӿ�ȥ��ȡ;
	std::vector<RateType> rpt_rate_type;//֧�ֶ����ʹۿ�������������ѡ��Ĭ��ԭʼ����
};

class TIM_DECL TIMIntManager {
public:
	static TIMIntManager &get();
	TIMIntManager();
	uint64_t tiny_id();
	void Init(TIMCallBack *cb);
	void Uninit(TIMCallBack *cb);
	void UserIdToTinyId(std::list<TIMUser> &reqList, TIMValueCallBack<const std::list<TIMUser> &> *cb, uint64_t timeout = 5000);
    void TinyIdToUserId(std::list<uint64_t> &tinyids, TIMValueCallBack<const std::list<TIMUser> &> *cb, uint64_t timeout = 5000);
	void MultiVideoApp(const std::string &req, TIMValueCallBack<const std::string &> *cb, uint64_t timeout = 5000);
	void MultiVideoInfo(const std::string &req, TIMValueCallBack<const std::string &> *cb, uint64_t timeout = 5000);
	void RequestSharpSvr(uint64_t touin, const std::string &req, TIMValueCallBack<const std::string &> *cb, uint64_t timeout = 5000);
	void ResponseSharpSvr(uint64_t touin, const std::string &req, TIMValueCallBack<const std::string &> *cb, uint64_t timeout = 5000);
	void SetSharpSvrPushCallBack(TIMValueCallBack<const std::string &> *cb);
	TIMValueCallBack<const std::string &> *GetSharpSvrPushCallBack();
	void SetSharpSvrRspCallBack(TIMValueCallBack<const std::string &> *cb);
	TIMValueCallBack<const std::string &> *GetSharpSvrRspCallBack();
	void Request(const std::string &cmd, const std::string &req, TIMValueCallBack<const std::string &> *cb, uint64_t timeout = 5000);
	void Request(const std::string &cmd, const std::string &req, TIMValueCallBack<const std::string &> *cb, bool resend, uint64_t timeout = 5000);
	void RequestMultiVideo(const std::string &cmd, const std::string &req_body, TIMValueCallBack<const std::string &> *cb, uint64_t timeout = 5000);

	bool ready();

	 /**
     * ����ʼ��������
     *
     * @room_info			������Ϣ
     * @para				��������
     * @param cb         ��onSuccess�����з��ط����õ�url�б�, ���liveUrl
     */
	void RequestMultiVideoStreamerStart(const TIMRoomInfo& room_info, const TIMStreamParam& para, TIMValueCallBack<TIMStreamRsp&> *cb);
	  /**
     * ���������������
     *
	 * @room_info			������Ϣ
	 * @para				��������
	 * @channel_ids			RequestMultiVideoStreamerStart ���ص�channel_id
     * @param cb			�ص�����
     */
	void RequestMultiVideoStreamerStop(const TIMRoomInfo& room_info, const TIMStreamParam& para, const std::list<uint64_t>& channel_ids, TIMCallBack *cb);
	   /**
     * ����ʼ¼������
     *
	 * @room_info			������Ϣ
	 * @para				¼�Ʋ���
     * @param cb
     */
	void RequestMultiVideoRecorderStart(const TIMRoomInfo& room_info, const TIMRecordParam& para, TIMCallBack* cb);
	   /**
     * �������¼������
     *
     * @param relation_id   ������ID
     * @param room_id	    ����ID
     * @param signature		ǩ��
     * @param cb
     */
	void RequestMultiVideoRecorderStop(const TIMRoomInfo& room_info, const TIMRecordParam& para, TIMValueCallBack<std::list<std::string>&> *cb);


	/**
	 *  ���Ͷ�������Ƶ����
	 *
	 *  @param bussType ҵ������
	 *  @param authType ��Ȩ����
	 *  @param authid  ��ȨID
	 *  @param requestType:
	 *  1-----���𷢷�������Ƶ����
		2-----����ȡ������Ƶ����
		3-----���շ���������Ƶ����
		4-----���շ��ܾ�����Ƶ����
	 *  @param receivers  ����Щ�˷�������
	 */
	void RequestVideoInvitation(int bussType, int authType, int auth_id, int requestType, std::list<TIMUser>& receivers, TIMCallBack* cb);

    /**
     * ���������ϱ�����(internal)
     * @param type ҵ������
     * @param data ģ���Զ�����ϱ�����
     * @param cb �ص��ӿ�
     */
	void RequestQualityReport(int type, std::string& data, TIMCallBack* cb);

	/**
	* ����Ƶ�������
	*
	* @param bussi_type ҵ������
	* @param auth_type  ��Ȩ����
	* @param cb         �ص��ӿ�
	*/
	void RequestMeasureSpeed(uint16_t bussi_type, uint16_t auth_type, TIMCallBack* cb);
private:
	bool ready_;
};

}

#endif
