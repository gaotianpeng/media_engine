#pragma once

#include <QtWidgets/QDialog>
#include "ui_QDialogInvite.h"
#include "QWidgetToast.h"

class QDialogInvite : public QDialog
{
    Q_OBJECT

public:
    QDialogInvite(QWidget *parent = Q_NULLPTR);
    ~QDialogInvite();

    void setRoomId(const QString& id);
    void setJoinUrl(const QString& url);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
protected slots:
    void onCloseBtnClicked();
    void onCopyIdBtnClicked();
    void onCopyUrlBtnClicked();
    void onOkBtnClicked();
    void onCancelBtnClicked();
private:
    Ui::QDialogInvite ui;
    bool m_pressed;
    QPoint m_point;
    QWidgetToast* m_toast;
};
