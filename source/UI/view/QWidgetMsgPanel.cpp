#include "QWidgetMsgPanel.h"
#include "QWidgetMsgItem.h"
#include "IMService.h"

#include <QScrollBar>

QWidgetMsgPanel::QWidgetMsgPanel(std::shared_ptr<MemberService> memberService, QWidget *parent)
    : QWidget(parent)
    , m_memberService(memberService)
    , m_nickName("")
    , m_userId("")
    , m_roomOwner("")
    , m_groupId("")
{
    ui.setupUi(this);

    ui.lw_msg_list->setSelectionMode(QListWidget::NoSelection);

    // 切换主线程
    qRegisterMetaType<txfunction>("txfunction");
    connect(this, SIGNAL(dispatch(txfunction)), this, SLOT(handle(txfunction)), Qt::QueuedConnection);

    connect(ui.btn_send, SIGNAL(clicked()), this, SLOT(onSendBtnClicked()));
    connect(ui.tb_msg, SIGNAL(enterPressed()), this, SLOT(onEnterPress()));
    connect(ui.tb_msg, SIGNAL(ctrlEnterPressed()), this, SLOT(onCtrlEnterPress()));

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
	ui.tb_msg->verticalScrollBar()->setStyleSheet(scrollBarStyle);
	ui.lw_msg_list->verticalScrollBar()->setStyleSheet(scrollBarStyle);
}

QWidgetMsgPanel::~QWidgetMsgPanel()
{
}

void QWidgetMsgPanel::setNickName(const QString& nick)
{
    m_nickName = nick;
}

void QWidgetMsgPanel::setUserId(const QString& id)
{
    m_userId = id;
}

void QWidgetMsgPanel::setRoomOwner(const QString& owner)
{
    m_roomOwner = owner;
}

void QWidgetMsgPanel::setGroupId(const QString& id)
{
    m_groupId = id;
}

void QWidgetMsgPanel::addMsgContent(const QString& userId, const QString& msg)
{
	//应tomzhu要求，加入群第一条消息必须激活，所以如果是来自admin则不处理
	if (userId == "admin" || userId == "@TIM#SYSTEM")
	{
		return;
	}
    std::string nickName = m_memberService->nickName(userId.toStdString());

    QString content;
    if (m_roomOwner == userId)
    {
        content = QString::fromWCharArray(L"<b>%1(管理员): %2</b>").arg(nickName.c_str()).arg(msg);
    }
    else
    {
        content = QString::fromWCharArray(L"<font color=\'blue\'>%1</font>: %2").arg(nickName.c_str()).arg(msg);
    }

    QWidgetMsgItem *itemView = new QWidgetMsgItem();
    itemView->setMsgContent(content);

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(itemView->size());
    ui.lw_msg_list->addItem(item);
    ui.lw_msg_list->setItemWidget(item, itemView);
    ui.lw_msg_list->scrollToItem(item);
}

void QWidgetMsgPanel::addSysMsgContent(const QString& userId, const QString& msg)
{
	if (userId == "admin" || userId == "@TIM#SYSTEM")
	{
		return;
	}
	std::string nickName = m_memberService->nickName(userId.toStdString());

	QString content;
	if (m_roomOwner != userId)
	{
		content = QString::fromWCharArray(L"<font color=\'blue\'>%1</font>%2").arg(nickName.c_str()).arg(msg);
	}

	QWidgetMsgItem *itemView = new QWidgetMsgItem();
	itemView->setMsgContent(content);

	QListWidgetItem *item = new QListWidgetItem();
	item->setSizeHint(itemView->size());
	ui.lw_msg_list->addItem(item);
	ui.lw_msg_list->setItemWidget(item, itemView);
	ui.lw_msg_list->scrollToItem(item);
}

void QWidgetMsgPanel::sendGroupMsg(const std::string& msg)
{
    IMService::shared_instance()->sendGroupTextMsg(msg.c_str());
}

void QWidgetMsgPanel::sendC2CMsg(const std::string& destUserId, const std::string& msg)
{
    IMService::shared_instance()->sendC2CTextMsg(destUserId.c_str(), msg.c_str());
}


void QWidgetMsgPanel::onRecvC2CTextMsg(const char * userId, const char * msg)
{
    if (!userId || !*userId || !msg || !*msg)
    {
        return;
    }

    QString msgUserId = userId;
    QString msgContent = msg;
    emit dispatch([this, msgUserId, msgContent] {
        addMsgContent(msgUserId, msgContent);
    });
}

void QWidgetMsgPanel::onRecvGroupTextMsg(const char * groupId, const char * userId, const char * msg)
{
    if (!userId || !*userId || !msg || !*msg)
    {
        return;
    }

    QString msgUserId = userId;
    QString msgContent = msg;
    emit dispatch([this, msgUserId, msgContent] {
        addMsgContent(msgUserId, msgContent);
    });
}

void QWidgetMsgPanel::handle(txfunction func)
{
    func();
}

void QWidgetMsgPanel::onSendBtnClicked()
{
    QString msg = ui.tb_msg->toPlainText();
    if (msg.isEmpty())
    {
        return;
    }

    addMsgContent(m_userId, msg);

	sendGroupMsg(msg.toStdString().c_str());

    ui.tb_msg->clear();
}

void QWidgetMsgPanel::onEnterPress()
{
    if (!ui.tb_msg->hasFocus())
    {
        return;
    }

    onSendBtnClicked();
}

void QWidgetMsgPanel::onCtrlEnterPress()
{
    if (!ui.tb_msg->hasFocus())
    {
        return;
    }

    QString msg = ui.tb_msg->toPlainText();
    msg.append("\n");

    ui.tb_msg->setPlainText(msg);
    ui.tb_msg->moveCursor(QTextCursor::End);
}
