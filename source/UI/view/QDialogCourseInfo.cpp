#include "QDialogCourseInfo.h"
#include "iLive.h"
#include "Context.h"
#include "IMService.h"
#include "QDialogProgress.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <cstdlib>
#include <ctime>

QDialogCourseInfo::QDialogCourseInfo(QWidget *parent)
    : QDialog(parent)
    , m_pressed(false)
    , m_point()
    , m_type(NoType)
    , m_roomId(0)
    , m_roomName("")
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
	setAttribute(Qt::WA_TranslucentBackground);

	QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
	shadowEffect->setOffset(0, 0);
	shadowEffect->setColor(Qt::gray);
	shadowEffect->setBlurRadius(5);
	ui.widgetMain->setGraphicsEffect(shadowEffect);

    //connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(onCloseBtnClicked()));
    //connect(ui.btn_operate, SIGNAL(clicked()), this, SLOT(onOperateBtnClicked()));
    //connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(onCancelBtnClicked()));
}

QDialogCourseInfo::~QDialogCourseInfo()
{

}

void QDialogCourseInfo::init(EnterType type, int roomId, const QString& roomName)
{
    m_type = type;
    m_roomId = roomId;
    m_roomName = roomName;

    if (JoinRoomType == m_type)
    {
        ui.tb_court_name->setText(m_roomName);
        ui.btn_operate->setText(QStringLiteral("进入课堂"));

        ui.tb_court_name->setEnabled(false);
    }
    else if (CreateRoomType == m_type)
    {
        ui.tb_court_name->setText("");
        ui.btn_operate->setText(QStringLiteral("创建课堂"));

        ui.tb_court_name->setEnabled(true);
    }
}

void QDialogCourseInfo::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pressed = true;
        m_point = event->pos();
    }
}

void QDialogCourseInfo::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pressed)
    {
        move(event->pos() - m_point + pos());
    }
}

void QDialogCourseInfo::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_pressed = false;
}

//void QDialogCourseInfo::onCloseBtnClicked()
//{
//	done(-1);
//}
//
//void QDialogCourseInfo::onOperateBtnClicked()
//{
//	if (JoinRoomType == m_type)
//	{
//		QString courtName = ui.tb_court_name->text();
//		QString nickName = ui.tb_nick_name->text();
//		Context::instance().setRoomName(courtName.toStdString());
//		Context::instance().setNickName(nickName.toStdString());
//		Context::instance().setRoomID(m_roomId);
//		emit QDialogProgress::instance().showProgress(QStringLiteral("正在进入课堂"));
//		m_service->joinRoom(m_roomId, Context::instance().userPwd(), Context::instance().nickName());
//	}
//	else if (CreateRoomType == m_type)
//	{
//		QString courtName = ui.tb_court_name->text();
//		QString nickName = ui.tb_nick_name->text();
//		Context::instance().setRoomName(courtName.toStdString());
//		Context::instance().setNickName(nickName.toStdString());
//		emit QDialogProgress::instance().showProgress(QStringLiteral("正在创建课堂"));
//		m_service->createRoom(courtName.toStdString(), "", Context::instance().nickName());
//	}
//}
//
//void QDialogCourseInfo::onCancelBtnClicked()
//{
//    done(-1);
//}
