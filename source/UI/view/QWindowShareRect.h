#pragma once

#include <QMainWindow>
#include <QLabel>
#include "QWindowShareToolBar.h"
#include "ui_QWindowShareRect.h"

class QWindowShareRect : public QMainWindow
{
	Q_OBJECT

public:
	QWindowShareRect(QWidget *parent = Q_NULLPTR);
	~QWindowShareRect();

	void init(int x, int y, int w, int h, QWindowShareToolBar *pTool);
	void getShareRect(int& left, int& top, int& right, int& bottom);

	enum {
		nodir,
		top = 0x01,
		bottom = 0x02,
		left = 0x04,
		right = 0x08,
		topLeft = 0x01 | 0x04,
		topRight = 0x01 | 0x08,
		bottomLeft = 0x02 | 0x04,
		bottomRight = 0x02 | 0x08
	} resizeDir;

protected:
	void mousePressEvent(QMouseEvent *event)  override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	void testEdge();
signals:
	void notifyChangeShareRect();
private:
	Ui::QWindowShareRect ui;
	QPoint dragPosition;   //鼠标拖动的位置
	QLabel* m_pLabel;
	int    edgeMargin;     //鼠标检测的边缘距离
	QPoint m_PressedPos;
	QPoint m_CurrentPos;
	QPoint m_PressedGlobalPos;
	QPoint m_CurrentGlobalPos;
	QWindowShareToolBar *m_pTool;
	bool m_bRectChange;

	int m_curpos;
};
