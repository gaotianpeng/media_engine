#pragma once

#include <QWidget>
#include <QTimer>

#include "ui_QWidgetVolume.h"
#include "VideoService.h"

class QWidgetVolume : public QWidget
{
	Q_OBJECT

public:
	QWidgetVolume(QWidget *parent = Q_NULLPTR);
	~QWidgetVolume();

	void updateVolume();

protected:
	void leaveEvent(QEvent *event) override;
	void enterEvent(QEvent *event) override;
	void focusOutEvent(QFocusEvent *event) override;
signals:
	void notifyCloseVolume(bool checked);
	
private slots:
	void onTimeOut();
	void on_vsVolume_valueChanged(int value);
private:
	Ui::QWidgetVolume ui;
	QTimer * m_pTimer;
	uint32		m_uVolume;
};
