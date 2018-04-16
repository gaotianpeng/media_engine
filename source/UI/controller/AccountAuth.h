#pragma once

#include "AccountService.h"
#include "commonType.h"
#include "QDialogCourseList.h"
#include "QDialogCourseInfo.h"

#include <QObject>
#include <QSettings>

typedef std::function<void(void)> txfunction;

class AccountAuth
    : public QObject
    , public IAccountService
{
    Q_OBJECT

public:
    AccountAuth(QObject *parent);
    ~AccountAuth();

    int exec(int reEnter = 0);
	void quit();
	void exitRoom();
    bool isEnterRoom() const;
    AuthInfo authInfo() const;
protected:
    virtual void onQueryRoomList(int code, const char *desc, const std::vector<RoomItem>& roomList);
    virtual void onRegister(int code, const char *desc);
    virtual void onLogin(int code, const char *desc);
    virtual void onLogout(int code, const char *desc);
    virtual void onCreateRoom(int code, const char *desc);
    virtual void onJoinRoom(int code, const char *desc);
    virtual void onReportJoinRoom(int code, const char *desc);
    virtual void onQuitRoom(int code, const char *desc);
	virtual void onQuitAVRoom();
	virtual void onError();
signals:
    void dispatch(txfunction func);     // 投递线程队列

private slots:
    void handle(txfunction func);       // 执行函数
private:
    std::shared_ptr<AccountService> m_service;
    QSettings m_settings;
    QDialogCourseList m_frontPage;
    QDialogCourseInfo m_enterRoom;
};
