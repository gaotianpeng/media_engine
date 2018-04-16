#pragma once

#include <QWidget>
#include "ui_QWindowShareToolBar.h"

class QWindowShareToolBar : public QWidget
{
	Q_OBJECT

public:
	QWindowShareToolBar(QWidget *parent = Q_NULLPTR);
	~QWindowShareToolBar();

signals:
	void notifyCancelShare();
	void notifyStartShareRect();

private slots:
	void on_ok_clicked();
	void on_cancel_clicked();

private:
	Ui::QWindowShareToolBar ui;
};
