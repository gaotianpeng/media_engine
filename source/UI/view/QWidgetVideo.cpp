#include "QWidgetVideo.h"
#include "Context.h"
#include "QWidgetToast.h"
#include <QMetaType> 

extern QWidget * mainWindow;

QWidgetVideo::QWidgetVideo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::StrongFocus);
	initUI();

	GetILive()->getCameraList(m_cameraList);
	GetILive()->getMicList(m_micList);

	qRegisterMetaType<ModifyMemberInfoItem>("ModifyMemberInfoItem");
	connect(this, SIGNAL(modifyMemberInfo(ModifyMemberInfoItem)), mainWindow, SLOT(on_modifyMemberInfo(ModifyMemberInfoItem)));
	connect(this, SIGNAL(notifyGrandPermission(std::string, int)), mainWindow, SLOT(on_grandPermission(std::string, int)));
	connect(this, SIGNAL(notifyEndInteract(const char*)), mainWindow, SLOT(on_endInteract(const char*)));
	connect(this, SIGNAL(modifyConfInfo(std::string)), mainWindow, SLOT(on_modifyConfInfo(std::string)));
	connect(this, SIGNAL(notifyToast(QString)), mainWindow, SLOT(on_notifyToast(QString)));
}

QWidgetVideo::~QWidgetVideo()
{
}

void QWidgetVideo::setMainRender(MainRenderType mainType)
{
	m_mainRenderType = mainType;

	switch (mainType)
	{
	case MAIN_RENDER_CAMERA:
		{
			ui.stacked_main->setCurrentIndex(0);
			VideoService::shared_instance()->setMainRenderType(VIDEO_SRC_TYPE_CAMERA);
		}
		break;
	case MAIN_RENDER_BOARD:
		{
			//白板数据不通过ilive，具体new还需要对接。
			if (!whiteBoard)
			{
				bool enableDraw = true;
				if (m_userType == STUDENT_USER_TYPE)
				{
					enableDraw = false;
					whiteBoard = new QWindowWhiteBoard(enableDraw);
//					whiteBoard->setGeometry(0, 0, ui.page_board->size().width(), ui.page_board->size().height());
				}
				else
					whiteBoard = new QWindowWhiteBoard(enableDraw);
				vBoardLayout = new QVBoxLayout(ui.page_board);
				vBoardLayout->setMargin(0);
				vBoardLayout->addWidget(whiteBoard);
				ui.page_board->setLayout(vBoardLayout);
				whiteBoard->show();
			}
			ui.stacked_main->setCurrentIndex(1);
			VideoService::shared_instance()->setMainRenderType(VIDEO_SRC_TYPE_NONE);
		}
		break;
	case MAIN_RENDER_SCREEN:
		{
			ui.stacked_main->setCurrentIndex(0);
			VideoService::shared_instance()->setMainRenderType(VIDEO_SRC_TYPE_SCREEN);
		}
		break;
	case MAIN_RENDER_MEDIA:
		{
			ui.stacked_main->setCurrentIndex(2);
			VideoService::shared_instance()->setMainRenderType(VIDEO_SRC_TYPE_MEDIA);
		}
		break;
	default:
		break;
	}
}

void QWidgetVideo::setRenders()
{
	VideoService::shared_instance()->resetRenders();
	for (int i = 1; i <= 5; i++)
	{
		VideoService::shared_instance()->setRenders(m_mapVideoRender[i], VIDEO_RENDER_SUB);
	}
	VideoService::shared_instance()->setRenders(ui.dis_main, VIDEO_RENDER_MAIN);
	//VideoService::shared_instance()->updateRenderMap(m_mainRenderIndex, m_mapRender);
}

void QWidgetVideo::setUserType(UserType userType)
{
	m_userType = userType;
	ui.page_media->setUserType(userType);
}

