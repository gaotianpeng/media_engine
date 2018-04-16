#pragma once

#include <QWidget>
#include <QTimer>
#include "commonType.h"
#include "ui_QWidgetMediaPlayer.h"
#include "VideoService.h"

class QWidgetMediaPlayer : public QWidget, public MediaPlayCallback
{
	enum E_PlayState
	{
		E_Stop,
		E_Playing,
		E_Pause,
	};

	Q_OBJECT
public:
	QWidgetMediaPlayer(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	~QWidgetMediaPlayer();

	void openMediaFile(const std::string& szFileName);
	void setUserType(UserType userType);
signals:
	void modifyMemberInfo(ModifyMemberInfoItem info);
private slots:
	void on_btnPlay_clicked();
	void on_btnOpen_clicked();
	void on_hsProgress_valueChanged(int value);
	void on_hsVolume_valueChanged(int value);
	void on_playTimer();

private:
	void _play();
	void _resume();
	void _pause();
	void _stop();

	void _updateBtnPlayState();
	void _updateTime();
	void _updateVolume();

	void _startPlayTimer();
	void _stopPlayTimer();

	QString _getTimeStrBySeconds(const int64 sec);

	void onOpenMediaPlay() override;
	void onCloseMediaPlay() override;
	void onOpenPlayer() override;
	void onClosePlayer() override;

private:
	Ui::MediaPlayer ui;
	bool		m_bReopen;
	std::string m_szMediaFile;
	E_PlayState m_eState;
	int64		m_n64Pos;
	int64		m_n64MaxPos;
	uint32		m_uVolume;
	QTimer*		m_playTimer;
	UserType	m_userType = TEACHER_USER_TYPE;
};
