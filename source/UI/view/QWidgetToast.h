#pragma once

#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QTimer>

class QWidgetToast : public QWidget
{
	Q_OBJECT
public:
	explicit QWidgetToast(QWidget *parent = 0);
	~QWidgetToast();

	//Ĭ����ʾ3�룬�޽��������Ч�������������ӿڿ��Ը������ã�����Ҫ��setText֮ǰʹ�á�
	//��ʾʱ�������Ϊ[1000,10000]����
	void setDuration(int nMSecond);
	void setCloseOut(bool closeOut);

	void setText(const QString & text);

protected:
	void paintEvent(QPaintEvent *e);
	void showEvent(QShowEvent *e);
signals:

private slots :
	void onTimerStayOut();
	void onTimerCloseOut();//���������
private:
	QLabel* m_pLabel;
	QTimer * m_pTimer;
	QTimer * m_pCloseTimer;
	qreal m_dTransparent;
	int m_nParentWidth;
	int m_nParentHeight;
	int m_nMSecond;
	bool m_bCloseOut;
};