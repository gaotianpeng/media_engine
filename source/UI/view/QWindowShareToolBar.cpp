#include "QWindowShareToolBar.h"

extern QWidget * mainTabWindow;
QWindowShareToolBar::QWindowShareToolBar(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnTopHint);
	ui.setupUi(this);
	connect(this, SIGNAL(notifyCancelShare()), mainTabWindow, SLOT(on_cancelShare()));
	connect(this, SIGNAL(notifyStartShareRect()), mainTabWindow, SLOT(on_startShareRect()));
}

QWindowShareToolBar::~QWindowShareToolBar()
{
}

void QWindowShareToolBar::on_ok_clicked()
{
	hide();
	emit notifyStartShareRect();	
}

void QWindowShareToolBar::on_cancel_clicked()
{
	hide();
	emit notifyCancelShare();	
}
