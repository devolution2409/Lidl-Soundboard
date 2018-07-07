#include "SfxSettingsWidget.h"

SfxSettingsWidget::SfxSettingsWidget(QWidget *parent) : QScrollArea(parent)
{
    _container = new QWidget();
    _layout = new QGridLayout(_container);


    this->setWidgetResizable(true);
    this->setWidget(_container);
}

SfxSettingsWidget::SfxSettingsWidget(QString sfxName, QWidget *parent)
    :SfxSettingsWidget(parent)
{
    // adding checkbox
    _checkbox = new QCheckBox(tr("Enable ") + sfxName);
    _checkbox->setCheckable(true);
    _layout->addWidget( _checkbox, 0,0,1,5);



}


void SfxSettingsWidget::addSlider(QString name,int min, int max, QString suffix,QString prefix)
{
    _sliders.append(new SliderSpin(min,max,suffix,prefix));
    if (_layout->rowCount() == 0)
        return;
    _layout->addWidget(new QLabel(name), _layout->rowCount(),0,1,1);
    // Row count will be increased by one after adding the label so we substract 1 to stay on the same line
    _layout->addWidget( _sliders.last() ,_layout->rowCount() - 1 , 1,1,4);

    // need to have this outside the lambda else _slider.size() will return the current size
    int index = _sliders.size() -1;
    connect(_sliders.last(),&SliderSpin::valueChanged,this,[=](int newValue){
            emit valueChanged(index, newValue);
        });

}


