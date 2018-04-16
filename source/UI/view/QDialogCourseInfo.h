#pragma once

#include <QDialog>

#include "ui_QDialogCourseInfo.h"
#include "AccountService.h"
#include "commonType.h"

class QDialogCourseInfo
    : public QDialog
{
    Q_OBJECT

public:
    QDialogCourseInfo(std::shared_ptr<AccountService> service, QWidget *parent = Q_NULLPTR);
    ~QDialogCourseInfo();

    void init(EnterType type, int roomId, const QString& roomName);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    void onCloseBtnClicked();
    void onOperateBtnClicked();
    void onCancelBtnClicked();
private:
    Ui::QDialogCourseInfo ui;
    bool m_pressed;
    QPoint m_point;

    std::shared_ptr<AccountService> m_service;
    EnterType m_type;
    int m_roomId;
    QString m_roomName;
};
