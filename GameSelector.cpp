#include "include/GameSelector.h"


GameSelector::GameSelector(QWidget* parent) : QWidget(parent)
{

    _gameSelectorUi = new Ui::GameSelector();
    _gameSelectorUi->setupUi(this);


  //  _gameSelectorUi->comboBox->addItem(tr("Default"));

    connect(_gameSelectorUi->comboBox,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), LIDL::Controller::ProfileController::GetInstance(), &LIDL::Controller::ProfileController::ManualGameConfigurationChanged);




    connect(_gameSelectorUi->addBtn, &QToolButton::clicked, this, [=]{
             _profileEdit = new ProfileEdit();

             _profileEdit->exec();
    });

    connect(LIDL::Controller::ProfileController::GetInstance(),&LIDL::Controller::ProfileController::ProfileConfigurationChanged,
            this, &GameSelector::RefreshProfiles);


    // delete active profile
    connect(_gameSelectorUi->deleteBtn, &QToolButton::clicked, this, [=]{
        // don't remove default profile DansW
        if (LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetName() == "Default")
            return;

        Profile* profileToDelete =  LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile();

        // revert to default (this will switch active profile)
        LIDL::Controller::ProfileController::GetInstance()->ManualGameConfigurationChanged("Default");

        LIDL::Controller::ProfileController::GetInstance()->DeleteProfile(profileToDelete);

        for (auto &i: LIDL::Controller::ProfileController::GetInstance()->GetProfiles())
        {
            qDebug() << "Profile:" << i->GetName();
        }


        this->RefreshProfiles();

            });

}

void GameSelector::RefreshProfiles()
{
    qDebug() << "[RefreshProfiles]";

    QString previous = LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetName();
    _gameSelectorUi->comboBox->clear();
    for (auto &i: LIDL::Controller::ProfileController::GetInstance()->GetProfiles())
    {
        qDebug() << "weeeeeeeeeeeee";
        _gameSelectorUi->comboBox->addItem(i->GetName());
    }
    for (int i = 0; i < _gameSelectorUi->comboBox->count(); i++)
    {
        // if profile wasn't deleted we remain on it
        if (_gameSelectorUi->comboBox->itemText(i) == previous)
        {
            _gameSelectorUi->comboBox->setCurrentIndex(i);
            break;
        }
        else
        {
          qDebug() << "Please implement reverting to default profile in GameSelector.cpp line 50";
        }

    }
}
