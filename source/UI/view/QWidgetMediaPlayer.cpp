#include "QWidgetMediaPlayer.h"
#include "VideoService.h"
#include "Context.h"
#include <QFileDialog>

static const char* szBtnPlayStyle = "QPushButton{ border-image: url(:/Course/course/play.png);}"
									"QPushButtont:hover{ border-image: url(:/Course/course/play.png);}"
									"QPushButton:pressed{ border-image: url(:/Course/course/play_press.png);}"
									"QPushButton:disabled{ border-image: url(:/Course/course/play.png);}";

static const char* szBtnPauseStyle = "QPushButton{ border-image: url(:/Course/course/pause.png);}"
									"QPushButtont:hover{ border-image: url(:/Course/course/pause.png);}"
									"QPushButton:pressed{ border-image: url(:/Course/course/pause_press.png);}"
									"QPushButton:disabled{ border-image: url(:/Course/course/pause.png);}";
extern QWidget * mainWindow;
QWidgetMediaPlayer::QWidgetMediaPlayer( QWidget* parent /*= Q_NULLPTR*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
	: QWidget(parent, f)
	, m_bReopen(false)
	, m_eState(E_Stop)
	, m_n64Pos(0)
	, m_n64MaxPos(0)
	, m_uVolume(0)
	, m_playTimer(NULL)
{
	ui.setupUi(this);

	m_playTimer = new QTimer(this);
	connect(m_playTimer, SIGNAL(timeout()), this, SLOT(on_playTimer()));

 	VideoService::shared_instance()->setMediaRender(ui.widRender);
 	VideoService::shared_instance()->setMediaPlayCallback(this);
	connect(this, SIGNAL(modifyMemberInfo(ModifyMemberInfoItem)), mainWindow, SLOT(on_modifyMemberInfo(ModifyMemberInfoItem)));
}

QWidgetMediaPlayer::~QWidgetMediaPlayer()
{

}

void QWidgetMediaPlayer::openMediaFile(const std::string& szFileName)
{
	m_szMediaFile = szFileName;
	if (m_eState != E_Stop)
	{
		if (!szFileName.empty())
		{
			m_bReopen = true;
		}
		else
			m_bReopen = false;
		
		_stop();
	}
	else
	{
		on_btnPlay_clicked();
	}
}

void QWidgetMediaPlayer::setUserType(UserType userType)
{
	if (userType == STUDENT_USER_TYPE)
	{
		ui.widControl->hide();
	}
}

void QWidgetMediaPlayer::on_btnPlay_clicked()
{
	if (m_eState == E_Stop)
	{
		_play();
	}
	else if(m_eState == E_Playing)
	{
		_pause();
	}
	else if (m_eState == E_Pause)
	{
		_resume();
	}
}

void QWidgetMediaPlayer::on_btnOpen_clicked()
{
	QString szMediaPath = QFileDialog::getOpenFileName(this, QStringLiteral("请选择播放的视频文件"), "", QStringLiteral("视频文件(*.avi *.mp4)"));
	if (!szMediaPath.isEmpty())
	{
		openMediaFile(szMediaPath.toStdString());
	}
}

void QWidgetMediaPlayer::on_hsProgress_valueChanged(int value)
{
	GetILive()->setPlayMediaFilePos(value);
}

void QWidgetMediaPlayer::on_hsVolume_valueChanged(int value)
{
	GetILive()->setPlayerVolume(value);
}

void QWidgetMediaPlayer::on_playTimer()
{
	GetILive()->getPlayMediaFilePos(m_n64Pos, m_n64MaxPos);
	if (m_n64Pos >= m_n64MaxPos)
	{
		VideoService::shared_instance()->mediaRestart();
		m_n64Pos = 0;
	}
	_updateTime();

}

void QWidgetMediaPlayer::_play()
{
	VideoService::shared_instance()->mediaPlay(m_szMediaFile.c_str());

	if ( !GetILive()->getCurPlayerState() )
	{
		VideoService::shared_instance()->openPlayer();
	}
	ui.widRender->beginRender();
	ModifyMemberInfoItem modifyItem;
	modifyItem.id = Context::instance().userId();
	modifyItem.mic = true;
	modifyItem.camera = true;
	modifyItem.screen = false;
	modifyItem.video = true;

	emit modifyMemberInfo(modifyItem);
}

void QWidgetMediaPlayer::_resume()
{
	int nRet= VideoService::shared_instance()->mediaResume();
	if (nRet == 0)
	{
		m_eState = E_Playing;
		_updateBtnPlayState();
	}
}

void QWidgetMediaPlayer::_pause()
{
	int nRet = VideoService::shared_instance()->mediaPause();
	if (nRet == 0)
	{
		m_eState = E_Pause;
		_updateBtnPlayState();
	}
}

void QWidgetMediaPlayer::_stop()
{
	VideoService::shared_instance()->mediaStop();
	ui.widRender->stopRender();

	if (!m_szMediaFile.empty())
	{
		ModifyMemberInfoItem modifyItem;
		modifyItem.id = Context::instance().userId();
		modifyItem.mic = true;
		modifyItem.camera = true;
		modifyItem.screen = false;
		modifyItem.video = false;
		emit modifyMemberInfo(modifyItem);
	}
}

void QWidgetMediaPlayer::_updateBtnPlayState()
{
	switch (m_eState)
	{
		case E_Stop:
		{
			ui.btnPlay->setStyleSheet(szBtnPlayStyle);
			break;
		}
		case E_Playing:
		{
			ui.btnPlay->setStyleSheet(szBtnPauseStyle);
			break;
		}
		case E_Pause:
		{
			ui.btnPlay->setStyleSheet(szBtnPlayStyle);
			break;
		}
		default:
		{
			break;
		}
	}
}

void QWidgetMediaPlayer::_updateTime()
{
	ui.hsProgress->blockSignals(true);
	ui.lbCurTime->setText(_getTimeStrBySeconds(m_n64Pos));
	ui.lbTotalTime->setText(_getTimeStrBySeconds(m_n64MaxPos));
	ui.hsProgress->setMinimum(0);
	ui.hsProgress->setMaximum(m_n64MaxPos);
	ui.hsProgress->setValue(m_n64Pos);
	int nStep = m_n64MaxPos / 40;
	nStep = nStep < 1 ? 1 : nStep;
	ui.hsProgress->setSingleStep(nStep);
	ui.hsProgress->setPageStep(nStep);
	ui.hsProgress->blockSignals(false);
}

void QWidgetMediaPlayer::_updateVolume()
{
	m_uVolume = GetILive()->getPlayerVolume();
	ui.hsVolume->blockSignals(true);
	ui.hsVolume->setValue(m_uVolume);
	ui.hsVolume->blockSignals(false);
}

void QWidgetMediaPlayer::_startPlayTimer()
{
	m_playTimer->start(1000);
}

void QWidgetMediaPlayer::_stopPlayTimer()
{
	m_playTimer->stop();
}

QString QWidgetMediaPlayer::_getTimeStrBySeconds(const int64 sec)
{
	int hours = sec / 3600;
	int minutes = (sec % 3600) / 60;
	int secs = sec % 60;
	char res[16] = { 0 };
	sprintf_s(res, 16, "%02d:%02d:%02d", hours, minutes, secs);
	return res;
}

void QWidgetMediaPlayer::onOpenMediaPlay()
{
	m_eState = E_Playing;
	_updateBtnPlayState();
	_updateTime();
	_startPlayTimer();
}

void QWidgetMediaPlayer::onCloseMediaPlay()
{
	m_eState = E_Stop;
	_updateBtnPlayState();
	_updateTime();
	_stopPlayTimer();

	if (m_bReopen)
	{
		on_btnPlay_clicked();
	}
}

void QWidgetMediaPlayer::onOpenPlayer()
{
	_updateVolume();
}

void QWidgetMediaPlayer::onClosePlayer()
{
	_updateVolume();
}

