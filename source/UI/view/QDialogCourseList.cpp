#include "QDialogCourseList.h"
#include "QWidgetCourseItem.h"
#include "QDialogDeviceMgr.h"
#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>
#include "QDialogMessage.h"

QDialogCourseList::QDialogCourseList(QWidget *parent)
    : QDialog(parent)
    , m_pressed(false)
    , m_point()
    , m_enterType(NoType)
    , m_roomId(0)
    , m_roomName("")
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
	setAttribute(Qt::WA_TranslucentBackground);

	QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
	shadowEffect->setOffset(0, 0);
	shadowEffect->setColor(Qt::gray);
	shadowEffect->setBlurRadius(5);
	ui.widgetMain->setGraphicsEffect(shadowEffect);

    connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(onCloseBtnClicked()));
    connect(ui.btn_join, SIGNAL(clicked()), this, SLOT(onJoinBtnClicked()));
    connect(ui.btn_create, SIGNAL(clicked()), this, SLOT(onCreateBtnClicked()));
	//connect(ui.btn_device, SIGNAL(clicked()), this, SLOT(on_btn_device_clicked()));

	ui.widge_no_room->setHidden(true);
	
	
	ui.lw_room_list->verticalScrollBar()->setStyleSheet(
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
})");

}

QDialogCourseList::~QDialogCourseList()
{
}

EnterType QDialogCourseList::enterType() const
{
    return m_enterType;
}

int QDialogCourseList::roomId() const
{
    return m_roomId;
}

QString QDialogCourseList::roomName() const
{
    return m_roomName;
}

void QDialogCourseList::setRoomList(const std::vector<RoomItem>& roomList)
{
	ui.lw_room_list->clear();
    for (std::vector<RoomItem>::const_iterator it = roomList.begin(); roomList.end() != it; ++it)
    {
        QWidgetCourseItem *itemView = new QWidgetCourseItem(*it, this);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(itemView->size());
        ui.lw_room_list->addItem(item);
        ui.lw_room_list->setItemWidget(item, itemView);
    }
	ui.lw_room_list->update();
	if(roomList.size() == 0)
	{
		ui.widge_no_room->setHidden(false);
	}
	else
	{
		ui.widge_no_room->setHidden(true);
	}
}

void QDialogCourseList::onCloseBtnClicked()
{
    m_enterType = NoType;
    m_roomName = "";

    done(0);
}

void QDialogCourseList::onJoinBtnClicked()
{
    QListWidgetItem * curentItem = ui.lw_room_list->currentItem();
    if (curentItem)
    {
        QWidgetCourseItem* itemView = (QWidgetCourseItem*)(ui.lw_room_list->itemWidget(curentItem));
        RoomItem item = itemView->roomItem();
        m_enterType = JoinRoomType;
        m_roomName = item.roomName.c_str();
        m_roomId = item.roomId;
    }
	else
	{
		m_enterType = RetryRoomType;
		QDialogMessage::exec(QString::fromWCharArray(L"请选择要加入的课程"), QDialogMessage::OK );
	}

    done(0);
}

void QDialogCourseList::onCreateBtnClicked()
{
    m_enterType = CreateRoomType;
    m_roomName = "";

    done(0);
}

void QDialogCourseList::on_btn_device_clicked()
{
	QDialogDeviceMgr mgr;
	mgr.initValue();
	mgr.exec();
}

void QDialogCourseList::on_btn_refresh_clicked()
{
	m_enterType = RetryRoomType;
	done(0);
}

void QDialogCourseList::on_lw_room_list_itemDoubleClicked(QListWidgetItem* item)
{
	if (item)
	{
		QWidgetCourseItem* itemView = (QWidgetCourseItem*)(ui.lw_room_list->itemWidget(item));
		RoomItem selected = itemView->roomItem();
		m_enterType = JoinRoomType;
		m_roomName = selected.roomName.c_str();
		m_roomId = selected.roomId;
		done(0);
	}
}

void QDialogCourseList::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pressed = true;
        m_point = event->pos();
    }
}

void QDialogCourseList::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pressed)
    {
        move(event->pos() - m_point + pos());
    }
}

void QDialogCourseList::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_pressed = false;
}
