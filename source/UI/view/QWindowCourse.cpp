#include "QWindowCourse.h"
#include "QDialogMessage.h"
#include <QShowEvent>
#include <QWindow>
#include "VideoService.h"
#include "Context.h"
#include "notify.h"
#include "txCloudLog.h"


QWidget * mainWindow = nullptr;
QWindowCourse::QWindowCourse(QWidget *parent)
	: QMainWindow(parent)
    , m_authInfo()
    , m_memberService()
    , widgetTab(nullptr)
	, widgetBar(nullptr)
    , memberPanel(nullptr)
    , imPanel(nullptr)
	, widgetSMain(nullptr)
	, m_accountInfo(nullptr)
{
	m_toast = nullptr;
	ui.setupUi(this);
	mainWindow = this;
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_Mapped );
	//setWindowModality(Qt::ApplicationModal);
	// 切换主线程
	qRegisterMetaType<txfunction>("txfunction");
	connect(this, SIGNAL(dispatch(txfunction)), this, SLOT(handle(txfunction)), Qt::QueuedConnection);
	

    IMService::shared_instance()->setRecvMsgCallBack(this);
}

QWindowCourse::~QWindowCourse()
{
    if (m_memberService)
    {
        m_memberService->setCallback(nullptr);
    }
}

void QWindowCourse::init(AccountAuth& info)
{
    m_authInfo = info.authInfo();
	m_accountInfo = &info;

    m_memberService = std::make_shared<MemberService>(m_authInfo);
	m_memberService->setCallback(this);

    if (m_authInfo.m_userType == TEACHER_USER_TYPE)
    {
        widgetTab = new QWidgetTab(ui.widget_main_share);
        QVBoxLayout *tabVBoxLayout = new QVBoxLayout(ui.widget_main_share);
        tabVBoxLayout->setMargin(0);
        tabVBoxLayout->addWidget(widgetTab);
        ui.widget_main_share->setLayout(tabVBoxLayout);
		widgetTab->init(m_authInfo);
        widgetTab->show();

        memberPanel = new QWidgetMemberPanel(ui.widget_member);
        memberPanel->setRoomId(m_authInfo.m_roomID);
        memberPanel->setRoomOwner(m_authInfo.m_roomOwner);
        memberPanel->setJoinUrl(m_authInfo.m_joinConfURL);
        memberPanel->setMainWindow(this);
        QVBoxLayout *memberVBoxLayout = new QVBoxLayout(ui.widget_member);
        memberVBoxLayout->setMargin(0);
        memberVBoxLayout->addWidget(memberPanel);
        ui.widget_member->setLayout(memberVBoxLayout);
        memberPanel->show();
    }
    else if (m_authInfo.m_userType == STUDENT_USER_TYPE)
    {
        ui.widget_member->hide();
        widgetSMain = new QWidgetSMain(ui.widget_main_share);
		widgetBar = new QWidgetBottomBar(m_memberService, ui.widget_main_share);
        QVBoxLayout *vboxLayout = new QVBoxLayout(ui.widget_main_share);
        vboxLayout->setMargin(0);
        vboxLayout->addWidget(widgetSMain);
		vboxLayout->addWidget(widgetBar);
        ui.widget_main_share->setLayout(vboxLayout);
        widgetSMain->show();
		widgetBar->show();
        connect(this, SIGNAL(sub_state_changed(int)), widgetSMain, SLOT(on_sub_state_changed(int)));
		connect(this, SIGNAL(mainWindowMoved(QPoint)), widgetBar, SLOT(on_mainWindow_moved(QPoint)));

        ui.widget_main->setFixedWidth(1018);
		widgetBar->setFixedWidth(1018);
		this->setFixedWidth(1020);		
    }

    imPanel = new QWidgetMsgPanel(m_memberService, ui.widget_message);
    imPanel->setNickName(m_authInfo.m_nickName.c_str());
    imPanel->setUserId(m_authInfo.m_userId.c_str());
    imPanel->setRoomOwner(m_authInfo.m_roomOwner.c_str());
    imPanel->setGroupId(m_authInfo.m_chatGroudID.c_str());
    QVBoxLayout *msgVBoxLayout = new QVBoxLayout(ui.widget_message);
    msgVBoxLayout->setMargin(0);
    msgVBoxLayout->addWidget(imPanel);
    ui.widget_message->setLayout(msgVBoxLayout);
    imPanel->show();
	
	if (m_authInfo.m_userType == STUDENT_USER_TYPE)
	{
		emit dispatch([this] {
			std::string home_screen = Context::instance().homeScreen();
			std::string id = Context::instance().roomOwner();
			std::string::size_type pos = home_screen.find("#");
			if (pos != std::string::npos)
			{
				std::string screen = home_screen.substr(0, pos);
				id = home_screen.substr(pos + 1);
				widgetSMain->setHomeScreen(screen, id);
			}
			else
			{
				widgetSMain->setHomeScreen(home_screen, id);
			}
			//widgetSMain->loadHomeScreen();
		});
	}

	ui.label_course_name->setText(Context::instance().roomName().c_str());
	ui.label_user_name->setText(Context::instance().roomOwner().c_str());

	VideoService::shared_instance()->openPlayer();
	m_memberService->queryMemberList();    // 刷新成员列表
}

