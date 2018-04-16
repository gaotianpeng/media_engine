#include "QWindowScreenTool.h"
#include "VideoService.h"
#include <QApplication>
#include <QPropertyAnimation>
#include <QDesktopWidget>

extern QWidget * mainTabWindow;

QWindowScreenTool::QWindowScreenTool(QWidget *parent)
	: QWidget(parent)
	, m_pTimer(nullptr)
	, m_enDriection(None)
	, m_bIsAutoHide(false)
	, m_bFullScreen(true)
{
	setWindowFlags(Qt::FramelessWindowHint |Qt::SubWindow| Qt::WindowStaysOnTopHint);
	int curMonitor = QApplication::desktop()->screenNumber(this);
	m_nDesktopWidth = QApplication::desktop()->screenGeometry(curMonitor).width();
	
	ui.setupUi(this);
	m_pTimer = new QTimer();
	m_pTimer->setInterval(5000);
	connect(m_pTimer, &QTimer::timeout, this, &QWindowScreenTool::onTimeOut);
	connect(this, SIGNAL(notifyEndShare()), mainTabWindow, SLOT(on_endShare()));
}

QWindowScreenTool::~QWindowScreenTool()
{
	SafeDelete(m_pTimer)
}

void QWindowScreenTool::initPosition(bool bFull)
{
	m_bFullScreen = bFull;
	if (bFull)
	{
		move((m_nDesktopWidth - this->width()) / 2, 0);
		m_pTimer->start();
	}
}

void QWindowScreenTool::leaveEvent(QEvent *event)
{
	if (m_bFullScreen)
	{
		isAutoHide();
		if (m_bIsAutoHide)
		{
			hideWidget();
		}
	}
}

void QWindowScreenTool::enterEvent(QEvent *event)
{
	if (m_bIsAutoHide && m_bFullScreen)
	{
		showWidget();
	}
}

void QWindowScreenTool::on_btn_endShare_clicked()
{
	emit notifyEndShare();
}

void QWindowScreenTool::onTimeOut()
{
	m_enDriection = Up;
	hideWidget();
	m_pTimer->stop();
}

void QWindowScreenTool::isAutoHide()
{
	QPoint pos = this->pos();

	m_bIsAutoHide = true;
	if (pos.x() < 3)
	{
		m_enDriection = Left;
	}
	else if (pos.y() < 3)
	{
		m_enDriection = Up;
	}
	else if (this->pos().x() + this->width() > m_nDesktopWidth - 3)
	{
		m_enDriection = Right;
	}
	else
	{
		m_enDriection = None;
		m_bIsAutoHide = false;
	}
}

void QWindowScreenTool::hideWidget()
{
	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
	animation->setDuration(100);
	animation->setStartValue(QRect(this->pos(), this->size()));

	QRect rcEnd;
	if (m_enDriection & Up)
	{
		rcEnd = QRect(this->x(), -this->height() + 3, this->size().width(), this->rect().height());
	}
	else if (m_enDriection & Left)
	{
		rcEnd = QRect(-this->width() + 3, this->y(), this->size().width(), this->rect().height());
	}
	else if (m_enDriection & Right)
	{
		rcEnd = QRect(m_nDesktopWidth - 3, this->y(), this->size().width(), this->rect().height());
	}
	animation->setEndValue(rcEnd);
	animation->start();
}

void QWindowScreenTool::showWidget()
{
	QPoint pos = this->pos();

	QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
	animation->setDuration(100);
	animation->setStartValue(QRect(pos, this->size()));

	QRect rcEnd;
	if (m_enDriection & Up)
	{
		rcEnd = QRect(this->x(), 0, this->size().width(), this->rect().height());
	}
	else if (m_enDriection & Left)
	{
		rcEnd = QRect(0, this->y(), this->size().width(), this->rect().height());
	}
	else if (m_enDriection & Right)
	{
		rcEnd = QRect(m_nDesktopWidth - this->width(), this->y(), this->size().width(), this->rect().height());
	}
	animation->setEndValue(rcEnd);
	animation->start();
}