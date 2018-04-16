#include "QSliderCustom.h"

QSliderCustom::QSliderCustom(QWidget *parent /*= 0*/)
	:QSlider(parent)
{

}

QSliderCustom::~QSliderCustom()
{

}

void QSliderCustom::mousePressEvent(QMouseEvent* event)
{
	int value = (event->pos().x() / (float)width()) * maximum();
	setValue(value);
	QSlider::mousePressEvent(event);
}
