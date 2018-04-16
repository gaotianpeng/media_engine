#include "QWidgetTab.h"
#include "QDialogMessage.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include "QWidgetVideo.h"
#include "QDialogDeviceMgr.h"
#include "Context.h"

QWidget * mainTabWindow = nullptr;
extern QWidget * mainWindow;

QWidgetTab::QWidgetTab(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	mainTabWindow = this;
/*
	//增加设备管理和开始授课按钮
	QWidget* widget_tab_corner = new QWidget(this);
	QPushButton * btn_device_manage = new QPushButton(QStringLiteral("设备管理"), widget_tab_corner);
	QPushButton * btn_teaching = new QPushButton(QStringLiteral("开始授课"), widget_tab_corner);
	QString deviceStyle = 
R"(
QPushButton {
	border: 1px solid #dddddd;
	background:  #ffffff;
	color: #000000;
	font: 9pt "Microsoft YaHei";
}

QPushButton:hover {
	border: 1px solid #d2d2d2;
	background:  #f2f2f2;
}

QPushButton:pressed {
	border: 1px solid #d2d2d2;
	background:  #e5e5e5;
}

QPushButton:disable {
	border: 1px solid #d5d5d5;
	color: #bbbbbb;
	background:  #f2f2f2;
}
)";

	QString teachingStyle = 
R"(
QPushButton {
	border: none;
	background:  #006eff;
	color: #ffffff;
	font: 10pt "Microsoft YaHei";
}

QPushButton:hover {
	background:  #0068f2;
}

QPushButton:pressed {
	background:  #0062e5;
}

QPushButton:disable {
	border: 1px solid #d5d5d5;
	color: #bbbbbb;
	background:  #f2f2f2;
}
)";

	btn_device_manage->setStyleSheet(deviceStyle);
	btn_teaching->setStyleSheet(teachingStyle);
	const QSize btnSize = QSize(76, 30);
	btn_device_manage->setFixedSize(btnSize);
	btn_teaching->setFixedSize(btnSize);

	const QSize widgetSize = QSize(162, 60);
	widget_tab_corner->setFixedSize(widgetSize);

	connect(btn_device_manage, SIGNAL(released()), this, SLOT(on_btn_device_manage_clicked()));
	connect(btn_teaching, SIGNAL(released()), this, SLOT(on_btn_teaching_clicked()));

	QHBoxLayout* hLayout = new QHBoxLayout(widget_tab_corner);
	hLayout->addWidget(btn_device_manage);
	hLayout->addWidget(btn_teaching);
	hLayout->setContentsMargins(QMargins(0,15,0,15));
	hLayout->setSpacing(10);
	ui.tabWidget->setCornerWidget(widget_tab_corner, Qt::TopRightCorner);
	widget_tab_corner->show();
	*/
	//增加摄像头面板
	cameraWidget = new QWidgetVideo;
	cameraWidget->setUserType(TEACHER_USER_TYPE);
	QVBoxLayout * vCameraLayout = new QVBoxLayout(ui.widget_camera);
	vCameraLayout->setMargin(0);
	vCameraLayout->addWidget(cameraWidget);

	//增加白板面板
	boardWidget = new QWidgetVideo;
	boardWidget->setUserType(TEACHER_USER_TYPE);
	QVBoxLayout * vBoardLayout = new QVBoxLayout(ui.widget_board);
	vBoardLayout->setMargin(0);
	vBoardLayout->addWidget(boardWidget);

	//增加屏幕分享
	screenWidget = new QWidgetVideo;
	screenWidget->setUserType(TEACHER_USER_TYPE);
	QVBoxLayout * vScreenLayout = new QVBoxLayout(ui.page_screen_content);
	vScreenLayout->setMargin(0);
	vScreenLayout->addWidget(screenWidget);
	screenWidget->show();

	//增加视频分享
	mediaWidget = new QWidgetVideo;
	mediaWidget->setUserType(TEACHER_USER_TYPE);
	QVBoxLayout * vMediaLayout = new QVBoxLayout(ui.page_media_content);
	vMediaLayout->setMargin(0);
	vMediaLayout->addWidget(mediaWidget);	
	mediaWidget->show();

	on_tabWidget_currentChanged();
	m_screenChoose = nullptr;
	m_sceenTool = nullptr;
	m_shareRect = nullptr;
	m_toolBar = nullptr;
	m_bScreen = false;
	m_bVideo = false;
	connect(this, SIGNAL(modifyConfInfo(std::string)), mainWindow, SLOT(on_modifyConfInfo(std::string)));
}

