#include "include/ProfileEdit.h"

ProfileEdit::ProfileEdit(QWidget *parent) : QDialog(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui = new Ui::ProfilEdit();
    ui->setupUi(this);
    this->setWindowTitle(tr("Profile Edition"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->nameEdit, &QLineEdit::editingFinished, this, [=]{
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(this->IsFormOk());
    });

    connect(ui->checkBoxCopySounds,&QCheckBox::clicked,ui->comboBoxProfiles ,&QComboBox::setEnabled);

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
            for (auto i: LIDL::Controller::SettingsController::GetInstance()->GetProfiles())
            {
                // if we find a game somewhere
                if ( i.GetGameList().contains(fileName) )
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

    //adding them to the list HYPERBRUH
    for(auto i: LIDL::Controller::SettingsController::GetInstance()->GetProfiles())
    {
        ui->comboBoxProfiles->addItem(i.GetName());
    }

    connect( ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=]{

        Profile::Builder builder = Profile::Builder().setName(ui->nameEdit->text());

        for(int i = 0; i < ui->listWidget->count(); ++i)
        {
            QListWidgetItem* item = ui->listWidget->item(i);
            builder.addExe(item->text());
        }

        LIDL::Controller::SettingsController::GetInstance()->AddProfile( builder.Build());



    });

}

bool ProfileEdit::IsFormOk()
{
    if (ui->listWidget->count() > 0 && !ui->nameEdit->text().isEmpty() )
    {
        return true;
    }
    return false;
}


