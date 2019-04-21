#include "include/ProfileEdit.h"

ProfileEdit::ProfileEdit(QWidget *parent) : QDialog(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui = new Ui::ProfilEdit();
    ui->setupUi(this);
    this->setWindowTitle(tr("LIDL Profile Creation"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->nameEdit, &QLineEdit::editingFinished, this, [=]{
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(this->IsFormOk());
    });



    connect(ui->btnAdd,&QToolButton::clicked, this, [=]{
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Select Application"), "", tr("Executable (*.exe)"));

        if (!fileName.isEmpty())
        {
            //forbidding adding the same exe twice
            for(int i = 0; i < ui->listWidget->count(); ++i)
            {
                QListWidgetItem* item = ui->listWidget->item(i);
                if (item->text() == fileName)
                {
                    return;
                }

            }
            // forbidding adding an exe that is already in another profile
            for (auto i: LIDL::Controller::ProfileController::GetInstance()->GetProfiles())
            {
                // if we find a game somewhere
                if ( i->GetGameList().contains(fileName) )
                {
                       QMessageBox::critical(this,"LIDL Error !","This executable is already set in another profile, please remove it before adding it in another profile.");
                       return;
                }

            }

            ui->listWidget->addItem(fileName);
        }
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(this->IsFormOk());


    });

    connect(ui->listWidget, &QListWidget::itemClicked, this, [=](QListWidgetItem* item){
        ui->btnMinus->setEnabled(true);
       // disconnect(ui->btnMinus);
        connect(ui->btnMinus, &QToolButton::clicked, this, [=]{
           delete  item;
           ui->btnMinus->setEnabled(false);
           disconnect(ui->btnMinus);
           ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(this->IsFormOk());

        });

    });

    connect(ui->checkBoxCopySounds,&QCheckBox::clicked,ui->comboBoxProfiles ,&QComboBox::setEnabled);

    //adding them to the list HYPERBRUH
    for(auto i: LIDL::Controller::ProfileController::GetInstance()->GetProfiles())
    {
        ui->comboBoxProfiles->addItem(i->GetName());
    }

    connect( ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=]{

        Profile::Builder builder = Profile::Builder().setName(ui->nameEdit->text());

        for(int i = 0; i < ui->listWidget->count(); ++i)
        {
            QListWidgetItem* item = ui->listWidget->item(i);
            builder.addExe(item->text());
        }


        // need to copy the sound if checkmark && radio is checked

        if (ui->checkBoxCopySounds->isChecked())
        {

                QVector<std::shared_ptr<SoundWrapper>> temp;
                Profile* profile = LIDL::Controller::ProfileController::GetInstance()->GetProfileForName( ui->comboBoxProfiles->currentText());
                // i is a reference to a shared_ptr soundwrapper

                for(auto &i: profile->GetSounds())
                {
                    //A shared_ptr may share ownership of an object while storing a pointer to another object.
                    //get() returns the stored pointer, not the managed pointer.
                    SoundWrapper* NaM = i.get();

                    // need to use a temp variable with the SoundWrapper* type else
                    // it will call the default constructor of SoundWrapper(QObject* parent= nullptr)
                    // despite trying static_cast,dynamic_cast and even reinterpret_cast

                    temp.append(std::make_shared<SoundWrapper>( NaM));
                }
                builder.setSounds(temp);
        }



        LIDL::Controller::ProfileController::GetInstance()->AddProfile( builder.Build());


    });

    connect(this, &QDialog::destroyed , this,[=]{
        qDebug() << "[ProfileEdit] I was destroyed!";
        this->deleteLater();
    });

    //connect(ui->nameEdit, &QLineEdit::keyPressEvent, [=]);
}

ProfileEdit::ProfileEdit(Profile * profile)
{


    this->setAttribute(Qt::WA_DeleteOnClose);
    ui = new Ui::ProfilEdit();
    ui->setupUi(this);
    this->setWindowTitle(tr("LIDL Profile Edition"));


    connect(this, &QDialog::destroyed , this,[=]{
        qDebug() << "[ProfileEdit] I was destroyed!";
        this->deleteLater();
    });
    connect(ui->nameEdit, &QLineEdit::editingFinished, this, [=]{
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(this->IsFormOk());
    });



    ui->nameEdit->setText( profile->GetName() );

    for (auto i: profile->GetGameList())
    {
        ui->listWidget->addItem(i);
    }

    connect(ui->btnAdd,&QToolButton::clicked, this, [=]{
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Select Application"), "", tr("Executable (*.exe)"));

        if (!fileName.isEmpty())
        {
            //forbidding adding the same exe twice
            for(int i = 0; i < ui->listWidget->count(); ++i)
            {
                QListWidgetItem* item = ui->listWidget->item(i);
                if (item->text() == fileName)
                {
                    return;
                }

            }
            // forbidding adding an exe that is already in another profile
            for (auto i: LIDL::Controller::ProfileController::GetInstance()->GetProfiles())
            {
                // if we find a game somewhere
                if ( i->GetGameList().contains(fileName) )
                {
                       QMessageBox::critical(this,"LIDL Error !","This executable is already set in another profile, please remove it before adding it in another profile.");
                       return;
                }

            }

            ui->listWidget->addItem(fileName);
        }
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(this->IsFormOk());


    });

    connect(ui->listWidget, &QListWidget::itemClicked, this, [=](QListWidgetItem* item){
        ui->btnMinus->setEnabled(true);
       // disconnect(ui->btnMinus);
        connect(ui->btnMinus, &QToolButton::clicked, this, [=]{
           delete  item;
           ui->btnMinus->setEnabled(false);
           disconnect(ui->btnMinus);
           ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(this->IsFormOk());

        });

    });

    ui->checkBoxCopySounds->hide();
    ui->comboBoxProfiles->hide();


    connect( ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=]{

        profile->SetName(ui->nameEdit->text());
        QSet<QString> temp;
        for(int i = 0; i < ui->listWidget->count(); ++i)
        {
            QListWidgetItem* item = ui->listWidget->item(i);
            temp.insert(item->text());
        }
        profile->SwapGames(temp);

        //Switching to the new profile name (and, even if name didn't change, it will also trigger the refresh i think
        LIDL::Controller::ProfileController::GetInstance()->AutomaticConfigurationChange(ui->nameEdit->text(),true);

    });


}

bool ProfileEdit::IsFormOk()
{
   // if (ui->listWidget->count() > 0 && !ui->nameEdit->text().isEmpty() )
    if (!ui->nameEdit->text().isEmpty())
    {
        return true;
    }
    return false;
}


