#include "QWidgetToast.h"
#include <QPainter>
#include <QApplication>  
#include <QDesktopWidget> 

QWidgetToast::QWidgetToast(QWidget *parent) :
	QWidget(parent),
	m_pLabel(NULL),
	m_pTimer(NULL),
	m_pCloseTimer(NULL),
	m_dTransparent(1.0),
	m_nMSecond(3000),
	m_bCloseOut(false)
{
	if (!parent)
	{
		QRect rect = QApplication::desktop()->screenGeometry();
		m_nParentHeight = rect.height();
		m_nParentWidth = rect.width();
	}
	else
	{
		m_nParentWidth = parentWidget()->width();
		m_nParentHeight = parentWidget()->height();
	}

	this->setFixedHeight(55);
	m_pLabel = new QLabel(this);
	m_pLabel->setFixedHeight(55);
	m_pLabel->move(0, 0);
	m_pLabel->setAlignment(Qt::AlignCenter);
	m_pLabel->setStyleSheet("color:white");
	this->hide();
	this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	m_pTimer = new QTimer();
	m_pTimer->setInterval(m_nMSecond);
	m_pCloseTimer = new QTimer();
	m_pCloseTimer->setInterval(20);
	connect(m_pTimer, &QTimer::timeout, this, &QWidgetToast::onTimerStayOut);
	connect(m_pCloseTimer, &QTimer::timeout, this, &QWidgetToast::onTimerCloseOut);
}

QWidgetToast::~QWidgetToast()
{
	delete m_pTimer;
	delete m_pCloseTimer;
}

void QWidgetToast::setText(const QString &text) {
	QFontMetrics fm(this->font());
	int width = std::max(fm.width(text) * 1.2,100.0);
	this->setFixedWidth(width);
	m_pLabel->setFixedWidth(width);
	this->m_pLabel->setText(text);
	this->show();
	m_pTimer->start();
}

void QWidgetToast::setDuration(int nMSecond)
{
	m_nMSecond = std::max(1000, nMSecond);
	m_nMSecond = std::min(10000, m_nMSecond);
	m_pTimer->setInterval(m_nMSecond);
}

void QWidgetToast::setCloseOut(bool closeOut)
{
	m_bCloseOut = closeOut;
}

void QWidgetToast::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(0, 0, 0, 180));
	painter.drawRoundedRect(rect(), 5, 5);
	QWidget::paintEvent(e);
}

void QWidgetToast::showEvent(QShowEvent *e)
{
	if (!m_bCloseOut)
	{
		QTimer::singleShot(m_nMSecond, this, SLOT(hide()));
	}
	this->move(m_nParentWidth / 2 - this->width() / 2,
		m_nParentHeight / 2 - this->height() / 2);
	QWidget::showEvent(e);
}

void QWidgetToast::onTimerStayOut()
{
	m_pTimer->stop();
	if (m_pCloseTimer->isActive()) {
		m_pCloseTimer->stop();
	}
	m_pCloseTimer->start();
}

void QWidgetToast::onTimerCloseOut()
{
	m_dTransparent -= 0.01;
	if (m_dTransparent <= 0.0001)
	{
		m_pCloseTimer->stop();
		this->hide();
		m_pLabel->clear();
	}
	else
	{
		setWindowOpacity(m_dTransparent);
	}
}