#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QWindowCourse.h"
#include "commonType.h"
#include "MemberService.h"
#include "QWidgetSMain.h"
#include "QWidgetMemberPanel.h"
#include "QWidgetTab.h"
#include "QWidgetMsgPanel.h"
#include "QWidgetBottomBar.h"
#include "AccountAuth.h"

typedef std::function<void(void)> txfunction;

class QWindowCourse
	: public QMainWindow
	, public IMemberService
    , public IMRecvMsgCallback
{
	Q_OBJECT

public:
	QWindowCourse(QWidget *parent = Q_NULLPTR);
	~QWindowCourse();

    void init(AccountAuth& info);
	void refreshMemberList();
	void modifyMemberInfo(const std::vector<ModifyMemberInfoItem>& infos);      // 开关麦克风或摄像头

protected:
	void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

protected:
	virtual void onGetMemberList(int total, const std::vector<MemberItem>& memberList);
	virtual void onModifyMemberInfo(int code, const char *desc);
	virtual void onInviteInteract(int code, const char *desc, const std::string& userId);
	virtual void onApplyInteract(int code, const char *desc);
    virtual void onGrandPermission(int code, const char *desc);
	virtual void onModifyConfInfo(int code, const char *desc);

    virtual void onRecvC2CTextMsg(const char * userID, const char * msg);
    virtual void onRecvGroupTextMsg(const char * groupID, const char * userID, const char * msg);
    virtual void onRecvC2CCustomMsg(const char * userID, const char * msg);
    virtual void onRecvGroupCustomMsg(const char * groupID, const char * userID, const char * msg);
    virtual void onRecvGroupSystemMsg(const char * groupID, const char * msg);

signals:
    void sub_state_changed(int);
	void dispatch(txfunction func);
	void mainWindowMoved(QPoint pos);

private slots:
    void on_btn_close_clicked();
	void handle(txfunction func);
	void on_modifyMemberInfo(ModifyMemberInfoItem info);      // 开关麦克风或摄像头
	void on_modifyConfInfo(std::string home_screen);
	void on_grandPermission(std::string userID, int permission);
	void on_endInteract(const char* userID);
	void on_actInvite(std::string userID);
	void on_notifyToast(QString text);
private:
	void onNotifyInviteInteract();
	void acceptInteract();//普通观众接受连麦邀请
	//void refuseInteract();//普通观众拒绝连麦邀请
	void exitInteract();//连麦观众执行主播发出的断线命令
	void modifyPermission(int permisson);
	void abnormalExit();

    Ui::QWindowCourse ui;

	QPoint mousePressedPosition; // 鼠标按下时的坐标
	QPoint windowPositionAsDrag; // 鼠标按下时窗口左上角的坐标
	QWidgetTab * widgetTab;
	QWidgetMemberPanel * memberPanel;
	QWidgetMsgPanel* imPanel;
	QWidgetBottomBar * widgetBar;
	QWidgetSMain * widgetSMain;
	QWidgetToast* m_toast;
	AccountAuth* m_accountInfo;
    AuthInfo m_authInfo;
    std::shared_ptr<MemberService> m_memberService;
};
