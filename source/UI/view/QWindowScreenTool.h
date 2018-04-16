#pragma once
#include <QTimer>
#include <QWidget>
#include "ui_QWindowScreenTool.h"

enum Direction
{
	None,
	Up,
	Right,
	Down,
	Left
};

class QWindowScreenTool : public QWidget
{
	Q_OBJECT

public:
	QWindowScreenTool(QWidget *parent = Q_NULLPTR);
	~QWindowScreenTool();

	void initPosition(bool bFull = true);
protected:
	void leaveEvent(QEvent *event) override;
	void enterEvent(QEvent *event) override;

private:
	void isAutoHide();
	void hideWidget();
	void showWidget();

signals:
	void notifyEndShare();
private slots:
	void on_btn_endShare_clicked();
	void onTimeOut();
private:
	Ui::QWindowScreenTool ui;
	QTimer * m_pTimer;
	Direction m_enDriection;
	bool m_bIsAutoHide;
	bool m_bFullScreen;

	int m_nDesktopWidth;
	
};
