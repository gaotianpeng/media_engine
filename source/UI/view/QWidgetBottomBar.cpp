#include <QCheckBox.h>
#include <QApplication>
#include <QDesktopWidget>
#include "QWidgetBottomBar.h"
#include "IMService.h"

extern QWidget * mainSWindow;
//extern QWidget * mainWindow;
QWidget * bottomBar = nullptr;
QWidgetBottomBar::QWidgetBottomBar(std::shared_ptr<MemberService> memberService, QWidget *parent)
	: QWidget(parent)
	, m_memberService(memberService)
	, m_toast(nullptr)
	, m_volume(nullptr)
	, m_bMute(false)
{
	ui.setupUi(this);
	bottomBar = this;
	int m_nDesktopWidth = QApplication::desktop()->width();
	setGeometry(QRect(QPoint(0, 0), QPoint(m_nDesktopWidth, this->size().height())));

	qRegisterMetaType<txfunction>("txfunction");
	connect(this, SIGNAL(dispatch(txfunction)), this, SLOT(handle(txfunction)), Qt::QueuedConnection);
	connect(this, SIGNAL(notifyFullScreen()), mainSWindow, SLOT(on_fullScreen()));
	//connect(ui.btnVolume, &QRadioButton::clicked, this, &QWidgetBottomBar::on_btnVolume_toggled);
	//connect(ui.btnVolume, &QCheckBox::toggled, this, &QWidgetBottomBar::on_btnVolume_toggled);
}

QWidgetBottomBar::~QWidgetBottomBar()
{
	SafeDelete(m_volume)
}

void QWidgetBottomBar::on_btnHandup_clicked()
{
	if (m_memberService)
	{
		m_memberService->applyInteract(1<<1|1<<2);
	}
}

void QWidgetBottomBar::on_btnFullScreen_clicked()
{
	emit notifyFullScreen();
}

void QWidgetBottomBar::on_btnVolume_toggled(bool checked)
{
	if (checked)
	{
		if (!m_volume)
		{
			m_volume = new QWidgetVolume(this);
		}
		m_volume->updateVolume();
		m_volume->show();
		QPoint pt = ui.btnVolume->pos();
		//ui.btnVolume->mapToParent(pt);
		//QPoint ptParent = ui.btnVolume->mapToParent(pt);
		QPoint ptGlobal = this->mapToGlobal(pt);
		QPoint ptVolume(ptGlobal.x() - 10, ptGlobal.y() - (m_volume->size().height()+15));
		m_volume->move(ptVolume);
	}
	else
	{
		if (m_volume)
		{
			m_volume->hide();
		}
	}
}

void QWidgetBottomBar::on_closeVolume(bool checked)
{
	if (!checked)
	{
		ui.btnVolume->setChecked(checked);
		if (m_volume)
		{
			m_volume->hide();
		}
	}
}

void QWidgetBottomBar::on_mainWindow_moved(QPoint pos)
{
	if (m_volume)
	{
		QPoint pt = ui.btnVolume->pos();
		QPoint ptGlobal = this->mapToParent(pt);
		QPoint ptVolume(ptGlobal.x() - 10, ptGlobal.y() - (m_volume->size().height()-35));
		m_volume->move(pos+ ptVolume);
	}
}

void QWidgetBottomBar::handle(txfunction func)
{
	func();
}

void QWidgetBottomBar::onApplyInteract(int code, const char * desc)
{
	if (!code)
	{
		emit dispatch([this] {
			if (!m_toast)
			{
				delete m_toast;				
			}
			m_toast = new QWidgetToast();
			m_toast->setDuration(1000);
			m_toast->setText(QString::fromWCharArray(L"等待老师批准发言"));
			//ui.btnHandup->setStyleSheet("QToolButton#btnHandup{border-image:url(:/Course/course/handing.png)}");
			ui.btnHandup->setToolTip(QString::fromWCharArray(L"等待老师批准发言"));
			ui.btnHandup->setDisabled(true);
		});
	}
	else
	{
		if (!m_toast)
		{
			delete m_toast;
		}
		m_toast = new QWidgetToast();
		m_toast->setDuration(1000);
		m_toast->setText(QString(desc));
	}
}

void QWidgetBottomBar::enterFullScreen()
{

}

void QWidgetBottomBar::onGrandPermission(int permission, bool bEndInteract)
{
	if (!permission)
	{
		ui.btnHandup->setDisabled(false);
		ui.btnHandup->setToolTip(QString::fromWCharArray(L""));
		if (!m_toast)
		{
			delete m_toast;
		}
		m_toast = new QWidgetToast();
		m_toast->setDuration(1000);
		if (bEndInteract)
		{
			m_toast->setText(QString::fromWCharArray(L"连接被教师断开"));
		}
		else
			m_toast->setText(QString::fromWCharArray(L"上麦请求被驳回"));
	}
	else if (permission == 1<<1)
	{
		m_bMute = true;
		if (!m_toast)
		{
			delete m_toast;
		}
		m_toast = new QWidgetToast();
		m_toast->setDuration(1000);
		m_toast->setText(QString::fromWCharArray(L"已被老师静音"));
	}
	else
	{
		ui.btnHandup->setToolTip(QString::fromWCharArray(L""));
		if (!m_toast)
		{
			delete m_toast;
		}
		m_toast = new QWidgetToast();
		m_toast->setDuration(1000);
		if (m_bMute)
		{
			m_bMute = false;
			m_toast->setText(QString::fromWCharArray(L"已被老师恢复声音"));
		}
		else
			m_toast->setText(QString::fromWCharArray(L"已被教师打开声音"));
	}
}

