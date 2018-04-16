#include "QWidgetMsgItem.h"

QWidgetMsgItem::QWidgetMsgItem(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

QWidgetMsgItem::~QWidgetMsgItem()
{
}

void QWidgetMsgItem::setMsgContent(const QString& msg)
{
    ui.label_msg->setText(msg);

    this->adjustSize();
}
