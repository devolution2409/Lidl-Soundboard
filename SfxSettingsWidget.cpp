#include "SfxSettingsWidget.h"

SfxSettingsWidget::SfxSettingsWidget(QWidget *parent) : QScrollArea(parent)
{
    //  this->setLineWidth(20);
    _container = new QWidget();
    _layout = new QGridLayout(_container);
    _layout->setSizeConstraint(QLayout::SetMinimumSize);
    this->setWidgetResizable(true);
    this->setMaximumHeight(230);
    this->setWidget(_container);

}

SfxSettingsWidget::SfxSettingsWidget(QString sfxName, LIDL::SFX_TYPE type, bool showCheckmarkAndPresets, QWidget *parent)
    :SfxSettingsWidget(parent)
{
    this->_type = type;
    // adding checkbox
    if (showCheckmarkAndPresets)
    {
        _checkbox = new QCheckBox(QString(tr("Enable %1")).arg(sfxName));
        _checkbox->setCheckable(true);
        _layout->addWidget( _checkbox, 0,0,1,2);
    }
    else
    {
        _layout->addWidget( new QLabel(tr("%1 settings").arg(sfxName)) ,0,0,1,2);
    }
    if (showCheckmarkAndPresets)
    {
        _presetBox = new QComboBox();
        // preset will be read from LIDL::setting controller
        // todo: add the structure of the sound effect in the constructor so that
        // we know where to get the info from :)

        _presetBox->addItem(tr("<No preset selected>"));
        if (type != LIDL::SFX_TYPE::NONE)
            _presetBox->addItems(LIDL::PresetController::GetInstance()->GetExistingPresetsList(type));


        _layout->addWidget( _presetBox,0,2,1,6);


        // surchage every type like this an return the corresponding thing :)
        //QVector<BASS_DX8_DISTORTION>  LIDL::PresetController::GetPreset(type)

        connect(_checkbox,&QCheckBox::toggled, this, [=](bool state){
            for (auto &i: _sliders)
                i->setEnabled(state);
            for (auto &i: _comboBox)
                i->setEnabled(state);
            _presetBox->setEnabled(state);
            emit checkBoxStateChanged(state);
        });

    }
    _layout->setColumnStretch(3,100);
}

QSize SfxSettingsWidget::sizeHint() const
{
    return QSize(this->width(),230);
}
void SfxSettingsWidget::deactivateAll()
{
    for (auto &i: _sliders)
        i->setEnabled(false);
    for (auto &i: _comboBox)
        i->setEnabled(false);
    _presetBox->setEnabled(false);
}

void SfxSettingsWidget::addSpacer()
{
    this->_layout->addItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding),
                           _layout->rowCount(),0,-1,-1);

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
    //_comboBox.last()->lineEdit()->setAlignment(Qt::AlignCenter);
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

    _comboBox.last()->setCurrentIndex(0);
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
        if (newValue > j->count() || newValue < 0)
            j->setCurrentIndex(0);
        else
            j->setCurrentIndex(newValue);
        return;
    }
}

int SfxSettingsWidget::getValueOfEnumParam(int enumValue) const
{
    if (enumValue == -1)
        return -3154;

    if (_specialMap.find(enumValue) == _specialMap.end() )
    {
        qDebug() << "Map isn't mapped correctly.";
        return -3154;
    }
    auto* i = dynamic_cast<SliderSpin*>(_specialMap.at(enumValue));

    if ( i != nullptr)
        return i->value();

    auto* j = dynamic_cast<QComboBox*>(_specialMap.at(enumValue));
    if (j !=nullptr)
        return j->currentIndex();

    return -3154;
}




void SfxSettingsWidget::setCheckboxState(bool state)
{
    this->_checkbox->setChecked(state);
}

std::map<int, int> SfxSettingsWidget::getSFXAsMap() const
{
    std::map<int,int> temp;

    for(std::map<int,QWidget*>::const_iterator it = _specialMap.begin(); it != _specialMap.end(); ++it)
    {
        auto* i = dynamic_cast<SliderSpin*>(it->second);
        if ( i != nullptr)
            temp.insert(std::pair<int,int>(it->first, i->value()));


        auto* j = dynamic_cast<QComboBox*>(it->second);
        if (j !=nullptr)
            temp.insert(std::pair<int,int>(it->first,j->currentIndex() ));

    }
    return temp;
}


//auto SfxSettingsWidget::getSFXAsStructure() const
//{
//        // not using switch because of jump to case label
//        // https://stackoverflow.com/a/5685578
//        if (_type == LIDL::SFX_TYPE::CHORUS)
//        {
//            BASS_DX8_CHORUS temp;


//            auto * i = dynamic_cast<SliderSpin*>(_specialMap.at(static_cast<int>(LIDL::SFX_CHORUS_PARAM::fDelay)));
//            if ( i != nullptr)
//                temp.fDelay = i->value();

//            i = dynamic_cast<SliderSpin*>(_specialMap.at(static_cast<int>(LIDL::SFX_CHORUS_PARAM::fDepth)));
//            if ( i != nullptr)
//                temp.fDepth = i->value();

//            i = dynamic_cast<SliderSpin*>(_specialMap.at(static_cast<int>(LIDL::SFX_CHORUS_PARAM::fFeedback)));
//            if ( i != nullptr)
//                temp.fFeedback = i->value();

//            i = dynamic_cast<SliderSpin*>(_specialMap.at(static_cast<int>(LIDL::SFX_CHORUS_PARAM::fFrequency)));
//            if ( i != nullptr)
//                temp.fFrequency = i->value();

//            i = dynamic_cast<SliderSpin*>(_specialMap.at(static_cast<int>(LIDL::SFX_CHORUS_PARAM::fWetDryMix)));

//            if ( i != nullptr)
//                temp.fWetDryMix = i->value();

//            auto* j = dynamic_cast<QComboBox*>(_specialMap.at( static_cast<int>(LIDL::SFX_CHORUS_PARAM::lPhase)));
//            if (j !=nullptr)
//                temp.lPhase = j->currentIndex();

//            j = dynamic_cast<QComboBox*>(_specialMap.at(static_cast<int>(LIDL::SFX_CHORUS_PARAM::lWaveform)));
//            if (j !=nullptr)
//                temp.lWaveform = j->currentIndex();

//            return temp;
//        }
//        else if (_type == LIDL::SFX_TYPE::COMPRESSOR)
//        {


//        }
//        else if (_type == LIDL::SFX_TYPE::DISTORTION)
//        {

//        }
//        else if (_type == LIDL::SFX_TYPE::ECHO)
//        {

//        }
//        else if (_type == LIDL::SFX_TYPE::FLANGER)
//        {

//        }
//        else if (_type == LIDL::SFX_TYPE::GARGLE)
//        {

//        }

//    return -1;
//}
