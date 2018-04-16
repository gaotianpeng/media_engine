#pragma once

#include <QTextEdit>
#include <QKeyEvent>

class QTextEditCustomEditor : public QTextEdit
{
    Q_OBJECT

public:
    QTextEditCustomEditor(QWidget *parent);
    ~QTextEditCustomEditor();
signals:
    void enterPressed();
    void ctrlEnterPressed();
protected:
    virtual void keyPressEvent(QKeyEvent * ev);
};
