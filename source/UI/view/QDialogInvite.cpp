#include "QDialogInvite.h"

#include <QClipboard>
#include <QMouseEvent>

QDialogInvite::QDialogInvite(QWidget *parent)
    : QDialog(parent)
    , m_pressed(false)
    , m_point()
    , m_toast(nullptr)
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(onCloseBtnClicked()));
    connect(ui.btn_copy_id, SIGNAL(clicked()), this, SLOT(onCopyIdBtnClicked()));
    connect(ui.btn_copy_url, SIGNAL(clicked()), this, SLOT(onCopyUrlBtnClicked()));
    connect(ui.btn_ok, SIGNAL(clicked()), this, SLOT(onOkBtnClicked()));
    connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(onCancelBtnClicked()));
}

QDialogInvite::~QDialogInvite()
{

}

void QDialogInvite::setRoomId(const QString& id)
{
    ui.tb_room_id->setText(id);
}

void QDialogInvite::setJoinUrl(const QString& url)
{
    ui.tb_join_url->setText(url);
}

void QDialogInvite::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pressed = true;
        m_point = event->pos();
    }
}

void QDialogInvite::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pressed)
    {
        move(event->pos() - m_point + pos());
    }
}

void QDialogInvite::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_pressed = false;
}

void QDialogInvite::onCloseBtnClicked()
{
    reject();
}

void QDialogInvite::onCopyIdBtnClicked()
{

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui.tb_room_id->text());

    if (!m_toast)
    {
        m_toast = new QWidgetToast(this);
    }

    m_toast->setDuration(1000);
    m_toast->setText(QString::fromWCharArray(L"���Ƴɹ�"));
}

void QDialogInvite::onCopyUrlBtnClicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui.tb_join_url->text());

    if (!m_toast)
    {
        m_toast = new QWidgetToast(this);
    }

    m_toast->setDuration(1000);
    m_toast->setText(QString::fromWCharArray(L"���Ƴɹ�"));
}

void QDialogInvite::onOkBtnClicked()
{
    accept();
}

void QDialogInvite::onCancelBtnClicked()
{
    reject();
}
