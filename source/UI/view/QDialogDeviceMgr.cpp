#include "QDialogDeviceMgr.h"
#include "QDialogMessage.h"
#include "VideoService.h"
#include "Context.h"
#include <QMouseEvent>

#define FromStdStr(str) QString::fromStdString(str)
#define FromBits(str) QString::fromLocal8Bit(str)

QDialogDeviceMgr::QDialogDeviceMgr(QWidget *parent)
	: QDialog(parent)
	, m_pressed(false)
	, m_point()
	, m_pTimer(nullptr)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | windowFlags());
	//setAttribute(Qt::WA_TranslucentBackground);
	m_pTimer = new QTimer();
	m_pTimer->setInterval(500);
	connect(m_pTimer, &QTimer::timeout, this, &QDialogDeviceMgr::onTimeOut);
	//connect(ui.checkBox_camera, &QCheckBox::clicked, this, &QDialogDeviceMgr::on_checkBox_camera_toggled);
	//connect(ui.checkBox_mic, &QCheckBox::clicked, this, &QDialogDeviceMgr::on_checkBox_mic_toggled);
}

QDialogDeviceMgr::~QDialogDeviceMgr()
{
	SafeDelete(m_pTimer)
}

void QDialogDeviceMgr::initValue()
{
	VideoService::shared_instance()->setRenders(ui.widget_render, VIDEO_RENDER_MAIN);
	VideoService::shared_instance()->setMainRenderID(Context::instance().userId());
	GetILive()->startDeviceTest(OnStartDeviceTestSuc, OnStartDeviceTestErr, this);
	//bool bOpen = VideoService::shared_instance()->getMicOpen();
	//if (!bOpen)
	//{
	//	ui.checkBox_mic->setChecked(false);
	//}
}

void QDialogDeviceMgr::initUI()
{
	VideoService::shared_instance()->getCameraList(m_cameraList);
	if (m_cameraList.empty())
	{
		GetILive()->getCameraList(m_cameraList);
	}
	int nRet;
	VideoService::shared_instance()->getMicList(m_micList);
	if (m_micList.empty())
	{
		nRet = GetILive()->getMicList(m_micList);
	}

	for (int i = 0; i < m_cameraList.size(); i++)
	{
		ui.cb_camera->addItem(FromBits(m_cameraList[i].second.c_str()));
	}
	for (int i = 0; i < m_micList.size(); i++)
	{
		ui.cb_mic->addItem(FromBits(m_micList[i].second.c_str()));
	}

	if (!m_cameraList.empty())
	{
		m_cameraID = m_cameraList[0].first.c_str();
		GetILive()->openCamera(m_cameraList[0].first.c_str());
	}
	else
	{
		ui.checkBox_camera->setChecked(false);
	}
	if (!m_micList.empty())
	{
		m_micID = m_micList[0].first.c_str();
		GetILive()->openMic(m_micList[0].first.c_str());
		m_pTimer->start();
	}
	else
	{
		ui.checkBox_mic->setChecked(false);
	}

	int value = GetILive()->getMicVolume();
	ui.hsVolume->blockSignals(true);
	ui.hsVolume->setValue(value);
	ui.hsVolume->blockSignals(false);
	VideoService::shared_instance()->openPlayer();
}

void QDialogDeviceMgr::updateDynamicVolume()
{
	int volume = GetILive()->getMicDynamicVolume();
	ui.volumeBar->blockSignals(true);
	ui.volumeBar->setValue(volume);
	ui.volumeBar->blockSignals(false);
}

void QDialogDeviceMgr::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_pressed = true;
		m_point = event->pos();
	}
}

void QDialogDeviceMgr::mouseMoveEvent(QMouseEvent *event)
{
	if (m_pressed)
	{
		move(event->pos() - m_point + pos());
	}
}

void QDialogDeviceMgr::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);

	m_pressed = false;
}

void QDialogDeviceMgr::OnStartDeviceTestSuc(void* data)
{
	QDialogDeviceMgr* pMgr = (QDialogDeviceMgr*)data;
	if (pMgr)
	{
		pMgr->initUI();
	}
}

void QDialogDeviceMgr::OnStartDeviceTestErr(int code, const char *desc, void* data)
{

}

void QDialogDeviceMgr::OnStopDeviceTestSuc(void* data)
{

}

void QDialogDeviceMgr::OnStopDeviceTestErr(int code, const char *desc, void* data)
{

}

void QDialogDeviceMgr::on_btn_ok_clicked()
{
	VideoService::shared_instance()->setCamera(m_cameraID);
	VideoService::shared_instance()->setMic(m_micID);
	VideoService::shared_instance()->setRenders(nullptr, VIDEO_RENDER_MAIN);
	GetILive()->stopDeviceTest(OnStopDeviceTestSuc, OnStopDeviceTestErr, this);

	m_pTimer->stop();
	accept();
}

void QDialogDeviceMgr::on_btn_cancel_clicked()
{
	VideoService::shared_instance()->setRenders(nullptr, VIDEO_RENDER_MAIN);
	GetILive()->stopDeviceTest(OnStopDeviceTestSuc, OnStopDeviceTestErr, this);
	m_pTimer->stop();
	reject();
}

void QDialogDeviceMgr::on_btn_close_clicked()
{
	GetILive()->stopDeviceTest(OnStopDeviceTestSuc, OnStopDeviceTestErr, this);
	m_pTimer->stop();
	reject();
}

void QDialogDeviceMgr::on_hsVolume_valueChanged(int value)
{
	if (value >= 0)
	{
		GetILive()->setMicVolume(value);
	}	
}

void QDialogDeviceMgr::on_checkBox_mic_toggled(bool checked)
{
	if (checked)
	{
		if (!m_micID.empty())
		{
			GetILive()->openMic(m_micID.c_str());
			m_pTimer->start();
		}
	}
	else
	{
		GetILive()->closeMic();
		m_pTimer->stop();
	}

	//VideoService::shared_instance()->setMicOpen(checked);
}

void QDialogDeviceMgr::on_checkBox_camera_toggled(bool checked)
{
	if (checked)
	{
		if (!m_cameraID.empty())
		{
			GetILive()->openCamera(m_cameraID.c_str());
		}
	}
	else
	{
		GetILive()->closeCamera();
	}
	//VideoService::shared_instance()->setCameraView(checked);
}

void QDialogDeviceMgr::on_cb_mic_currentIndexChanged(int index)
{
	if (m_micList.size() > index)
	{
		m_micID = m_micList[index].first.c_str();
		//GetILive()->closeMic();
		//GetILive()->openMic(m_micID.c_str());
	}
}

void QDialogDeviceMgr::on_cb_camera_currentIndexChanged(int index)
{
	if (m_cameraList.size() > index)
	{
		m_cameraID = m_cameraList[index].first.c_str();
		//GetILive()->closeCamera();
		//GetILive()->openCamera(m_cameraID.c_str());
	}
}

void QDialogDeviceMgr::onTimeOut()
{
	updateDynamicVolume();
}
 