#include "SfxSettingsWidget.h"

SfxSettingsWidget::SfxSettingsWidget(QWidget *parent) : QScrollArea(parent)
{
    this->setLineWidth(20);
    _container = new QWidget();
    _layout = new QGridLayout(_container);
    _layout->setSizeConstraint(QLayout::SetMinimumSize);
//    _layout->setSpacing(0);
  //  _layout->setMargin(0);
    //_layout->setVerticalSpacing(0);
    this->setWidgetResizable(true);
    this->setMaximumHeight(200);
    this->setWidget(_container);
}

SfxSettingsWidget::SfxSettingsWidget(QString sfxName, QWidget *parent)
    :SfxSettingsWidget(parent)
{
    // adding checkbox
    _checkbox = new QCheckBox(tr("Enable ") + sfxName);
    _checkbox->setCheckable(true);
    _layout->addWidget( _checkbox, 0,0,1,5);

    connect(_checkbox,&QCheckBox::toggled, this, [=](bool state){
        for (auto &i: _sliders)
            i->setEnabled(state);
    });
}


void SfxSettingsWidget::addSlider(QString name,int min, int max, QString suffix,QString prefix)
{

    _sliders.append(new SliderSpin(min,max,suffix,prefix));
    _layout->addWidget(new QLabel(name), _layout->rowCount(),0,1,2);
    // Row count will be increased by one after adding the label so we substract 1 to stay on the same line
    _layout->addWidget( _sliders.last() ,_layout->rowCount() - 1 , 2,1,6);

    // need to have this outside the lambda else _slider.size() will return the current size
    int index = _sliders.size() -1;
    connect(_sliders.last(),&SliderSpin::valueChanged,this,[=](int newValue){
            emit valueChanged(index, newValue);
        });

}


void SfxSettingsWidget::addComboBox(QString label,QStringList values)
{
    _comboBox.append(new QComboBox());
    _layout->addWidget(new QLabel(label), _layout->rowCount(),0,1,2);
    _layout->addWidget(_comboBox.last() ,_layout->rowCount() - 1,2,1,6 );
    for (auto i: values)
        _comboBox.last()->addItem(i);

}