QWidgetTab::~QWidgetTab()
{
	SafeDelete(m_screenChoose)
	SafeDelete(m_sceenTool)
	SafeDelete(m_shareRect)
	SafeDelete(m_toolBar)
}

void QWidgetTab::init(const AuthInfo & info)
{
	m_authInfo = info;
	cameraWidget->setId(m_authInfo.m_userId.c_str(), m_authInfo.m_nickName.c_str());
	boardWidget->setId(m_authInfo.m_userId.c_str(), m_authInfo.m_nickName.c_str());
	mediaWidget->setId(m_authInfo.m_userId.c_str(), m_authInfo.m_nickName.c_str());
	cameraWidget->initVideoInfo();
}

void QWidgetTab::setVideoInfo(const std::vector<VideoHandsupItem>& vVideoUserInfo)
{
	cameraWidget->setVideoInfo(vVideoUserInfo);
	boardWidget->setVideoInfo(vVideoUserInfo, false);
	mediaWidget->setVideoInfo(vVideoUserInfo, false);
}



void QWidgetTab::EndShare()
{
	VideoService::shared_instance()->stopScreenPreView();
	m_bScreen = false;
	ui.stacked_screen->setCurrentIndex(0);
	if (m_sceenTool)
	{
		m_sceenTool->hide();
	}
	if (m_shareRect)
	{
		delete m_shareRect;
		m_shareRect = nullptr;
	}
}

void QWidgetTab::on_btn_device_manage_clicked()
{
	QDialogDeviceMgr mgr;
	mgr.initValue();
	mgr.exec();
}

void QWidgetTab::on_btn_teaching_clicked()
{
}

void QWidgetTab::on_btn_full_share_clicked()
{
	//QT最小化后再恢复界面刷新会失效
	//mainWindow->setWindowState(Qt::WindowMinimized);
	if (m_bVideo)
	{
		mediaWidget->openMediaFile("");
		ui.stacked_media->setCurrentIndex(0);
		m_bVideo = false;
	}
	HWND hwndDesktop = ::GetDesktopWindow();
	RECT rc;
	::GetWindowRect(hwndDesktop, &rc);
	screenWidget->openScreenShareArea(rc.left, rc.top, rc.right, rc.bottom);
	m_bScreen = true;
	ui.stacked_screen->setCurrentIndex(1);
	if (!m_sceenTool)
	{
		m_sceenTool = new QWindowScreenTool();
	}
	m_sceenTool->initPosition();
	m_sceenTool->show();
//	ui.tab_screen->hide();

}

void QWidgetTab::on_btn_area_share_clicked()
{
	//mainWindow->setWindowState(Qt::WindowMinimized);
	if (!m_screenChoose)
	{
		m_screenChoose = new QWindowScreenShareArea();
	}
	m_screenChoose->init();
	m_screenChoose->show();
	//ui.stacked_screen->setCurrentIndex(1);
}

void QWidgetTab::on_btn_media_share_clicked()
{
	QString szMediaPath = QFileDialog::getOpenFileName(this, QStringLiteral("请选择播放的视频文件"), "", QStringLiteral("视频文件(*.avi *.mp4)"));
	if (!szMediaPath.isEmpty())
	{
		m_bVideo = true;
		mediaWidget->openMediaFile(szMediaPath.toStdString());
		ui.stacked_media->setCurrentIndex(1);
	}
}

