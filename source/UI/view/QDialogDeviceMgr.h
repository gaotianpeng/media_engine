#pragma once

#include <QWidget>
#include <QDialog>
#include <QTimer>
#include "ui_QDialogDeviceMgr.h"
#include "QWidgetVideoRender.h"


class QDialogDeviceMgr : public QDialog
{
	Q_OBJECT

public:
	QDialogDeviceMgr(QWidget *parent = Q_NULLPTR);
	~QDialogDeviceMgr();

	void initValue();
	void initUI();

	void updateDynamicVolume();
protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	static void OnStartDeviceTestSuc(void* data);
	static void OnStartDeviceTestErr(int code, const char *desc, void* data);
	static void OnStopDeviceTestSuc(void* data);
	static void OnStopDeviceTestErr(int code, const char *desc, void* data);
private slots:
	void on_btn_ok_clicked();
	void on_btn_cancel_clicked();
	void on_btn_close_clicked();
	void on_hsVolume_valueChanged(int value);
	void on_checkBox_mic_toggled(bool checked);
	void on_checkBox_camera_toggled(bool checked);
	void on_cb_mic_currentIndexChanged(int index);
	void on_cb_camera_currentIndexChanged(int index);
	void onTimeOut();
private:
	Ui::QDialogDeviceMgr ui;
	bool m_pressed;
	QPoint m_point;
	Vector< Pair<String/*id*/, String/*name*/> > m_cameraList;
	Vector< Pair<String/*id*/, String/*name*/> > m_micList;
	std::string m_cameraID;
	std::string m_micID;
	QTimer * m_pTimer;

};