void QWidgetVideo::setVideoInfo(const std::vector<VideoHandsupItem> & vVideoUserInfo, bool bSetupVideo)
{
	m_videoNum = vVideoUserInfo.size();
	ui.scrollbar_videos->setMaximum(m_videoNum - 5);
	ui.scrollbar_videos->setValue(0);

	//m_vVideoUserInfo = vVideoUserInfo;
	m_vVideoUserInfo.clear();

	if (bSetupVideo)
		VideoService::shared_instance()->resetRenderMap();
	//m_mapRender.clear();
	int renderNum = 0;

	MenuInfo menuInfo;
	menuInfo.camera = false;
	menuInfo.fullScreen = false;
	menuInfo.mainDis = false;
	menuInfo.mic = false;
	menuInfo.teacher = false;  //自己的不受影响

	if (m_cameraList.size() != 0 && m_userType == TEACHER_USER_TYPE)
	{
		const char * id = m_cameraList[0].first.c_str();
		//m_mapRender[m_id] = m_mapRender.size();
		menuInfo.teacher = true;
		if (bSetupVideo)
			VideoService::shared_instance()->startLocalPreview(id, "0");
		
		m_vBGLabel[0]->hide();
		m_vMuteLabel[0]->hide();
		m_vUserNameLabel[renderNum]->setText(m_nickname.c_str());
		renderNum = 1;
		menuInfo.camera = true;
		menuInfo.mic = true;
		m_mapVideoRender[1]->setMenuInfo(menuInfo);
		m_mapVideoRender[1]->beginRender();

		VideoHandsupItem selfmember;
		selfmember.id = m_id;
		selfmember.nickName = m_nickname;
		selfmember.mic = true;
		selfmember.camera = true;
		selfmember.screen = false;
		selfmember.video = false;
		m_vVideoUserInfo.push_back(selfmember);
	}

	for (int i = 0; i < m_videoNum && renderNum < SUB_RENDER_MAX;i++)
	{
		if (vVideoUserInfo[i].id != m_id)
		{
			m_vVideoUserInfo.push_back(vVideoUserInfo[i]);
			//m_mapRender[vVideoUserInfo[i].memberInfo.id] = m_mapRender.size();
			m_vBGLabel[renderNum]->hide();
			
			m_vUserNameLabel[renderNum]->setText(vVideoUserInfo[i].nickName.c_str());
			menuInfo.camera = vVideoUserInfo[i].camera;
			menuInfo.mic = vVideoUserInfo[i].mic;

			renderNum++;
			m_mapVideoRender[renderNum]->setMenuInfo(menuInfo);
			
			if (i >= 0 && i < SUB_RENDER_MAX)
			{
				if (vVideoUserInfo[i].status == STATUS_HANDING)
				{
					m_mapVideoRender[renderNum]->stopRender();
					if(m_userType == TEACHER_USER_TYPE && renderNum >= 2)
						m_vReqFrame[renderNum-2]->show();
				}
				else if (vVideoUserInfo[i].status == STATUS_ONSTAGE) //在台上
				{
					m_mapVideoRender[renderNum]->stopRender();
					if (renderNum >= 2)
						m_vReqFrame[renderNum-2]->hide();
					//if (!vVideoUserInfo[i].mic && vVideoUserInfo[i].camera) //麦克风被禁，显示静音图标
					//{
					//	m_vMuteLabel[renderNum-1]->show();					
					//}
					//else
					//{
					//	m_vMuteLabel[renderNum - 1]->hide();
					//}
						
					if(bSetupVideo)
						VideoService::shared_instance()->startRemoteView(vVideoUserInfo[i].id.c_str());
					m_mapVideoRender[renderNum]->beginRender();

					//if (bSetupVideo && m_userType == STUDENT_USER_TYPE && vVideoUserInfo[i].id == Context::instance().userId())
					//{
					//	if (!vVideoUserInfo[i].mic && !m_bMute) //麦克风被禁
					//	{
					//		emit notifyToast(QString::fromWCharArray(L"已被老师静音"));
					//		m_bMute = true;
					//	}
					//	else if (vVideoUserInfo[i].mic && m_bMute)
					//	{
					//		emit notifyToast(QString::fromWCharArray(L"已被老师恢复声音"));
					//		m_bMute = false;
					//	}
					//}
				}
				else
				{
					if (m_userType == TEACHER_USER_TYPE)
						m_vReqFrame[renderNum-2]->hide();
				}



			}
		}
	}
	if (renderNum < SUB_RENDER_MAX)
	{
		for (int i = renderNum; i < SUB_RENDER_MAX; i++)
		{
			menuInfo.camera = false;
			menuInfo.mic = false;
			m_mapVideoRender[i + 1]->setMenuInfo(menuInfo);
			m_mapVideoRender[i + 1]->stopRender();
			m_vBGLabel[i]->show();
			m_vMuteLabel[i]->hide();
			QString str = QString::fromWCharArray(L"学生");
			str += QString::number(i);
			m_vUserNameLabel[i]->setText(str);
		}
	}

}

