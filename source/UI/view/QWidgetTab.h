#pragma once

#include <QWidget>
#include "ui_QWidgetTab.h"
#include "QWidgetVideo.h"
#include "QWindowScreenTool.h"
#include "QWindowScreenShareArea.h"
#include "QWindowShareRect.h"

class QWidgetTab : public QWidget
{
	Q_OBJECT

public:
	QWidgetTab(QWidget *parent = Q_NULLPTR);
	~QWidgetTab();
	void init(const AuthInfo& info);
	void setVideoInfo(const std::vector<VideoHandsupItem> & vVideoUserInfo);

private:
	Ui::QWidgetTab ui;
	QWidgetVideo * cameraWidget;
	QWidgetVideo * boardWidget;
	QWidgetVideo * screenWidget;
	QWidgetVideo * mediaWidget;
	AuthInfo m_authInfo;
	QWindowScreenShareArea *m_screenChoose;
	QWindowShareRect *m_shareRect;
	QWindowScreenTool *m_sceenTool;
	QWindowShareToolBar *m_toolBar;
	bool   m_bScreen;
	bool   m_bVideo;

	void EndShare();
signals:
	void modifyConfInfo(std::string home_screen);

private slots:
	void on_btn_device_manage_clicked();
	void on_btn_teaching_clicked();
	void on_btn_full_share_clicked();
	void on_btn_area_share_clicked();
	void on_btn_media_share_clicked();
	void on_tabWidget_currentChanged();
	void on_endShare();
	void on_beginShare();
	void on_choseShare(int x, int y, int w, int h);
	void on_cancelShare();
	void on_startShareRect();
	void on_changeShareRect();
};
