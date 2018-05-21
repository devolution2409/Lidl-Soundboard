#include "SettingsController.h"


// Need to initialize the static to nullptr here.
namespace LIDL{
SettingsController* SettingsController::self = nullptr;

SettingsController::SettingsController()
{
    // Settings default value in case
        // Volume
        defaultMainVolume       = 1;
        defaultVacVolume        = 1;
        // Folder for song = get app location
        defaultSoundboardFolder =  qApp->applicationDirPath();
        defaultSoundsFolder     =  qApp->applicationDirPath();
        // Default file count
        recentFileCount         = 5;
}


SettingsController* SettingsController::GetInstance()
{
    if (self == nullptr)
        self = new SettingsController();
    // else if it is different we return it
    return self;

}



int SettingsController::GetDefaultMainVolume()
{
    return defaultMainVolume;
}

int SettingsController::GetDefaultVacVolume()
{
    return defaultVacVolume;
}


void SettingsController::SetDefaultMainVolume(int newVolume)
{
    this->defaultMainVolume = newVolume;
}

void SettingsController::SetDefaultVacVolume(int newVolume)
{
    this->defaultVacVolume  = newVolume;
}

void SettingsController::ShowSettingsWindow()
{

    QDialog *widget = new QDialog();
    widget->setModal(true);
    widget->setFixedSize(400,300);
    ui = new Ui::Settings;
    ui->setupUi(widget);
    widget->setWindowTitle("LIDL Settings Editor");
    widget->setWindowIcon(QIcon(":/icon/resources/forsenAim.png"));

    // connect main slider to spinbox and vice versa
    connect(ui->sliderMain,SIGNAL(valueChanged(int)),ui->spinMain,SLOT(setValue(int)));
    connect(ui->spinMain,SIGNAL(valueChanged(int)),ui->sliderMain,SLOT(setValue(int)));
    // connect vac slider to vac spinbox and vice versa
    connect(ui->sliderVAC,SIGNAL(valueChanged(int)),ui->spinVAC,SLOT(setValue(int)));
    connect(ui->spinVAC,SIGNAL(valueChanged(int)),ui->sliderVAC,SLOT(setValue(int)));
    // connecting either slider or spin box to set the value here
    connect(ui->sliderVAC,SIGNAL(valueChanged(int)),this,SLOT(SetDefaultMainVolume(int)));
    connect(ui->sliderMain,SIGNAL(valueChanged(int)),this,SLOT(SetDefaultVacVolume(int)));

    // connecting the folder signals
    connect(ui->folderLidlBrowse,SIGNAL(clicked()),this,SLOT(buttonBrowseLidl()));
    connect(ui->folderSoundBrowse,SIGNAL(clicked()),this,SLOT(buttonBrowseSound()));

    // Lambda expression to connect with signal
    // WITH CAST ON THE SENDING CLASS FUNCTION POINTER HYPERWUTFACE
    // Keeping it in comments for the swag cause we need setter anyway
    /* connect(ui->fileCount,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int newValue){this->recentFileNumber = newValue;} ); */
    connect(ui->fileCount, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, SettingsController::SetRecentFileCount);

    // Settings all the value to which we already have stored
    ui->folderLidlEdit ->setText(defaultSoundboardFolder);
    ui->folderSoundEdit ->setText(defaultSoundsFolder);
    ui->sliderMain->setValue(defaultMainVolume * 100);
    ui->sliderVAC->setValue(defaultVacVolume * 100);
    ui->fileCount->setValue(recentFileCount);

    widget->show();
}

void SettingsController::buttonBrowseLidl()
{
    QString folderName = QFileDialog::getExistingDirectory(nullptr, tr("Open Directory"),
                                                                        "/home",
                                                                        QFileDialog::ShowDirsOnly
                                                                        | QFileDialog::DontResolveSymlinks);
    // if the fileName isn't empty, the user selected a file, so we add it.
    if (!folderName.isEmpty())
    {
        if (ui != nullptr)
            ui->folderLidlEdit->setText(folderName);
    }
    else
        qDebug() << "No folder selected";
}


void SettingsController::buttonBrowseSound()
{
    QString folderName = QFileDialog::getExistingDirectory(nullptr, tr("Open Directory"),
                                                                        "/home",
                                                                        QFileDialog::ShowDirsOnly
                                                                        | QFileDialog::DontResolveSymlinks);
    // if the fileName isn't empty, the user selected a file, so we add it.
    if (!folderName.isEmpty())
    {
        if (ui != nullptr)
            ui->folderSoundEdit->setText(folderName);

    }
    else
        qDebug() << "No folder selected";
}



void SettingsController::OpenSettings()
{
    QFile file("./lidlsettings.lidljson");
    // If settings file exist we parse it.
    if (file.exists())
    {
        qDebug() << "yeeeeha";
    }

    // else we create it
    // with default values
    else
        this->SaveSettings();
}

void SettingsController::SaveSettings()
{
    // creating the new JSon object
    QJsonObject saveFile;
    // Volumes
    QJsonObject volume;
    volume.insert("Main Volume", defaultMainVolume);
    volume.insert("VAC Volume",  defaultVacVolume);

    // Location
    QJsonObject location;
    location.insert("Defaut Soundboards (.lidljson) Path",defaultSoundboardFolder);
    location.insert("Default Sounds Path",defaultSoundsFolder);

    QJsonObject files;
    files.insert("Display Count", recentFileCount);

    QJsonArray fileArray;
    files.insert("Files",fileArray);

    saveFile.insert("Default Volumes",volume);
    saveFile.insert("Default Locations",location);
    saveFile.insert("Recent Files Info",files);

    QJsonDocument cdOMEGALUL(saveFile);
    QString boyishGiggles(cdOMEGALUL.toJson(QJsonDocument::Indented));
    QFile file("./lidlsettings.lidljson");

    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << boyishGiggles.toUtf8();
        file.close();
    }



}

//void SettingsController::SetDefaultValues()
//{
//    // Volume
//    defaultMainVolume       = 1;
//    defaultVacVolume        = 1;
//    // Folder for song = get app location
//    defaultSoundboardFolder =  qApp->applicationDirPath();
//    defaultSoundsFolder     =  qApp->applicationDirPath();
//    // Default file count
//    recentFileCount         = 5;
//}

void SettingsController::SetRecentFileCount(int count)
{
    recentFileCount = count;
}

} // end namespace