void QWindowCourse::refreshMemberList()
{
	m_memberService->queryMemberList();
}

void QWindowCourse::modifyMemberInfo(const std::vector<ModifyMemberInfoItem>& infos)
{
	m_memberService->modifyMemberInfo(infos);
}

void QWindowCourse::onGetMemberList(int total, const std::vector<MemberItem>& memberList)
{
	if (m_authInfo.m_userType == STUDENT_USER_TYPE)
	{
		m_memberService->refreshVideoList(memberList);
	}
	emit dispatch([this, total, memberList] {
        if (memberPanel)
        {
            memberPanel->setMemberList(memberList);
        }

//视频列表不等于成员列表
        if (widgetTab)
        {
            widgetTab->setVideoInfo(m_memberService->videoList());
        }
		else
		{
			if (widgetSMain)
			{
				widgetSMain->setVideoInfo(m_memberService->videoList());
				widgetSMain->loadHomeScreen();
			}
		}
	});
}

void QWindowCourse::onModifyMemberInfo(int code, const char * desc)
{
}

void QWindowCourse::onInviteInteract(int code, const char * desc, const std::string& userId)
{
	//if (code)
	//{
	//	return;
	//}
	//MemberItem item;
	//int ret = m_memberService->queryMember(userId, item);
	//if (ret)
	//{
	//	return;
	//}

	//VideoHandsupItem newitem;
	//newitem.id = item.id;
	//newitem.nickName = item.nickName;
	//newitem.mic = item.mic;
	//newitem.camera = item.camera;
	//newitem.screen = item.screen;
	//newitem.status = STATUS_INVITING;
	//m_memberService->addVideo(newitem);
	//emit dispatch([this] {
	//	if (widgetTab)
	//	{
	//		widgetTab->setVideoInfo(m_memberService->videoList());
	//	}
	//});
}

void QWindowCourse::onApplyInteract(int code, const char * desc)
{
	emit dispatch([this, code, desc] {
		if (widgetBar)
		{
			widgetBar->onApplyInteract(code, desc);
		}
	});
}

void QWindowCourse::onGrandPermission(int code, const char *desc)
{

}

void QWindowCourse::onModifyConfInfo(int code, const char *desc)
{

}

void QWindowCourse::onRecvC2CTextMsg(const char * userID, const char * msg)
{
    if (imPanel)
    {
        imPanel->onRecvC2CTextMsg(userID, msg);
    }
}

void QWindowCourse::onRecvGroupTextMsg(const char * groupID, const char * userID, const char * msg)
{
    if (imPanel)
    {
        imPanel->onRecvGroupTextMsg(groupID, userID, msg);
    }
}

void QWindowCourse::onRecvC2CCustomMsg(const char * userID, const char * msg)
{

}

void QWindowCourse::onRecvGroupCustomMsg(const char * groupID, const char * userID, const char * msg)
{

}

