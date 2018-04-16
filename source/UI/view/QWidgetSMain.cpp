#include "QWidgetSMain.h"
#include "QDialogDeviceMgr.h"
#include "VideoService.h"

QWidget * mainSWindow = nullptr;
QWidgetSMain::QWidgetSMain(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mainSWindow = this;
	initUI();
}

QWidgetSMain::~QWidgetSMain()
{
}

void QWidgetSMain::setVideoInfo(const std::vector<VideoHandsupItem> & vVideoUserInfo)
{
	if (videoWidget)
	{
		videoWidget->setVideoInfo(vVideoUserInfo);
	}	
}

void QWidgetSMain::setHomeScreen(std::string& screen, std::string& id)
{
	m_screen = screen;
	if (screen == "whiteboard")
	{
		ui.current_source->setText(QStringLiteral("当前画面：白板"));
	}
	else
	{
		VideoService::shared_instance()->updateHomeScreen(screen, id);
		if (screen == "camera")
		{
			ui.current_source->setText(QStringLiteral("当前画面：摄像头"));
			videoWidget->setMainRender(MAIN_RENDER_CAMERA);
		}
		else if (screen == "screen")
		{
			ui.current_source->setText(QStringLiteral("当前画面：屏幕分享"));
			videoWidget->setMainRender(MAIN_RENDER_SCREEN);
		}
		else if (screen == "video")
		{
			ui.current_source->setText(QStringLiteral("当前画面：播放视频"));
			videoWidget->setMainRender(MAIN_RENDER_MEDIA);
		}
	}
}

void QWidgetSMain::loadHomeScreen()
{
	if (m_screen == "whiteboard")
	{
		videoWidget->setMainRender(MAIN_RENDER_BOARD);	
	}
}

void QWidgetSMain::initUI()
{
	videoWidget = new QWidgetVideo(ui.widget_main);
	videoWidget->setUserType(STUDENT_USER_TYPE);

	vVideoLayout = new QVBoxLayout(ui.widget_main);
	vVideoLayout->setMargin(0);
	vVideoLayout->addWidget(videoWidget);
	//ui.widget_main->setLayout(vVideoLayout);

	//standbyWidget = new QWidget(ui.widget_main);
	//vStandbyLayout = new QVBoxLayout(ui.widget_main);
	//vStandbyLayout->setMargin(0);
	//vStandbyLayout->addWidget(standbyWidget);
	//ui.widget_main->setLayout(vStandbyLayout);

	//hideComponent();
	//standbyWidget->hide();
	ui.btn_device_manage->hide();
}

void QWidgetSMain::hideComponent()
{
	videoWidget->hide();
	standbyWidget->hide();
}

void QWidgetSMain::on_btn_device_manage_clicked()
{
	QDialogDeviceMgr mgr;
	mgr.initValue();
	mgr.exec();
}

void QWidgetSMain::on_fullScreen()
{
	videoWidget->fullScreen();
}

void QWidgetSMain::on_sub_state_changed(int index)
{
	hideComponent();
	switch (index)
	{
	case 0:
	{
		ui.widget_main->setLayout(vStandbyLayout);
		standbyWidget->show();
	}
	break;
	case 1:
	case 2:
	case 3:
	case 4:
	{
		ui.widget_main->setLayout(vVideoLayout);
		videoWidget->show();
		videoWidget->setMainRender((MainRenderType)index);
	}
	break;
	default:
		break;
	}
}