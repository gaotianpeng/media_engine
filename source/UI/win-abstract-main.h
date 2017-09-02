#pragma once

#include <QMainWindow>

class MEMainWindow : public QMainWindow {
	Q_OBJECT

public:
	inline MEMainWindow(QWidget *parent) : QMainWindow(parent) {}

	virtual void MEInit() = 0;
};
