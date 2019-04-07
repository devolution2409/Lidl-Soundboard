#include "include/GameSelector.h"


GameSelector::GameSelector(QWidget* parent) : QWidget(parent)
{

    _gameSelectorUi = new Ui::GameSelector();
    _gameSelectorUi->setupUi(this);


  //  _gameSelectorUi->comboBox->addItem(tr("Default"));

    connect(_gameSelectorUi->comboBox,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), LIDL::Controller::SettingsController::GetInstance(), &LIDL::Controller::SettingsController::ManualGameConfigurationChanged);




    connect(_gameSelectorUi->addBtn, &QToolButton::clicked, this, [=]{
             _profileEdit = new ProfileEdit();

             _profileEdit->exec();
    });

    connect(LIDL::Controller::SettingsController::GetInstance(),&LIDL::Controller::SettingsController::ProfileConfigurationChanged,
            this, &GameSelector::RefreshProfiles);




}

void GameSelector::RefreshProfiles()
{
    QString previous = _gameSelectorUi->comboBox->currentText();
    _gameSelectorUi->comboBox->clear();
    for (auto i: LIDL::Controller::SettingsController::GetInstance()->GetProfiles())
    {
        _gameSelectorUi->comboBox->addItem(i.GetName());
    }
    for (int i = 0; i < _gameSelectorUi->comboBox->count(); i++)
    {
        // if profile wasn't deleted we remain on it
        if (_gameSelectorUi->comboBox->itemText(i) == previous)
        {
            _gameSelectorUi->comboBox->setCurrentIndex(i);
            break;
        }

    }
}
