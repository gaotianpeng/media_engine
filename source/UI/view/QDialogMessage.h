#pragma once

#include <QtWidgets/QDialog>
#include "ui_QDialogMessage.h"

// 示例
// QDialogMessage::exec("腾讯云教育白板", QDialogMessage::OK | QDialogMessage::CANCEL);

class QDialogMessage : public QDialog
{
    Q_OBJECT

public:
    enum StandardButton
    {
        OK = 1,
        CANCEL = 2,
    };

    QDialogMessage(QWidget *parent = Q_NULLPTR);
    ~QDialogMessage();

    void setTitle(const QString& title);
	void setOKText(const QString& str);
	void setCancelText(const QString& str);

    static QDialog::DialogCode exec(const QString& title, int btns);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void closeEvent(QCloseEvent *event);

protected slots:
    void on_btn_close_clicked();
    void on_btn_ok_clicked();
    void on_btn_cancel_clicked();

private:
    Ui::QDialogMessage ui;

    bool m_pressed;
    QPoint m_point;
};
