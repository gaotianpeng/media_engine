#pragma once

#include <QWidget>
#include "ui_QWidgetSMain.h"
#include "QWidgetVideo.h"


class QWidgetSMain : public QWidget
{
	Q_OBJECT

public:
	QWidgetSMain(QWidget *parent = Q_NULLPTR);
	~QWidgetSMain();
	void setVideoInfo(const std::vector<VideoHandsupItem> & vVideoUserInfo);
	void setHomeScreen(std::string& screen, std::string& id);
	void loadHomeScreen();

private:
	Ui::QWidgetSMain ui;
	QWidgetVideo * videoWidget;
	QWidget * standbyWidget;

	QVBoxLayout * vVideoLayout;
	QVBoxLayout * vStandbyLayout;
	std::string m_screen;

private:
	void initUI();
	void hideComponent();

private slots:
// index£º  0£ºÎ´¿ªÊ¼£»1£ºÉãÏñÍ· 2£º°×°å 3£ºÆÁÄ» 4£ºÊÓÆµ
	void on_sub_state_changed(int index);
	void on_btn_device_manage_clicked();
	void on_fullScreen();
};