void QWidgetVideo::setId(const char * id, const char * nickname)
{
	m_id = id;
	m_nickname = nickname;
	VideoService::shared_instance()->setId(id);
}

void QWidgetVideo::initVideoInfo()
{
	if (m_userType == TEACHER_USER_TYPE)
	{
		if (!VideoService::shared_instance()->getMicOpen())
		{
			VideoService::shared_instance()->setMute(false);
		}
		if (m_cameraList.size() > 0)
		{
			const char * cameraid = m_cameraList[0].first.c_str();
			VideoService::shared_instance()->startLocalPreview(cameraid, "0");
		}
	}
	ModifyMemberInfoItem modifyItem;
	modifyItem.id = Context::instance().userId();
	modifyItem.mic = true;
	modifyItem.camera = true;
	modifyItem.screen = false;
	modifyItem.video = false;
	emit modifyMemberInfo(modifyItem);
}

void QWidgetVideo::openScreenShareArea(int32 left, int32 top, int32 right, int32 bottom)
{
	uint32 fps = 10;
	//VideoService::shared_instance()->setRenders(ui.dis_main, VIDEO_RENDER_SUB);
	//VideoService::shared_instance()->setCameraView(false);
	VideoService::shared_instance()->startScreenPreView(left, top, right, bottom, fps);
	ModifyMemberInfoItem modifyItem;
	modifyItem.id = Context::instance().userId();
	modifyItem.mic = true;
	modifyItem.camera = true;
	modifyItem.screen = true;
	modifyItem.video = false;

	emit modifyMemberInfo(modifyItem);
//	std::string home_screen = "screen#";
//	home_screen += m_id;
//	emit modifyConfInfo(home_screen);
}

void QWidgetVideo::changeScreenShareArea(int32 left, int32 top, int32 right, int32 bottom)
{
	VideoService::shared_instance()->changeScreenPreView(left, top, right, bottom);
}

void QWidgetVideo::closeScreenShareArea()
{
	VideoService::shared_instance()->stopScreenPreView();
	ModifyMemberInfoItem modifyItem;
	modifyItem.id = Context::instance().userId();
	modifyItem.mic = true;
	modifyItem.camera = true;
	modifyItem.screen = false;
	modifyItem.video = false;

	emit modifyMemberInfo(modifyItem);
}

