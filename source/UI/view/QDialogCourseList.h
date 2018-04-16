#pragma once

#include <QDialog>
#include "ui_QDialogCourseList.h"
#include "commonType.h"

class QDialogCourseList : public QDialog
{
    Q_OBJECT

public:
    QDialogCourseList(QWidget *parent = Q_NULLPTR);
    ~QDialogCourseList();

    EnterType enterType() const;
    int roomId() const;
    QString roomName() const;

    void setRoomList(const std::vector<RoomItem>& roomList);
protected slots:
    void onCloseBtnClicked();
    void onJoinBtnClicked();
    void onCreateBtnClicked();
	void on_btn_device_clicked();
	void on_btn_refresh_clicked();
	void on_lw_room_list_itemDoubleClicked(QListWidgetItem* item);


    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
private:
	Ui::QDialogCourseList ui;
    bool m_pressed;
    QPoint m_point;

    EnterType m_enterType;
    int m_roomId;
    QString m_roomName;
};
