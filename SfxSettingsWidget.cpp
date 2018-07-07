#include "SfxSettingsWidget.h"

SfxSettingsWidget::SfxSettingsWidget(QWidget *parent) : QScrollArea(parent)
{
  //  this->setLineWidth(20);
    _container = new QWidget();
    _layout = new QGridLayout(_container);
    _layout->setSizeConstraint(QLayout::SetMinimumSize);
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
        for (auto &i: _comboBox)
            i->setEnabled(state);
        emit checkBoxStateChanged(state);
    });
}

void SfxSettingsWidget::deactivateAll()
{
    for (auto &i: _sliders)
        i->setEnabled(false);
    for (auto &i: _comboBox)
        i->setEnabled(false);
}

void SfxSettingsWidget::addSlider(QString label,int min, int max, QString suffix,int specialValue,QString prefix)
{

    _sliders.append(new SliderSpin(min,max,suffix,prefix));
    _layout->addWidget(new QLabel(label), _layout->rowCount(),0,1,2);
    // Row count will be increased by one after adding the label so we substract 1 to stay on the same line
    _layout->addWidget( _sliders.last() ,_layout->rowCount() - 1 , 2,1,6);

    // need to have this outside the lambda else _slider.size() will return the current size
    int index = _sliders.size() -1;
    connect(_sliders.last(),&SliderSpin::valueChanged,this,[=](int newValue){
            emit sliderValueChanged(index, newValue,specialValue);
        });
    if (specialValue != -1)
        _specialMap.insert(std::pair<int,QWidget*>(specialValue,_sliders.last()));

}


void SfxSettingsWidget::addComboBox(QString label,QStringList values, int enumValue)
{
    _comboBox.append(new QComboBox());
    _layout->addWidget(new QLabel(label), _layout->rowCount(),0,1,2);
    _layout->addWidget(_comboBox.last() ,_layout->rowCount() - 1,2,1,6 );
    for (auto i: values)
        _comboBox.last()->addItem(i);

    // need to cast the connection so that it sends int
    // need to have this outside the lambda else _slider.size() will return the current size
    int index = _comboBox.size() -1;
    connect(_comboBox.last(),static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,[=](int newIndex){
        emit comboBoxValueChanged(index,newIndex,enumValue);
    });

    if (enumValue != -1)
        _specialMap.insert(std::pair<int,QWidget*>(enumValue,_comboBox.last()));

}
void SfxSettingsWidget::setSliderValue(int sliderIndex, int newValue)
{
    // if we are oob we return
    if (sliderIndex > _sliders.size() - 1)
        return;
    this->_sliders.at(sliderIndex)->setValue(newValue);

}

void SfxSettingsWidget::setComboBoxIndex(int comboIndex, int newItemIndex)
{
    if (comboIndex > _comboBox.size() - 1)
        return;
    this->_comboBox.at(comboIndex)->setCurrentIndex(newItemIndex);
}

void SfxSettingsWidget::setValueOfEnumParam(int enumValue, int newValue)
{
    if (enumValue == -1)
        return;

    if (_specialMap.find(enumValue) == _specialMap.end() )
    {
        qDebug() << "Map isn't mapped correctly.";
        return;
    }

    auto* i = dynamic_cast<SliderSpin*>(_specialMap.at(enumValue));

    if ( i != nullptr)
    {
        i->setValue(newValue);
        return;
    }
    auto* j = dynamic_cast<QComboBox*>(_specialMap.at(enumValue));
    if (j !=nullptr)
    {
        j->setCurrentIndex(newValue);
    }
}

void SfxSettingsWidget::setCheckboxState(bool state)
{
    this->_checkbox->setChecked(state);
}