void QWidgetTab::on_tabWidget_currentChanged()
{
	int index = ui.tabWidget->currentIndex();
	switch (index)
	{
	case 0:
	{
		cameraWidget->setRenders();
		cameraWidget->setMainRender(MAIN_RENDER_CAMERA);
		std::string home_screen = "camera#";
		home_screen += Context::instance().userId();
		emit modifyConfInfo(home_screen);
	}
		break;
	case 1:
	{
		boardWidget->setRenders();
		boardWidget->setMainRender(MAIN_RENDER_BOARD);
		std::string home_screen = "whiteboard";
		//home_screen += Context::instance().userId();
		emit modifyConfInfo(home_screen);
	}
		break;
	case 2:
	{
		if (m_bVideo)
		{
			mediaWidget->openMediaFile("");
			ui.stacked_media->setCurrentIndex(0);
			m_bVideo = false;
		}
		screenWidget->setRenders();
		screenWidget->setMainRender(MAIN_RENDER_SCREEN);
		std::string home_screen = "screen#";
		home_screen += Context::instance().userId();
		emit modifyConfInfo(home_screen);
	}
		break;
	case 3:
	{
		if (m_bScreen)
		{
			//QDialogMessage msgBox;
			//msgBox.setTitle(QString::fromWCharArray(L"播放视频前是否要关掉屏幕分享"));
			//QDialog* dlg = static_cast<QDialog*>(&msgBox);
			//if (dlg->exec() == QDialogMessage::OK)
			//{//同意上台
			//	acceptInteract();
			//}
			EndShare();
		}
		mediaWidget->setRenders();
		mediaWidget->setMainRender(MAIN_RENDER_MEDIA);
		std::string home_screen = "video#";
		home_screen += Context::instance().userId();
		emit modifyConfInfo(home_screen);
	}
		break;
	default:
		break;
	}
}

void QWidgetTab::on_endShare()
{
	//ui.tab_screen->show();
	//mainWindow->setWindowState(Qt::WindowActive);
	m_bScreen = false;
	screenWidget->closeScreenShareArea();
	ui.stacked_screen->setCurrentIndex(0);
	if (m_sceenTool)
	{
		m_sceenTool->hide();
	}
	if (m_shareRect)
	{
		delete m_shareRect;
		m_shareRect = nullptr;
	}
}

void QWidgetTab::on_beginShare()
{
	ui.stacked_screen->setCurrentIndex(1);
	if (!m_sceenTool)
	{
		m_sceenTool = new QWindowScreenTool();
	}
	m_sceenTool->initPosition();
	m_sceenTool->show();
}

void QWidgetTab::on_choseShare(int x, int y, int w, int h)
{
	if (!m_shareRect)
	{
		m_shareRect = new QWindowShareRect();		
	}
	if (!m_toolBar)
	{
		m_toolBar = new QWindowShareToolBar();
	}

	m_shareRect->init(x, y, w, h, m_toolBar);
	m_shareRect->show();	
}

void QWidgetTab::on_cancelShare()
{
	//mainWindow->setWindowState(Qt::WindowActive);
	ui.stacked_screen->setCurrentIndex(0);
	if (m_screenChoose)
	{
		delete m_screenChoose;
		m_screenChoose = nullptr;
	}
	if (m_sceenTool)
	{
		m_sceenTool->hide();
	}
	if (m_shareRect)
	{
		delete m_shareRect;
		m_shareRect = nullptr;
	}
}

void QWidgetTab::on_startShareRect()
{
	//mainWindow->setWindowState(Qt::WindowActive);
	if (m_shareRect)
	{
		//if (m_bVideo)
		//{
		//	mediaWidget->openMediaFile("");
		//	ui.stacked_media->setCurrentIndex(0);
		//	m_bVideo = false;
		//}
		int left, right, top, bottom;
		m_shareRect->getShareRect(left,top,right,bottom);
		screenWidget->openScreenShareArea(left, top, right, bottom);
		ui.stacked_screen->setCurrentIndex(1);
		if (!m_sceenTool)
		{
			m_sceenTool = new QWindowScreenTool();
		}
		m_sceenTool->initPosition();
		m_sceenTool->show();
		m_bScreen = true;
	}
}

void QWidgetTab::on_changeShareRect()
{
	if (m_shareRect)
	{
		int left, right, top, bottom;
		m_shareRect->getShareRect(left, top, right, bottom);
		screenWidget->openScreenShareArea(left, top, right, bottom);
//		ui.stacked_screen->setCurrentIndex(1);
	}
}
