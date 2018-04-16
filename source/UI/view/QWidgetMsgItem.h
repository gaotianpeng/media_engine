#pragma once

#include <QWidget>
#include "ui_QWidgetMsgItem.h"

class QWidgetMsgItem : public QWidget
{
    Q_OBJECT

public:
    QWidgetMsgItem(QWidget *parent = Q_NULLPTR);
    ~QWidgetMsgItem();

    void setMsgContent(const QString& msg);
private:
    Ui::QWidgetMsgItem ui;
};
