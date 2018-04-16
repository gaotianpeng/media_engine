#pragma once

#include <QWidget>
#include "ui_QWidgetBottomBar.h"
#include "MemberService.h"
#include "QWidgetToast.h"
#include "QWidgetVolume.h"

typedef std::function<void(void)> txfunction;

class QWidgetBottomBar : public QWidget
{
	Q_OBJECT

public:
	QWidgetBottomBar(std::shared_ptr<MemberService> memberService, QWidget *parent = Q_NULLPTR);
	~QWidgetBottomBar();

	void onApplyInteract(int code, const char * desc);	
	void enterFullScreen();
	void onGrandPermission(int permission, bool bEndInteract);

signals:
	void dispatch(txfunction func);
	void notifyFullScreen();
private slots:
	void handle(txfunction func);
	void on_btnHandup_clicked();
	void on_btnFullScreen_clicked();
	void on_btnVolume_toggled(bool checked);
	void on_closeVolume(bool checked);
	void on_mainWindow_moved(QPoint pos);
private:
	Ui::QWidgetBottomBar ui;
	std::shared_ptr<MemberService> m_memberService;
	QWidgetToast* m_toast;
	QWidgetVolume* m_volume;
	bool m_bMute;
};
