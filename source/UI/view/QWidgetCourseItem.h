#pragma once

#include <QWidget>
#include "ui_QWidgetCourseItem.h"
#include "commonType.h"

class QWidgetCourseItem : public QWidget
{
    Q_OBJECT

public:
    QWidgetCourseItem(const RoomItem& item, QWidget *parent = Q_NULLPTR);
    ~QWidgetCourseItem();

    RoomItem roomItem() const;
private:
    Ui::QWidgetCourseItem ui;
    RoomItem m_item;
};
