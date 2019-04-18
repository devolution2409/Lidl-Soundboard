#include "SettingsController.h"


// Need to initialize the static to nullptr here.
namespace LIDL{
namespace Controller{
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

        dragAndDropSeveralWrappers = true;

        _showFlags  = LIDL::SHOW_SETTINGS::NO_SETTINGS ;
        // we must process events except when a auto-hold ptt command has just been issued.
        _eventProcessing = true;
        this->fileName = "lidlsettings.json";
        connect(&_activePttTimer,&QTimer::timeout, [=]{
                this->unHoldPTT();});

        _isEditing = false;
        _activePttVitualKey = -1;
        _activePttScanCode =-1;

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
            this, &SettingsController::SetRecentFileCount);

    // Settings all the value to which we already have stored
    ui->folderLidlEdit ->setText(defaultSoundboardFolder);
    ui->folderSoundEdit ->setText(defaultSoundsFolder);
    ui->sliderMain->setValue(defaultMainVolume);
    ui->sliderVAC->setValue(defaultVacVolume);
    ui->fileCount->setValue(recentFileCount);
    ui->radioOne->setChecked(dragAndDropSeveralWrappers);
//    bool wrapperBehavior = true; // default


//    connect(ui->radioOne, QRadioButton::clicked, [=]{
//        wrapperBehavior = false;
//    } );

//    connect(ui->radioSeveral, QRadioButton::clicked, [=]{
//        wrapperBehavior = true;
//    } )



    connect(ui->buttonBox,&QDialogButtonBox::accepted,
            [=]{
                    defaultSoundboardFolder = ui->folderLidlEdit->text();
                    defaultSoundsFolder     = ui->folderSoundEdit->text();
                    recentFileCount         = ui->fileCount->value();
                    defaultMainVolume       = ui->sliderMain->value();
                    defaultVacVolume        = ui->sliderVAC->value();
                    dragAndDropSeveralWrappers = ui->radioOne->isChecked();
                    widget->close();
                    this->SaveSettings();
                    emit SettingsChanged();
                    });

    connect(ui->buttonBox,&QDialogButtonBox::rejected,widget,&QDialog::close);

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
                          if (QFileInfo(it.value().toString()).exists())
                            recentFiles.push_back(QFileInfo(it.value().toString()));;
                      }

                  }
              }
              if (json.contains("Drag And Drop Mode"))
                  this->dragAndDropSeveralWrappers = json.value("Drag And Drop Mode").toBool();
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
        emit RecentFilesChanged();
        return false;
    }
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

    // Most recent file has the smallest index
    for ( auto i: this->recentFiles )
        fileArray.insert(QString::number(count++),i.absoluteFilePath());



    files.insert("Files",fileArray);

    saveFile.insert("Drag And Drop Mode",dragAndDropSeveralWrappers);
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

void SettingsController::addFileToRecent(QFileInfo fileInfo)
{
    // Check if file isn't contained in the vector already and then we append it if not
    auto result =  std::find(recentFiles.begin(),recentFiles.end(),fileInfo);

    // We want the deck to be 1 2 3 4 5 6
    // and not 6 5 4 3 2 1
    // if file wasn't already there we need to prepend it
    if (result == recentFiles.end())
    {
        // if size becomes (somehow) larger than 10
        // (in case some lidl genius hack it inside the json
        // we delete the elements anyway
        while (recentFiles.size() > 10)
            recentFiles.pop_back();
        // append new file to the END of the deck
        recentFiles.push_front(fileInfo);
    }
    // else we need to put it first forsenL
    else
    {
        // we iter from the result to the first element
        // and we swap element one by one
        int nigg = 0;
        for ( std::deque<QFileInfo>::reverse_iterator i(result); i != recentFiles.rend(); ++i )
        {
            if ( std::prev(i,1) != recentFiles.rend() )
                std::iter_swap(i,std::prev(i,1) );
            qDebug() << "iteration :"  << nigg++ << " deck is:  ";
            for (unsigned int j = 0; j < recentFiles.size(); j++)
                 qDebug() << recentFiles[j].fileName();

        }
        // eShrug
        while (recentFiles.size() > 10)
            recentFiles.pop_back();
    }
    emit RecentFilesChanged();
}


QString SettingsController::GetLastOpenedSoundboard()
{
    // need to access the element BEFORE end one.
    // using reverse iterator to have VALID iterator
    // on last element. (could also use --(recentFiles.end());
    if (recentFiles.size() > 0)
        return recentFiles.begin()->filePath();
    return QString("");
}

std::deque<QFileInfo> SettingsController::GetRecentFiles()
{
    return this->recentFiles;
}



bool SettingsController::IsThisFirstTimeUser()
{
    return !(this->fileAlreadyExisted);
}




void SettingsController::holdPTT(int duration)
{
    int vk = LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetPttVirtualKey();
    int sc = LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetPttScanCode();
    if ( vk == -1 || sc == -1 )
           return;

    // If a timer is already running we compare remaining time
    // and new duration, if remaining < new, we need to reschedule the stop
    if ( _activePttTimer.remainingTime() < duration )
    {
        qDebug() << "blblbl";
        this->_eventProcessing = false;

           qDebug() << "[SettingsController::holdPTT] attempting to hold ptt";
        keybd_event(vk, sc,KEYEVENTF_EXTENDEDKEY, 0);
        // works with 1ms for some reason OMEGALUL
        QTimer::singleShot(1,Qt::PreciseTimer, [=]{
                this->_eventProcessing = true;
            });
        //this->_eventProcessing = true;
        _activePttTimer.stop();
        _activePttTimer.start(duration);
    }
}

bool SettingsController::getEventProcessing() const
{
    return this->_eventProcessing;
}
void SettingsController::unHoldPTT()
{
    qDebug() << "[SettingController::unHoldPTT()] started";
    //qDebug() << "unholding ptt here";
    // Unpressing the key physically
    keybd_event(LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetPttVirtualKey(),
                 LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetPttScanCode(),KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    // stopping the timer else PTT will be unhold on each tick forsenT
    _activePttTimer.stop();
    qDebug() << "[SettingController::unHoldPTT()] ended nam";
}




void SettingsController::addShowFlag(LIDL::SHOW_SETTINGS addedFlag)
{
    this->_showFlags |= addedFlag;
}

void SettingsController::removeShowFlag(LIDL::SHOW_SETTINGS removedFlag)
{
    this->_showFlags &= ~removedFlag;
}

bool SettingsController::checkShowFlags(LIDL::SHOW_SETTINGS checkedFlag) const
{
    return _showFlags & checkedFlag;
}
LIDL::SHOW_SETTINGS SettingsController::getShowFlags() const
{
    return _showFlags;
}

QStringList SettingsController::GetSupportedMimeTypes() const
{
    QStringList mimes;
    mimes << "audio/wav"   << "audio/xwav" << "audio/x-wav" // wav
                           << "audio/mpeg3" << "audio/x-mpeg-3" <<"audio/mpeg" //mp3
                           << "audio/ogg"  //ogg
                           << "audio/flac"; // flac
    return mimes;
}



bool SettingsController::GetDragAndDropSeveralWrappers() const
{
    return this->dragAndDropSeveralWrappers;
}

bool SettingsController::isEditing() const
{
    return this->_isEditing;
}

void SettingsController::setEditing(bool newState)
{
    this->_isEditing = newState;
}




} // end namespace controller
} // end namespace lidl
