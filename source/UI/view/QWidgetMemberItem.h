#pragma once

#include <QWidget>
#include <QMenu>
#include "ui_QWidgetMemberItem.h"
#include "QWidgetMemberPanel.h"

class QWidgetMemberItem
    : public QWidget
{
    Q_OBJECT

public:
    QWidgetMemberItem(QWidgetMemberPanel* panel, const MemberItem& item, bool isRoomOwner, QWidget *parent = Q_NULLPTR);
    ~QWidgetMemberItem();

    MemberItem item() const;
    void setMemberItem(const MemberItem& aItem);
	void setInteractStatus(bool bInteract);

protected:
	void    mousePressEvent(QMouseEvent * event) override;
signals:
	void    actInvite(std::string userId);
private:
    Ui::QWidgetMemberItem ui;
    QWidgetMemberPanel* m_panel;
    MemberItem m_item;
	QMenu *m_member_menu;
	QAction* m_invite_interact;
};
