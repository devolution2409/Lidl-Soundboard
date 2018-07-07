#include "SliderSpin.h"

SliderSpin::SliderSpin(QWidget *parent)
    : QWidget(parent)
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
    // following line is needed else each line will have margins BrokeBack Clap
    this->_layout->setMargin(0);
}

SliderSpin::SliderSpin(int min, int max, QString suffix, QString prefix, QWidget *parent)
    : SliderSpin(parent)
{

    this->setRange(min,max);
    if (! prefix.isEmpty())
        this->_spinbox->setPrefix(prefix);
    if (! suffix.isEmpty())
        this->_spinbox->setSuffix(suffix);
}

void SliderSpin::setRange(int min, int max)
{
    this->_slider->setRange(min,max);
    this->_spinbox->setRange(min,max);
}


void SliderSpin::setValue(int value)
{
    this->_slider->setValue(value);
    this->_spinbox->setValue(value);
}

void SliderSpin::setSuffix(const QString &suffix)
{
    this->_spinbox->setSuffix(suffix);
}

void SliderSpin::setPrefix(const QString &prefix)
{
    this->_spinbox->setPrefix(prefix);
}

int SliderSpin::value() const
{
    return this->_slider->value();
}

void SliderSpin::setEnabled(bool state)
{
    this->_spinbox->setEnabled(state);
    this->_slider->setEnabled(state);
}
