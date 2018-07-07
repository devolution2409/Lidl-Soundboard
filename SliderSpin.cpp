#include "SliderSpin.h"

SliderSpin::SliderSpin(QWidget *parent) : QWidget(parent)
{
    this->_layout = new QGridLayout(this);
    this->_slider = new QSlider(Qt::Orientation::Horizontal);
    this->_spinbox = new QSpinBox();
    connect(_slider, QSlider::valueChanged, _spinbox,QSpinBox::setValue);
    connect(_spinbox, static_cast<void (QSpinBox::*)(int)>(QSpinBox::valueChanged),
            _slider,QSlider::setValue);

    connect(_slider, &QSlider::valueChanged, this, [=](int value){
        emit valueChanged(value);
    });

    _layout->addWidget(_slider,0,0,1,8);
    _layout->addWidget(_spinbox,0,8,1,2);

}



void SliderSpin::setRange(int min, int max)
{
    this->_slider->setRange(min,max);
    this->_spinbox->setRange(min,max);
}




