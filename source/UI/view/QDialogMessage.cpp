#include "QDialogMessage.h"

#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

QDialogMessage::QDialogMessage(QWidget *parent)
    : QDialog(parent)
    , m_pressed(false)
    , m_point()
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
	//setAttribute(Qt::WA_TranslucentBackground);

	QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
	shadowEffect->setOffset(0, 0);
	shadowEffect->setColor(Qt::gray);
	shadowEffect->setBlurRadius(5);
	ui.widgetMain->setGraphicsEffect(shadowEffect);
}

QDialogMessage::~QDialogMessage()
{

}

void QDialogMessage::setTitle(const QString& title)
{
    ui.label_title->setText(title);
}

void QDialogMessage::setOKText(const QString& str)
{
	ui.btn_ok->setText(str);
}

void QDialogMessage::setCancelText(const QString& str)
{
	ui.btn_cancel->setText(str);
}

QDialog::DialogCode QDialogMessage::exec(const QString& title, int btns)
{
    QDialogMessage msgBox;
    msgBox.ui.label_title->setText(title);
    msgBox.ui.btn_ok->setVisible(QDialogMessage::OK == (QDialogMessage::OK & btns));
    msgBox.ui.btn_cancel->setVisible(QDialogMessage::CANCEL == (QDialogMessage::CANCEL & btns));

    QDialog* dlg = static_cast<QDialog*>(&msgBox);

    return static_cast<QDialog::DialogCode>(dlg->exec());
}

void QDialogMessage::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pressed = true;
        m_point = event->pos();
    }
}

void QDialogMessage::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pressed)
    {
        move(event->pos() - m_point + pos());
    }
}

void QDialogMessage::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_pressed = false;
}

void QDialogMessage::closeEvent(QCloseEvent *event)
{
    QDialog::closeEvent(event);

    reject();
}

void QDialogMessage::on_btn_close_clicked()
{
    reject();
}

void QDialogMessage::on_btn_ok_clicked()
{
    accept();
}

void QDialogMessage::on_btn_cancel_clicked()
{
    reject();
}
