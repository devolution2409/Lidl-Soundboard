#include "SettingsController.h"


// Need to initialize the static to nullptr here.
namespace LIDL{
SettingsController* SettingsController::self = nullptr;

SettingsController::SettingsController()
{
    // Settings default value in case
        // Volume
        defaultMainVolume       = 100;
        defaultVacVolume        = 100;
        // Folder for song = get app location
        defaultSoundboardFolder =  qApp->applicationDirPath();
        defaultSoundsFolder     =  qApp->applicationDirPath();
        // Default file count
        recentFileCount         = 5;
        this->fileName = "lidlsettings.json";
}


SettingsController* SettingsController::GetInstance()
{
    if (self == nullptr)
        self = new SettingsController();
    // else if it is different we return it
    return self;

}

QString SettingsController::GetDefaultSoundboardFolder() const
{
    return defaultSoundboardFolder;
}

QString SettingsController::GetDefaultSoundFolder() const
{
    return defaultSoundsFolder;
}

int SettingsController::GetDefaultMainVolume() const
{
    return defaultMainVolume;
}

int SettingsController::GetDefaultVacVolume() const
{
    return defaultVacVolume;
}


int SettingsController::GetRecentFilesCount() const
{
    return recentFileCount;
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
    connect(ui->sliderVAC,SIGNAL(valueChanged(int)),this,SLOT(SetDefaultVacVolume(int)));
    connect(ui->sliderMain,SIGNAL(valueChanged(int)),this,SLOT(SetDefaultMainVolume(int)));

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
    ui->sliderMain->setValue(defaultMainVolume);
    ui->sliderVAC->setValue(defaultVacVolume);
    ui->fileCount->setValue(recentFileCount);

    connect(ui->buttonBox,QDialogButtonBox::accepted,
            [=]{
                    defaultSoundboardFolder = ui->folderLidlEdit->text();
                    defaultSoundsFolder     = ui->folderSoundEdit->text();
                    recentFileCount         = ui->fileCount->value();
                    defaultMainVolume       = ui->sliderMain->value();
                    defaultVacVolume        = ui->sliderVAC->value();
                    widget->close();
                    this->SaveSettings();
                    emit SettingsChanged();
                    });

    connect(ui->buttonBox,QDialogButtonBox::rejected,widget,QDialog::close);

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



bool SettingsController::OpenSettings()
{
    QFile file(this->fileName);
    // If settings file exist we parse it.
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)   )
        {
            QString jsonAsString = file.readAll();
            //QJsonParseError error;
            QJsonDocument cdOMEGALUL = QJsonDocument::fromJson(jsonAsString.toUtf8());
            if (cdOMEGALUL.isNull())
            {
                file.close();
                // Return false to prevent main ui from fetching recent data
                return false;
                // todo: deal with error message
            }
            // else we parse it
            else
            {
              QJsonObject json = cdOMEGALUL.object();
              if (json.contains("Default Locations"))
              {
                  QJsonObject locations = json.value("Default Locations").toObject();
                  if (locations.contains("Default Sounds Path"))
                      this->defaultSoundsFolder = locations.value("Default Sounds Path").toString();
                  if (locations.contains("Defaut Soundboards (.lidljson) Path"))
                      this->defaultSoundboardFolder = locations.value("Defaut Soundboards (.lidljson) Path").toString();
              }
              if (json.contains("Default Volumes"))
              {
                  QJsonObject volumes = json.value("Default Volumes").toObject();
                  if (volumes.contains("Main Volume"))
                      defaultMainVolume = volumes.value("Main Volume").toInt();
                  if (volumes.contains("VAC Volume"))
                      defaultVacVolume  = volumes.value("VAC Volume").toInt();
              }
              if (json.contains("Recent Files Info"))
              {
                  QJsonObject fileObject =  json.value("Recent Files Info").toObject();
                  if (fileObject.contains("Display Count"))
                      recentFileCount = fileObject.value("Display Count").toInt();
                  if (fileObject.contains("Files"))
                  {
                      //qDebug() << "!????";
                      QJsonObject files = fileObject.value("Files").toObject();
                      // have to use standard iterator and not auto cause it doesn't work pajaL
                      // if we iterate from beginning to end, the first element
                      // of the index
                      // is the last opened soundboard
                      // else it is the other way around
                      for (QJsonObject::iterator it = files.begin(); it!= files.end(); it++)
                      {
                          // if the file exists we push it into the array
                          if (QFileInfo(it.key()).exists())
                            recentFiles.push_back(QFileInfo(it.key()));
                          //qDebug() <<  "file name:" << it.key() << "index:" <<it.value().toInt();
                      }

                  }
              }
            }
        }
        //qDebug() <<"size here:" << recentFiles.size();
        fileAlreadyExisted = true;
        return true;
    }
    // else we create it
    // with default values
    else
    {
        this->SaveSettings();
        fileAlreadyExisted = false;
        return false;
    }
    emit RecentFilesChanged();
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

    QJsonObject fileArray;
    int count = 0;
    for ( auto i: this->recentFiles )
    {
        // oldest file will have the littlest index (forsenE)
        fileArray.insert(i.absoluteFilePath(), count++);
    }


    files.insert("Files",fileArray);


    saveFile.insert("Default Volumes",volume);
    saveFile.insert("Default Locations",location);
    saveFile.insert("Recent Files Info",files);

    QJsonDocument cdOMEGALUL(saveFile);
    QString boyishGiggles(cdOMEGALUL.toJson(QJsonDocument::Indented));
    QFile file(this->fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << boyishGiggles.toUtf8();
        file.close();
    }

}