void QWindowCourse::onRecvGroupSystemMsg(const char * groupID, const char * msg)
{
    // 逐个尝试解析通知类型

    OutputDebugStringA(msg);
    OutputDebugStringA("\n");

    MemberJoinNotify memberJoinNotify;
    if (memberJoinNotify.Parse(msg))
    {
        MemberItem item = memberJoinNotify.GetMemberItem();
        m_memberService->addMember(item);

        emit dispatch([this, item] {
            if (memberPanel)
            {
                memberPanel->addMember(item);
            }
			if (imPanel)
			{
				imPanel->addSysMsgContent(item.id.c_str(), QStringLiteral("加入了房间"));
			}
			/*视频列表不等于成员列表，而是等学生上台后再更新
            if (widgetTab)
            {
                widgetTab->setVideoInfo(m_memberService->videoList());
            }*/
        });
    }

    MemberQuitNotify memberQuitNotify;
    if (memberQuitNotify.Parse(msg))
    {
        std::string userId = memberQuitNotify.GetUserId();
        emit dispatch([this, userId] {
			if (imPanel)
			{
				imPanel->addSysMsgContent(userId.c_str(), QStringLiteral("离开了房间"));
			}
			m_memberService->removeMember(userId);
            if (memberPanel)
            {
                memberPanel->removeMember(userId);
            }
            if (widgetTab)
            {
                widgetTab->setVideoInfo(m_memberService->videoList());
            }
			else
			{
				if (widgetSMain)
				{
					widgetSMain->setVideoInfo(m_memberService->videoList());
				}
			}
        });
    }

    DestoyRoomNotify destroyRoomNotify;
    if (destroyRoomNotify.Parse(msg))
    {
        // todo
        // 房间销毁
    }

    ModifyMemberInfoNotify modifyMemberInfoNotify;
    if (modifyMemberInfoNotify.Parse(msg))
    {
        std::map<std::string, ModifyMemberInfoItem> modifyMemberItems = modifyMemberInfoNotify.GetMemberMoidfyList();

        emit dispatch([this, modifyMemberItems] {
			for (std::map<std::string, ModifyMemberInfoItem>::const_iterator it = modifyMemberItems.begin(); it != modifyMemberItems.end(); ++it)
			{
				if (it->second.status == 0)
				{
					if (imPanel)
					{
						imPanel->addSysMsgContent(it->second.id.c_str(), QStringLiteral("离开了房间"));
					}
					if (memberPanel)
					{
						memberPanel->removeMember(it->second.id);
					}

					if (it->first == Context::instance().userId())
					{
						abnormalExit();
					}
				}				
			}

            if (memberPanel)
            {
                memberPanel->modifyMember(modifyMemberItems);
            }

			bool modifyFlag = m_memberService->modifyMember(modifyMemberItems);
			if (modifyFlag)
			{
				if (m_authInfo.m_userType == STUDENT_USER_TYPE)
				{
					m_memberService->refreshVideoList();
				}
				else
				{
					m_memberService->refreshVideoList(modifyMemberItems);
				}
			}
			
            if (widgetTab && modifyFlag)
            {
                widgetTab->setVideoInfo(m_memberService->videoList());
            }
			if (widgetSMain)
			{
				widgetSMain->setVideoInfo(m_memberService->videoList());
			}
        });
    }

    InviteInteractNotify inviteInteractNotify;
    if (inviteInteractNotify.Parse(msg))
    {
        // 老师邀请发言
		emit dispatch([this] {
			onNotifyInviteInteract();
		});
    }

    ApplyInteractNotify applyInteractNotify;
    if (applyInteractNotify.Parse(msg))
    {
		std::string userId = applyInteractNotify.GetMember();
		MemberItem member;
		int ret = m_memberService->queryMember(userId, member);
		if (ret)
		{
			return;
		}
		ret = m_memberService->queryVideoMember(userId);
		if (!ret)
		{
			return;
		}
		m_memberService->addVideo(member);

		emit dispatch([this, member] {

			if (widgetTab)
			{
				widgetTab->setVideoInfo(m_memberService->videoList());
			}
		});
    }

    GrandPermissionNotify grandPermissionNotify;
    if (grandPermissionNotify.Parse(msg))
    {
		int permission = grandPermissionNotify.GetPermission();
        // 设置互动权限,需要切换线程
		emit dispatch([this, permission] {
			bool bEndInteract = true;
			if (permission == (1<<1|1<< 2))
			{
				acceptInteract();
			}
			else if (permission == 0)
			{
				int ret = m_memberService->queryVideoMember(Context::instance().userId());
				if (ret)//不存在，申请被驳回
				{
					bEndInteract = false;
				}
				exitInteract();				
			}
			else
			{
				modifyPermission(permission);
			}
			if (widgetBar)
			{
				widgetBar->onGrandPermission(permission, bEndInteract);
			}
		});		
    }

    ModifyRoomInfoNotify modifyRoomInfoNotify;
    if (modifyRoomInfoNotify.Parse(msg))
    {
        // 房间信息变更
		if (m_authInfo.m_userType == STUDENT_USER_TYPE)
		{
			std::string home_screen = modifyRoomInfoNotify.homeScreen();
			Context::instance().setHomeScreen(home_screen);
			emit dispatch([this] {
				std::string home_screen = Context::instance().homeScreen();
				std::string id = m_authInfo.m_userId;
				std::string::size_type pos = home_screen.find("#");
				if (pos != std::string::npos && widgetSMain)
				{
					std::string screen = home_screen.substr(0, pos);
					id = home_screen.substr(pos + 1);
					widgetSMain->setHomeScreen(screen, id);
				}
				else
				{
					widgetSMain->setHomeScreen(home_screen, id);
				}
				widgetSMain->loadHomeScreen();
			//VideoService::shared_instance()->openPlayer();
			});
		}
    }

	DestoyRoomNotify destoyRoomNotify;
	if (destoyRoomNotify.Parse(msg))
	{
		if (m_authInfo.m_userType == STUDENT_USER_TYPE)
		{
			emit dispatch([this] {
				QDialogMessage::exec(QString::fromWCharArray(L"老师已经结束了该课程"), QDialogMessage::OK);
				on_btn_close_clicked();
			});
		}
	}
}

