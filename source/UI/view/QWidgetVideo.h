#pragma once

#include <QWidget>
#include "ui_QWidgetVideo.h"
#include "commonType.h"
#include "VideoService.h"
#include "QWindowWhiteBoard.h"

enum MainRenderType
{
	MAIN_RENDER_CAMERA = 1,
	MAIN_RENDER_BOARD = 2,
	MAIN_RENDER_SCREEN = 3,
	MAIN_RENDER_MEDIA = 4,
};


struct videoUserInfo
{
	bool mainUser;
	MemberItem memberInfo;
};

class QWidgetVideo : public QWidget
{
	Q_OBJECT

public:
	QWidgetVideo(QWidget *parent = Q_NULLPTR);
	~QWidgetVideo();

	//MAIN_RENDER_CAMERA:双击后，dis_main显示对应小屏幕的画面。
	//其他类型:dis_main隐藏，对应区域可用于显示其他内容，如白板等
	void setMainRender(MainRenderType mainType);
	void setRenders();
	void setUserType(UserType userType);
	void setVideoInfo(const std::vector<VideoHandsupItem> & vVideoUserInfo, bool bSetupVideo = true);
	void setId(const char * id, const char * nickname);
	void initVideoInfo();
	void openScreenShareArea(int32 left, int32 top, int32 right, int32 bottom);
	void changeScreenShareArea(int32 left, int32 top, int32 right, int32 bottom);
	void closeScreenShareArea();

	void fullScreen();

	void openMediaFile(const std::string& szFileName);

private:
	Ui::QWidgetVideo ui;

	QWindowWhiteBoard * whiteBoard = nullptr;
	QVBoxLayout *vBoardLayout;
	UserType m_userType = TEACHER_USER_TYPE;
	MainRenderType m_mainRenderType = MAIN_RENDER_CAMERA;
	int m_videoNum = 0;
	std::vector<VideoHandsupItem> m_vVideoUserInfo;
	std::vector<QLabel*> m_vUserNameLabel;
	std::vector<QLabel*> m_vTagLabel;
	std::vector<QLabel*> m_vMuteLabel;
	std::vector<QLabel*> m_vBGLabel;
	std::vector<QFrame*> m_vReqFrame;
	std::map<int, QWidgetVideoRender*> m_mapVideoRender;
	//std::map<std::string, int> m_mapVideoHandsList;
	//std::map<std::string, int> m_mapRender;
	int m_scrollbar_index = 0;
	std::string m_id;
	std::string m_nickname;
	int m_mainRenderIndex = -1;
	int m_mainSpeaking = -1;
	Vector< Pair<String/*id*/, String/*name*/> > m_cameraList;
	Vector< Pair<String/*id*/, String/*name*/> > m_micList;
	bool m_bMute;
private:
	void initUI();
	std::string getNickName(const char *userID);

signals:
	void modifyMemberInfo(ModifyMemberInfoItem info);
	void modifyConfInfo(std::string home_screen);
	void notifyGrandPermission(std::string userID, int permission);
	void notifyEndInteract(const char* userID);
	void notifyToast(QString strText);

private slots:
	void on_dis_doubleClicked(int index);
	void on_dis_escPressed(int index);
	void on_dis_actSpeaking(int index, bool open);
	void on_dis_actCamera(int index, bool open);
	void on_dis_actMic(int index, bool open);
	void on_dis_actEnd(int index);
	void on_scrollbar_videos_sliderReleased();
	void on_btn_accept2_clicked();
	void on_btn_accept3_clicked();
	void on_btn_accept4_clicked();
	void on_btn_accept5_clicked();
	void on_btn_deny2_clicked();
	void on_btn_deny3_clicked();
	void on_btn_deny4_clicked();
	void on_btn_deny5_clicked();

};
