#include "include/GameSelector.h"


GameSelector::GameSelector(QWidget* parent) : QWidget(parent)
{

    _gameSelectorUi = new Ui::GameSelector();
    _gameSelectorUi->setupUi(this);


    _gameSelectorUi->comboBox->addItem(tr("Default"));

    connect(_gameSelectorUi->comboBox,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), LIDL::Controller::SettingsController::GetInstance(), &LIDL::Controller::SettingsController::ManualGameConfigurationChanged);

    _profileEdit = new ProfileEdit();

    connect(_gameSelectorUi->addBtn, &QToolButton::clicked,  _profileEdit, &ProfileEdit::exec);


}