void QWindowCourse::handle(txfunction func)
{
	func();
}

void QWindowCourse::on_modifyMemberInfo(ModifyMemberInfoItem info)
{
	std::vector<ModifyMemberInfoItem> vModifyItem;
	vModifyItem.push_back(info);
	m_memberService->modifyMemberInfo(vModifyItem);
}

void QWindowCourse::on_modifyConfInfo(std::string home_screen)
{
	m_memberService->modifyConfInfo(home_screen);
}

void QWindowCourse::on_grandPermission(std::string userID, int permission)
{
	m_memberService->grandPermission(userID, permission);
	if (!permission)
	{
		m_memberService->removeVideo(userID);
		if (widgetTab)
		{
			widgetTab->setVideoInfo(m_memberService->videoList());
		}
	}
}

void QWindowCourse::on_endInteract(const char* userID)
{
	m_memberService->grandPermission(userID, 0);
	//m_memberService->removeVideo(userID);
	//if (widgetTab)
	//{
	//	widgetTab->setVideoInfo(m_memberService->videoList());
	//}
}

void QWindowCourse::on_actInvite(std::string userID)
{
	m_memberService->inviteInteract(userID, 1<<1|1<<2);
}


void QWindowCourse::on_notifyToast(QString text)
{
	if (!m_toast)
	{
		delete m_toast;
		m_toast = nullptr;
	}
	m_toast = new QWidgetToast();
	m_toast->setDuration(1000);
	m_toast->setText(text);
}

void QWindowCourse::onNotifyInviteInteract()
{//通知学生互动
	QDialogMessage msgBox;
	msgBox.setTitle(QString::fromWCharArray(L"管理员邀请你发言"));
	msgBox.setOKText(QString::fromWCharArray(L"同意"));
	msgBox.setCancelText(QString::fromWCharArray(L"拒绝"));

	QDialog* dlg = static_cast<QDialog*>(&msgBox);
	if (dlg->exec() == QDialogMessage::OK)
	{//同意上台
		acceptInteract();
	}
}