void QWidgetVideo::initUI()
{
	VideoService::shared_instance()->setRenders(ui.dis_main, VIDEO_RENDER_MAIN);
	connect(ui.dis_main, SIGNAL(doubleClicked(int)), this, SLOT(on_dis_doubleClicked(int)));
	connect(ui.dis_main, SIGNAL(escPressed(int)), this, SLOT(on_dis_escPressed(int)));
	connect(ui.dis_main, SIGNAL(actSpeaking(int, bool)), this, SLOT(on_dis_actSpeaking(int, bool)));
	connect(ui.dis_main, SIGNAL(actCamera(int, bool)), this, SLOT(on_dis_actCamera(int, bool)));
	connect(ui.dis_main, SIGNAL(actMic(int, bool)), this, SLOT(on_dis_actMic(int, bool)));

	ui.dis_main->setRenderIndex(0);

	MenuInfo menuInfo;
	menuInfo.camera = false;
	menuInfo.fullScreen = false;
	menuInfo.mainDis = true;
	menuInfo.mic = false;
	ui.dis_main->setMenuInfo(menuInfo);

	m_mapVideoRender[1] = ui.dis_1;
	m_mapVideoRender[2] = ui.dis_2;
	m_mapVideoRender[3] = ui.dis_3;
	m_mapVideoRender[4] = ui.dis_4;
	m_mapVideoRender[5] = ui.dis_5;

	menuInfo.mainDis = false;
	m_bMute = false;

	for (int i = 1; i <= 5; i++)
	{
		VideoService::shared_instance()->setRenders(m_mapVideoRender[i], VIDEO_RENDER_SUB);
		m_mapVideoRender[i]->setRenderIndex(i);
		m_mapVideoRender[i]->setMenuInfo(menuInfo);
		connect(m_mapVideoRender[i], SIGNAL(doubleClicked(int)), this, SLOT(on_dis_doubleClicked(int)));
		connect(m_mapVideoRender[i], SIGNAL(escPressed(int)), this, SLOT(on_dis_escPressed(int)));
		connect(m_mapVideoRender[i], SIGNAL(actSpeaking(int, bool)), this, SLOT(on_dis_actSpeaking(int, bool)));
		connect(m_mapVideoRender[i], SIGNAL(actCamera(int, bool)), this, SLOT(on_dis_actCamera(int, bool)));
		connect(m_mapVideoRender[i], SIGNAL(actMic(int, bool)), this, SLOT(on_dis_actMic(int, bool)));
		connect(m_mapVideoRender[i], SIGNAL(actEnd(int)), this, SLOT(on_dis_actEnd(int)));
	}

	ui.scrollbar_videos->setMinimum(0);
	ui.scrollbar_videos->setMaximum(m_videoNum - 5);
	ui.scrollbar_videos->setSingleStep(1);

	m_vUserNameLabel.push_back(ui.label_user1);
	m_vUserNameLabel.push_back(ui.label_user2);
	m_vUserNameLabel.push_back(ui.label_user3);
	m_vUserNameLabel.push_back(ui.label_user4);
	m_vUserNameLabel.push_back(ui.label_user5);

	m_vTagLabel.push_back(ui.label_tag1);
	m_vTagLabel.push_back(ui.label_tag2);
	m_vTagLabel.push_back(ui.label_tag3);
	m_vTagLabel.push_back(ui.label_tag4);
	m_vTagLabel.push_back(ui.label_tag5);
	for (int i = 0; i < 5; i++)
	{
		m_vTagLabel[i]->hide();
	}

	m_vMuteLabel.push_back(ui.label_mute1);
	m_vMuteLabel.push_back(ui.label_mute2);
	m_vMuteLabel.push_back(ui.label_mute3);
	m_vMuteLabel.push_back(ui.label_mute4);
	m_vMuteLabel.push_back(ui.label_mute5);
	m_vMuteLabel.push_back(ui.label_mute_main);

	m_vBGLabel.push_back(ui.label_dis1_bg);
	m_vBGLabel.push_back(ui.label_dis2_bg);
	m_vBGLabel.push_back(ui.label_dis3_bg);
	m_vBGLabel.push_back(ui.label_dis4_bg);
	m_vBGLabel.push_back(ui.label_dis5_bg);

	for (int i = 0; i < 6; i++)
	{
		m_vMuteLabel[i]->hide();
	}

	m_vReqFrame.push_back(ui.req2);
	m_vReqFrame.push_back(ui.req3);
	m_vReqFrame.push_back(ui.req4);
	m_vReqFrame.push_back(ui.req5);

	ui.widget_camera_tip->hide();
}

std::string QWidgetVideo::getNickName(const char * userID)
{
	std::string nickName;
	for (std::vector<VideoHandsupItem>::iterator it = m_vVideoUserInfo.begin(); m_vVideoUserInfo.end() != it; ++it)
	{
		if (userID == it->id)
		{
			nickName = it->nickName;
			break;
		}
	}
	return nickName;
}

