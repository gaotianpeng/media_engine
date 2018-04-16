#include "QWidgetVolume.h"
#include <QRadioButton.h>

extern QWidget * bottomBar;
QWidgetVolume::QWidgetVolume(QWidget *parent)
	: QWidget(parent)
	, m_uVolume(0)
	, m_pTimer(nullptr)
{
	setWindowFlags( Qt::ToolTip);
	ui.setupUi(this);	
	resize(40,140);
	m_pTimer = new QTimer();
	m_pTimer->setInterval(1000);
	
	//connect(parent, &QRadioButton::toggled, this, [this] { emit doubleClicked();  });
	connect(this, SIGNAL(notifyCloseVolume(bool)), bottomBar, SLOT(on_closeVolume(bool)));
	connect(m_pTimer, &QTimer::timeout, this, &QWidgetVolume::onTimeOut);
}

QWidgetVolume::~QWidgetVolume()
{
	SafeDelete(m_pTimer)
}

void QWidgetVolume::leaveEvent(QEvent *event)
{
	m_pTimer->start();
}

void QWidgetVolume::enterEvent(QEvent *event)
{
	m_pTimer->stop();
}

void QWidgetVolume::focusOutEvent(QFocusEvent *event)
{
	emit notifyCloseVolume(false);
}

void QWidgetVolume::onTimeOut()
{
	m_pTimer->stop();
	//hide();
	emit notifyCloseVolume(false);
}

void QWidgetVolume::on_vsVolume_valueChanged(int value)
{
	GetILive()->setPlayerVolume(value);
}

void QWidgetVolume::updateVolume()
{
	m_uVolume = GetILive()->getPlayerVolume();
	ui.vsVolume->blockSignals(true);
	ui.vsVolume->setValue(m_uVolume);	
	ui.vsVolume->blockSignals(false);
}


