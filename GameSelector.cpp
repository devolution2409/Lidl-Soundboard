#include "include/GameSelector.h"


GameSelector::GameSelector(QWidget* parent) : QWidget(parent)
{

    _gameSelectorUi = new Ui::GameSelector();
    _gameSelectorUi->setupUi(this);




  //  _gameSelectorUi->comboBox->addItem(tr("Default"));

    connect(_gameSelectorUi->comboBox,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            LIDL::Controller::ProfileController::GetInstance(), &LIDL::Controller::ProfileController::ManualGameConfigurationChanged);




    connect(_gameSelectorUi->addBtn, &QToolButton::clicked, this, [=]{
             _profileEdit = new ProfileEdit();

             _profileEdit->exec();
    });

    connect(LIDL::Controller::ProfileController::GetInstance(),&LIDL::Controller::ProfileController::ProfileConfigurationChanged,
            this, &GameSelector::RefreshProfiles);


    // delete active profile
    connect(_gameSelectorUi->deleteBtn, &QToolButton::clicked, this, [=]{
        if (LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile() == nullptr ||
                LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetName() =="Default")
        return;

       LIDL::Controller::ProfileController::GetInstance()->DeleteActiveProfile();
       this->RefreshProfiles();

            });

}

void GameSelector::RefreshProfiles()
{
    _gameSelectorUi->comboBox->blockSignals(true);
    qDebug() << "[GameSelector::RefreshProfiles()] started";

    QString previous = LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetName();
    _gameSelectorUi->comboBox->clear();
    qDebug() << "number of profiles:" << LIDL::Controller::ProfileController::GetInstance()->GetProfiles().size();
    for (auto &i: LIDL::Controller::ProfileController::GetInstance()->GetProfiles())
    {
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
    }



    _gameSelectorUi->comboBox->blockSignals(false);
    // we need to force SwitchToProfile on the soundboard main ui now
    emit RefreshWrappers( LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetSounds());

      qDebug() << "[GameSelector::RefreshProfiles()] ended nam";

}
