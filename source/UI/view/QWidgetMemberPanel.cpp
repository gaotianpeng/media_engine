#include "QWidgetMemberPanel.h"
#include "QWidgetMemberItem.h"
#include "QDialogInvite.h"

#include <QScrollBar>

QWidgetMemberPanel::QWidgetMemberPanel(QWidget *parent)
    : QWidget(parent)
    , m_mainWidow(nullptr)
    , m_roomId(0)
    , m_roomOwner("")
    , m_joinUrl("")
{
    ui.setupUi(this);

    connect(ui.btn_invite, SIGNAL(clicked()), this, SLOT(onInviteBtnClicked()));

	const QString scrollBarStyle = 
R"(QScrollBar{
    background: transparent;
	 width: 7px;
}

QScrollBar::handle {
    background-color: #dbdbdb;
    border-radius: 3px;
}

QScrollBar::handle:hover {
    background-color: #dfdfdf;
}

QScrollBar::handle:pressed {
    background-color: #cccccc;
}

QScrollBar::add-line, QScrollBar::sub-line {
    background: transparent;
    height: 0px;
    width: 0px;
}

QScrollBar::add-page, QScrollBar::sub-page {
    background: transparent;
}

QScrollBar::up-arrow, QScrollBar::down-arrow {
    background: transparent;
    height: 0px;
    width: 0px;
})";
	ui.lw_member->verticalScrollBar()->setStyleSheet(scrollBarStyle);
}

QWidgetMemberPanel::~QWidgetMemberPanel()
{

}

void QWidgetMemberPanel::setRoomId(int id)
{
    m_roomId = id;
}

void QWidgetMemberPanel::setRoomOwner(const std::string& owner)
{
    m_roomOwner = owner;
}

void QWidgetMemberPanel::setJoinUrl(const std::string& url)
{
    m_joinUrl = url;
}

void QWidgetMemberPanel::setMainWindow(QMainWindow* mainWidow)
{
    m_mainWidow = mainWidow;
}

void QWidgetMemberPanel::setMemberList(const std::vector<MemberItem>& memberList)
{
    ui.lw_member->clear();

	for (std::vector<MemberItem>::const_iterator it = memberList.begin(); memberList.end() != it; ++it)
	{
        addMember(*it);
	}
}

void QWidgetMemberPanel::addMember(const MemberItem& item)
{
    QWidgetMemberItem *itemView = new QWidgetMemberItem(this, item, item.id == m_roomOwner);
    QListWidgetItem *widgetItem = new QListWidgetItem();
    widgetItem->setSizeHint(itemView->size());
    ui.lw_member->addItem(widgetItem);
    ui.lw_member->setItemWidget(widgetItem, itemView);
}

void QWidgetMemberPanel::removeMember(const std::string& userId)
{
    int count = ui.lw_member->count();
    for (int i = 0; i < count; ++i)
    {
        QListWidgetItem* widgetItem = ui.lw_member->item(i);
        QWidgetMemberItem *itemView = (QWidgetMemberItem *)(ui.lw_member->itemWidget(widgetItem));
        if (!itemView)
        {
            continue;
        }

        MemberItem item = itemView->item();
        if (userId == item.id)
        {
            //ui.lw_member->removeItemWidget(widgetItem);
			ui.lw_member->takeItem(i);
			delete widgetItem;
			return;
        }
    }
}

void QWidgetMemberPanel::modifyMember(const std::map<std::string, ModifyMemberInfoItem>& items)
{
    int count = ui.lw_member->count();
    for (int i = 0; i < count; ++i)
    {
        QListWidgetItem* widgetItem = ui.lw_member->item(i);
        QWidgetMemberItem *itemView = (QWidgetMemberItem *)(ui.lw_member->itemWidget(widgetItem));
        if (!itemView)
        {
            continue;
        }

        MemberItem item = itemView->item();

        std::map<std::string, ModifyMemberInfoItem>::const_iterator findIt = items.find(item.id);
        if (items.end() != findIt)
        {
            item.mic = findIt->second.mic;
            item.camera = findIt->second.camera;
            item.screen = findIt->second.screen;
            itemView->setMemberItem(item);
			if (item.mic|| item.camera)
			{
				itemView->setInteractStatus(true);
			}
			else
			{
				itemView->setInteractStatus(false);
			}
        }
    }
}

void QWidgetMemberPanel::onInviteBtnClicked()
{
    QDialogInvite panel(m_mainWidow);
    panel.setRoomId(QString::number(m_roomId, 10));
    panel.setJoinUrl(m_joinUrl.c_str());
    panel.exec();
}