void QWindowCourse::acceptInteract()
{
	if (VideoService::shared_instance()->getRole() != LiveGuest)
	{
		VideoService::shared_instance()->changeRole(LiveGuest);
	}
	else
	{
		VideoService::shared_instance()->openCamera();
		VideoService::shared_instance()->setMute(false);
	}
	
	ModifyMemberInfoItem modifyItem;
	modifyItem.id = m_authInfo.m_userId;
	modifyItem.mic = true;
	modifyItem.camera = true;
	modifyItem.screen = false;
	modifyItem.video = false;
	m_memberService->modifyMemberList(modifyItem.id, modifyItem);
	if (m_authInfo.m_userType == STUDENT_USER_TYPE)
	{
		m_memberService->refreshVideoList();
		if (widgetSMain)
		{
			widgetSMain->setVideoInfo(m_memberService->videoList());
		}
	}

	on_modifyMemberInfo(modifyItem);
}

void QWindowCourse::exitInteract()
{
	VideoService::shared_instance()->changeRole(Guest);
	ModifyMemberInfoItem modifyItem;
	modifyItem.id = m_authInfo.m_userId;
	modifyItem.mic = false;
	modifyItem.camera = false;
	modifyItem.screen = false;
	modifyItem.video = false;
	m_memberService->modifyMemberList(modifyItem.id, modifyItem);
	if (m_authInfo.m_userType == STUDENT_USER_TYPE)
	{
		m_memberService->refreshVideoList();
		if (widgetSMain)
		{
			widgetSMain->setVideoInfo(m_memberService->videoList());
		}
	}
	on_modifyMemberInfo(modifyItem);
}

void QWindowCourse::modifyPermission(int permisson)
{
	ModifyMemberInfoItem modifyItem;
	modifyItem.id = m_authInfo.m_userId;

	if (permisson == 1<<1) //关麦克风
	{
		modifyItem.mic = false;
		modifyItem.camera = true;
		VideoService::shared_instance()->openCamera();
		VideoService::shared_instance()->setMute(true);
	}
	else if (permisson == 1 << 2) //关摄像头
	{
		modifyItem.mic = true;
		modifyItem.camera = false;
		VideoService::shared_instance()->closeCamera();
		VideoService::shared_instance()->setMute(false);
	}
	modifyItem.screen = false;
	modifyItem.video = false;
	m_memberService->modifyMemberList(modifyItem.id, modifyItem);

	if (m_authInfo.m_userType == STUDENT_USER_TYPE)
	{
		m_memberService->refreshVideoList();
		if (widgetSMain)
		{
			widgetSMain->setVideoInfo(m_memberService->videoList());
		}
	}

	on_modifyMemberInfo(modifyItem);
}

void QWindowCourse::abnormalExit()
{
	QDialogMessage::exec(QString::fromWCharArray(L"网络中断退出课堂"), QDialogMessage::OK);
	m_accountInfo->exitRoom();
	hide();
}

void QWindowCourse::on_btn_close_clicked()
{
	QDialogMessage msgBox;
	msgBox.setTitle(QString::fromWCharArray(L"确定要退出课堂？"));

	QDialog* dlg = static_cast<QDialog*>(&msgBox);
	if (dlg->exec() == QDialogMessage::OK)
	{
		m_accountInfo->exitRoom();
		hide();
	}
	//QApplication::exit(0);
}

void QWindowCourse::mousePressEvent(QMouseEvent *e)
{
	mousePressedPosition = e->globalPos();
	windowPositionAsDrag = pos();
}

void QWindowCourse::mouseReleaseEvent(QMouseEvent *e)
{
	Q_UNUSED(e)
		// 鼠标放开始设置鼠标按下的位置为 null，表示鼠标没有被按下
		mousePressedPosition = QPoint();
}

void QWindowCourse::mouseMoveEvent(QMouseEvent *e)
{
	if (!mousePressedPosition.isNull()) {
		// 鼠标按下并且移动时，移动窗口, 相对于鼠标按下时的位置计算，是为了防止误差累积
		QPoint delta = e->globalPos() - mousePressedPosition;
		move(windowPositionAsDrag + delta);
		emit mainWindowMoved(windowPositionAsDrag + delta);
	}
}

void QWindowCourse::closeEvent(QCloseEvent *event) 
{
	m_accountInfo->exitRoom();
}
