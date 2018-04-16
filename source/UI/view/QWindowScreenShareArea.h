#pragma once
#include <wtypes.h>
#include <QMainWindow>
#include <QLabel>

#include "ui_QWindowScreenShareArea.h"

class QWindowScreenShareArea : public QMainWindow
{
	Q_OBJECT

public:
	QWindowScreenShareArea(QWidget *parent = Q_NULLPTR);
	~QWindowScreenShareArea();

	void init();
private:
	//void testEdge();

protected:
	void mousePressEvent(QMouseEvent *event)  override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent*) override;


signals:
	void notifyCancelShare();
	void notifyChoseShare(int x, int y, int w, int h);

private:
	Ui::QWindowScreenShareArea ui;
	int m_nDesktopWidth;
	int m_nDesktopHeight;
	bool m_bMouseDown;
	bool m_bAreaChose;//是否选择了一块区域	
	RECT m_rcChose;
	RECT m_rcScreen;

	int m_curpos;
	int    edgeMargin;
	QPoint m_PressedPos;
	QPoint m_CurrentPos;
	QPoint m_PressedGlobalPos;
	QPoint m_CurrentGlobalPos;
	QPoint m_dragPosition;
	QLabel* m_pLabel;
};
