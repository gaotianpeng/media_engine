#include "QWidgetMemberItem.h"
#include <QMouseEvent>
extern QWidget * mainWindow;

QWidgetMemberItem::QWidgetMemberItem(QWidgetMemberPanel* panel, const MemberItem& item, bool isRoomOwner, QWidget *parent)
    : QWidget(parent)
    , m_panel(panel)
    , m_item(item)
{
    ui.setupUi(this);
	connect(this, SIGNAL(actInvite(std::string)), mainWindow, SLOT(on_actInvite(std::string)));
	m_invite_interact = nullptr;
    if (isRoomOwner)
    {
        QString nickName = item.nickName.c_str();
        nickName.append(QString::fromWCharArray(L"(管理员)"));
        ui.label_nick->setText(nickName);

        ui.label_status->setText(QString::fromWCharArray(L"主讲"));
		m_member_menu = nullptr;
    }
    else
    {
        ui.label_nick->setText(item.nickName.c_str());
        ui.label_status->setText(1 == item.mic ? QString::fromWCharArray(L"互动中") : QLatin1String(""));
		m_member_menu = new QMenu(this);
		m_invite_interact = new QAction(QString::fromLocal8Bit("邀请发言"), m_member_menu);
		m_member_menu->addAction(m_invite_interact);
		m_member_menu->setStyleSheet(".QMenu{background-color:#444444;\
		color: rgb(255,255,255);\
		font: 9pt \"微软雅黑\";}");
		connect(m_invite_interact, &QAction::triggered, this, [this] { emit actInvite(m_item.id); });
    }
}

QWidgetMemberItem::~QWidgetMemberItem()
{
	if (m_invite_interact)
	{
		delete m_invite_interact;
		m_invite_interact = nullptr;
	}

	if (m_member_menu)
	{
		delete m_member_menu;
		m_member_menu = nullptr;
	}
}

MemberItem QWidgetMemberItem::item() const
{
    return m_item;
}

void QWidgetMemberItem::setMemberItem(const MemberItem& aItem)
{
    m_item = aItem;
}

void QWidgetMemberItem::setInteractStatus(bool bInteract)
{
	if (bInteract)
	{
		ui.label_status->setText(QString::fromWCharArray(L"互动中"));
	}
	else
	{
		ui.label_status->setText(QLatin1String(""));
	}
}

void QWidgetMemberItem::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::RightButton)
	{
		if (m_member_menu)
		{
			m_member_menu->exec(QCursor::pos());
		}		
	}
}