void QWidgetVideo::on_dis_doubleClicked(int index)
{
	//0 为index，即为dis_main接收到doubleClick
	if (index == 0)
	{
		if (m_userType == STUDENT_USER_TYPE || (m_userType == TEACHER_USER_TYPE && m_mainRenderType == MAIN_RENDER_SCREEN))
		{
			ui.dis_main->enterFullScreen();
		}
	}
	else
	{
		VideoService::shared_instance()->enterMainRender(index);
		m_mainRenderIndex = index;
		VideoService::shared_instance()->setRenders(ui.dis_main, VIDEO_RENDER_MAIN);
		m_vTagLabel[index - 1]->show();
		for (int i = 1; i <= 5; i++)
		{
			if (i != index)
			{
				m_mapVideoRender[i]->setFullScreen(false);
			}			
		}
		
	}
}

void QWidgetVideo::on_dis_escPressed(int index)
{
	VideoService::shared_instance()->exitMainRender(index);
	if (m_mainRenderIndex == index || 0 == index)
	{
		m_mapVideoRender[index]->setFullScreen(false);
		m_vTagLabel[index - 1]->hide();
		m_mainRenderIndex = -1;
	}
	ui.dis_main->update();
}

void QWidgetVideo::on_dis_actSpeaking(int index, bool open)
{
}

void QWidgetVideo::on_dis_actCamera(int index, bool open)
{
	if (index == 0)
	{
		if (m_mainRenderIndex != -1)
		{
			index = m_mainRenderIndex;
		}
		else
			return;
	}
	if (open)
	{
		m_vBGLabel[index - 1]->hide();
	}
	else
		m_vBGLabel[index - 1]->show();

	if (index > m_vVideoUserInfo.size())
	{
		return;
	}
	const char* id = m_vVideoUserInfo[index - 1].id.c_str();
	if (id == m_id)
	{
		VideoService::shared_instance()->setCameraView(open);
	}
	else
	{
		if (open)
		{
			VideoService::shared_instance()->startRemoteView(id);
		}
		else
		{
			VideoService::shared_instance()->stopRemoteView(id);
		}
	}

	ModifyMemberInfoItem modifyItem;
	modifyItem.id = m_vVideoUserInfo[index - 1].id;
	modifyItem.mic = m_vVideoUserInfo[index - 1].mic;
	modifyItem.camera = 0;
	modifyItem.screen = m_vVideoUserInfo[index - 1].screen;
	modifyItem.video = m_vVideoUserInfo[index - 1].video;
	m_vVideoUserInfo[index - 1].camera = 0;

	emit modifyMemberInfo(modifyItem);
}

void QWidgetVideo::on_dis_actMic(int index, bool open)
{
	if (index > m_vVideoUserInfo.size())
	{
		return;
	}
	const char* id = m_vVideoUserInfo[index - 1].id.c_str();
	if (id == m_id)
	{
		VideoService::shared_instance()->setMute(!open);
	}
	if (open)
	{
		m_vMuteLabel[index - 1]->hide();
	}
	else
	{
		m_vMuteLabel[index - 1]->show();
	}
	if (index > m_vVideoUserInfo.size())
	{
		return;
	}

	ModifyMemberInfoItem modifyItem;
	modifyItem.id = m_vVideoUserInfo[index - 1].id;
	modifyItem.mic = open;
	modifyItem.camera = m_vVideoUserInfo[index - 1].camera;
	modifyItem.screen = m_vVideoUserInfo[index - 1].screen;
	modifyItem.video = m_vVideoUserInfo[index - 1].video;
	m_vVideoUserInfo[index - 1].mic = open;

	emit modifyMemberInfo(modifyItem);
}

void QWidgetVideo::on_dis_actEnd(int index)
{
	if (index <= 1)
	{
		return;
	}
	m_vBGLabel[index - 1]->show();
	m_vMuteLabel[index - 1]->hide();
	QString str;
	str.sprintf("%d", index - 1);
	m_vUserNameLabel[index - 1]->setText(QString::fromWCharArray(L"学生") + str);
	if (index > m_vVideoUserInfo.size())
	{
		return;
	}
	const char* id = m_vVideoUserInfo[index - 1].id.c_str();
	VideoService::shared_instance()->stopRemoteView(id);
	emit notifyEndInteract(id);
}

