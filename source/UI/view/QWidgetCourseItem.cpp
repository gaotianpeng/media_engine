#include "QWidgetCourseItem.h"

QWidgetCourseItem::QWidgetCourseItem(const RoomItem& item, QWidget *parent)
    : QWidget(parent)
    , m_item(item)
{
    ui.setupUi(this);

    if (item.roomName.empty())
    {
        ui.label_room_name->setText(QString::number(item.roomId));
    }
    else
    {
        ui.label_room_name->setText(item.roomName.c_str());
    }
}

QWidgetCourseItem::~QWidgetCourseItem()
{
}

RoomItem QWidgetCourseItem::roomItem() const
{
    return m_item;
}
