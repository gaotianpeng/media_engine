#pragma once

#include <QWidget>
#include "ui_QWidgetMsgPanel.h"
#include "MemberService.h"
#include "IMService.h"

typedef std::function<void(void)> txfunction;

class QWidgetMsgPanel
    : public QWidget
{
    Q_OBJECT

public:
    QWidgetMsgPanel(std::shared_ptr<MemberService> memberService, QWidget *parent = Q_NULLPTR);
    virtual ~QWidgetMsgPanel();

    void setNickName(const QString& nick);
    void setUserId(const QString& id);
    void setRoomOwner(const QString& owner);
    void setGroupId(const QString& id);

    void addMsgContent(const QString& userId, const QString& msg);
	void addSysMsgContent(const QString& userId, const QString& msg);
    void sendGroupMsg(const std::string& msg);
    void sendC2CMsg(const std::string& destUserId, const std::string& msg);

    void onRecvC2CTextMsg(const char * userId, const char * msg);
    void onRecvGroupTextMsg(const char * groupId, const char * userId, const char * msg);
signals:
    void dispatch(txfunction func);     // 投递线程队列
protected slots:
    void handle(txfunction func);       // 执行函数

    void onSendBtnClicked();
    void onEnterPress();
    void onCtrlEnterPress();
private:
    Ui::QWidgetMsgPanel ui;

    std::shared_ptr<MemberService> m_memberService;
    QString m_nickName;
    QString m_userId;
    QString m_roomOwner;
    QString m_groupId;
};
