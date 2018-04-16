#pragma once

#include <QtWidgets/QSlider>
#include <QMouseEvent>

class QSliderCustom : public QSlider
{
public:
	QSliderCustom(QWidget *parent = 0);
	virtual ~QSliderCustom();

protected:
	void mousePressEvent(QMouseEvent* event) override;
};