void SettingsController::SetRecentFileCount(int count)
{
    recentFileCount = count;
}

void SettingsController::addFile(QFileInfo fileInfo)
{
    // Check if file isn't contained in the vector already and than we append it if not
    auto result =  std::find(recentFiles.begin(),recentFiles.end(),fileInfo);
    // if file wasn't already there we need to append it
    if (result == recentFiles.end())
    {
        // if size becomes (somehow) larger than 10
        // (in case some lidl genius hack it inside the json
        // we delete the elements anyway
        while (recentFiles.size() > 10)
            recentFiles.pop_front();
        // append newest file
        recentFiles.push_back(fileInfo);
    }
    // else we need to put it first forsenL
    else
    {
        // we iter from the result to the first element
        for ( std::deque<QFileInfo>::reverse_iterator i(result); i != recentFiles.rend(); ++i )
            if ( std::prev(i,1) != recentFiles.rend() )
                std::iter_swap(i,std::prev(i,1) );


    }
    emit RecentFilesChanged();
}

QString SettingsController::GetLastOpenedSoundboard()
{
    // need to access the element BEFORE end one.
    // using reverse iterator to have VALID iterator
    // on last element. (could also use --(recentFiles.end());
    if (recentFiles.size() > 0)
        return recentFiles.rbegin()->filePath();
    return QString("");
}

std::deque<QFileInfo> SettingsController::GetRecentFiles()
{
    return this->recentFiles;
}

// We save the state here and than we can use it to be compared when
// we need to know if soundboard was modified
void SettingsController::SaveState(QVector<SoundWrapper *> sounds, CustomShortcutEdit * pttEdit, CustomShortcutEdit * stopEdit)
{
    savedSounds = sounds;
    // QKeySequence are copy disabled so we need to do it by hand forsenT
    savedPTT.setKeySequence(    pttEdit->keySequence()        );
    savedPTT.setScanCode( pttEdit->getScanCode()       );
    savedPTT.setVirtualKey(pttEdit->getVirtualKey() );

    savedStop.setKeySequence(stopEdit->keySequence());
    savedStop.setScanCode(stopEdit->getScanCode()  );
    savedStop.setVirtualKey(stopEdit->getVirtualKey());
}

bool SettingsController::IsThisFirstTimeUser()
{
    return !(this->fileAlreadyExisted);
}

bool SettingsController::SaveIsDifferentFrom( QVector<SoundWrapper*> sounds, CustomShortcutEdit* pttEdit, CustomShortcutEdit* stopEdit  )
{
    //    comparing ptt
    if (    (pttEdit->keySequence()   != this->savedPTT.keySequence()) ||
            (pttEdit->getScanCode()   != this->savedPTT.getScanCode()) ||
            (pttEdit->getVirtualKey() != this->savedPTT.getVirtualKey()))
        return true;
    // comparing stop
    if (    (stopEdit->keySequence()   != this->savedStop.keySequence()) ||
            (stopEdit->getScanCode()   != this->savedStop.getScanCode()) ||
            (stopEdit->getVirtualKey() != this->savedStop.getVirtualKey()))
        return true;
    // comparing size of Qvectors
    if (sounds.size() != this->savedSounds.size())
        return true;

    else  // comparing wrappers using overloaded operator==
        for (int i = 0; i < savedSounds.size(); ++i)
            // need to call the operator== while dereferencing the pointers forsenE
            if ( *(savedSounds.at(i)) != *(sounds.at(i)))
                return true;

   return false;
}



} // end namespace