void QWidgetVideo::on_scrollbar_videos_sliderReleased()
{
	int startIndex = ui.scrollbar_videos->value();
	if (startIndex < 0)
	{
		return;
	}
	m_scrollbar_index = startIndex;
	if (startIndex + SUB_RENDER_MAX > m_vVideoUserInfo.size())
	{
		return;
	}
	for (int i = 0; i < 5; i++)
	{
		m_vTagLabel[i]->hide();
	}

	std::vector<std::string> vTmpID;
	int labelCount = 0;
	for (int i = startIndex; i < startIndex + SUB_RENDER_MAX; i++)
	{
		vTmpID.push_back(m_vVideoUserInfo[i].id);
		bool bMainUser = m_mainSpeaking == i;
		if (bMainUser && (i - startIndex) >= 0 )
		{
			m_vTagLabel[i - startIndex]->show();
		}
		QString text = bMainUser ? m_vVideoUserInfo[i].nickName.c_str() + QStringLiteral("(管理员)") : m_vVideoUserInfo[i].nickName.c_str();
		m_vUserNameLabel[labelCount]->setText(text);
		labelCount++;
	}
	VideoService::shared_instance()->batchRemoteView(&vTmpID);
}

void QWidgetVideo::on_btn_accept2_clicked()
{
	VideoHandsupItem member = m_vVideoUserInfo[1];
	emit notifyGrandPermission(member.id, 1<<1|1<<2);
}

void QWidgetVideo::on_btn_accept3_clicked()
{
	VideoHandsupItem member = m_vVideoUserInfo[2];
	emit notifyGrandPermission(member.id, 1<<1|1<<2);
}

void QWidgetVideo::on_btn_accept4_clicked()
{
	VideoHandsupItem member = m_vVideoUserInfo[3];
	emit notifyGrandPermission(member.id, 1<<1|1<<2);
}

void QWidgetVideo::on_btn_accept5_clicked()
{
	VideoHandsupItem member = m_vVideoUserInfo[4];
	emit notifyGrandPermission(member.id, 1<<1|1<<2);
}

void QWidgetVideo::on_btn_deny2_clicked()
{
	VideoHandsupItem member = m_vVideoUserInfo[1];
	m_vReqFrame[0]->hide();
	m_vBGLabel[1]->show();
	m_vMuteLabel[1]->hide();
	m_vUserNameLabel[1]->setText(QString::fromWCharArray(L"学生1"));
	emit notifyGrandPermission(member.id, 0);
}

void QWidgetVideo::on_btn_deny3_clicked()
{
	VideoHandsupItem member = m_vVideoUserInfo[2];
	m_vReqFrame[1]->hide();
	m_vBGLabel[2]->show();
	m_vMuteLabel[2]->hide();
	m_vUserNameLabel[2]->setText(QString::fromWCharArray(L"学生2"));
	emit notifyGrandPermission(member.id, 0);
}

void QWidgetVideo::on_btn_deny4_clicked()
{
	VideoHandsupItem member = m_vVideoUserInfo[3];
	m_vReqFrame[2]->hide();
	m_vBGLabel[3]->show();
	m_vMuteLabel[3]->hide();
	m_vUserNameLabel[3]->setText(QString::fromWCharArray(L"学生3"));
	emit notifyGrandPermission(member.id, 0);
}

void QWidgetVideo::on_btn_deny5_clicked()
{
	VideoHandsupItem member = m_vVideoUserInfo[4];
	m_vReqFrame[3]->hide();
	m_vBGLabel[4]->show();
	m_vMuteLabel[4]->hide();
	m_vUserNameLabel[4]->setText(QString::fromWCharArray(L"学生4"));
	emit notifyGrandPermission(member.id, 0);
}

void QWidgetVideo::fullScreen()
{
	if (m_userType == STUDENT_USER_TYPE || (m_userType == TEACHER_USER_TYPE && m_mainRenderType == MAIN_RENDER_SCREEN))
	{
		ui.dis_main->enterFullScreen();
	}
}

void QWidgetVideo::openMediaFile(const std::string& szFileName)
{
	ui.page_media->openMediaFile(szFileName);
}
