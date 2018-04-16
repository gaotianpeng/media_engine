#pragma once
#include <functional>
#include <QWidget>
#include <QMainWindow>
#include "ui_QWidgetMemberPanel.h"
#include "commonType.h"

typedef std::function<void(void)> txfunction;

class QWidgetMemberPanel: public QWidget
{
    Q_OBJECT

public:
    QWidgetMemberPanel(QWidget *parent = Q_NULLPTR);
    ~QWidgetMemberPanel();

    void setRoomId(int id);
    void setRoomOwner(const std::string& owner);
    void setJoinUrl(const std::string& url);
    void setMainWindow(QMainWindow* mainWidow);

    void setMemberList(const std::vector<MemberItem>& memberList);
    void addMember(const MemberItem& item);
    void removeMember(const std::string& userId);
    void modifyMember(const std::map<std::string, ModifyMemberInfoItem>& items);
protected slots:
    void onInviteBtnClicked();
private:
    Ui::QWidgetMemberPanel ui;
    QMainWindow* m_mainWidow;

    int m_roomId;
    std::string m_roomOwner;
    std::string m_joinUrl;
};
