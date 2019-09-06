#pragma once 

#include "win-abstract-main.h"
#include "ui_MEBasic.h"
#include <memory>

class MEBasic : public MEMainWindow
{
	Q_OBJECT 
public:
	explicit MEBasic(QWidget *parent = 0);
	virtual ~MEBasic();

	virtual void MEInit() override;
protected:


private:
	std::unique_ptr<Ui::MainWindow> ui;
};