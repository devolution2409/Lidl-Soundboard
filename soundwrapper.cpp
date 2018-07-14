#include "soundwrapper.h"

//I
SoundWrapper::SoundWrapper(QObject *parent) : QObject(parent)
{
    this->_player = new CustomPlayer();
    connect(_player,&CustomPlayer::ErrorPlaying,this,[=](QString songName){
         emit ErrorPlaying(songName);
    });

    connect(_player,&CustomPlayer::NowPlaying ,this,[=](QString songName){
        emit NowPlaying(songName);
    });

    connect(_player, CustomPlayer::holdPTT, [=] (int duration){
            emit holdPTTProxy(duration);
    });

}

//II: Constructor when opening a EXP Json file
SoundWrapper::SoundWrapper(QVector<QString> fileList,LIDL::Playback playbackMode,int mainVolume, int vacVolume,int mainOutput, int vacOutput,QObject * parent)
    : SoundWrapper::SoundWrapper(parent)
{
    // Adding sound to the QVector<QFile*>
    for (auto i: fileList)
        this->addSound(i,static_cast<float>(mainVolume/100.0),static_cast<float>(vacVolume/100.0));
    this->_playMode = playbackMode;
    this->_player->SetPlaylist(this->getSoundList());
    this->_player->SetPlaybackMode( this->getPlayMode());

    this->setPlayerMainOutput(mainOutput);
    this->setPlayerVACOutput(vacOutput);
}



//III: Constructor used when opening a LIDLJSON file
// and we editing/adding a souund
SoundWrapper::SoundWrapper(QVector<LIDL::SoundFile *> fileList, LIDL::Playback playbackMode, QKeySequence shortcut, int shortcutVirtualKey,
                           int mainOutput, int vacOutput, QObject *parent)
            : SoundWrapper(parent)
{
    //qDebug() << "WAKANDA FOR EVAH";
    this->_soundList = fileList;
    this->_playMode = playbackMode;
    this->_keySequence = shortcut;
    this->_virtualKey = shortcutVirtualKey;
    this->setPlayerMainOutput(mainOutput);
    this->setPlayerVACOutput(vacOutput);
    this->_player->SetPlaylist(this->getSoundList());
    this->_player->SetPlaybackMode(this->getPlayMode());
}



/********************************************
 *                SLOT                      *
 ********************************************/

void SoundWrapper::Play()
{
    _player->PlayNext();
}

void SoundWrapper::Stop()
{
    // destroy the player, test to see if it stop sounds
    _player->Stop();
   // _player->unHoldPTT();
}


/********************************************
 *                Getters:                  *
 ********************************************/

// Soundlist
QVector<LIDL::SoundFile*> SoundWrapper::getSoundList()
{
   return this->_soundList;
}
// KeySequence/Shorcut
QKeySequence SoundWrapper::getKeySequence()
{
    return this->_keySequence;
}

// playmode
LIDL::Playback SoundWrapper::getPlayMode()
{
    return this->_playMode;
}

QString SoundWrapper::getSoundListAsQString()
{
    QString tmpString;
    // auto &i instead of auto i because the array contains pointers
    // we created exists method in LIDL::SoundFile and it will
    // check for http and https files existing on remote server aswell smiley face
    if (_soundList.size() == 0)
    {
        return QString(tr("No sounds!"));
    }

    for (auto &i: _soundList)
    {
        // if the file doesn't exist we put a warning sign
        if ( !(i->exists()))
        {
            tmpString.append("⚠️");
            emit UnexistantFile();
        }
        tmpString.append(i->fileName());
        tmpString.append("\n");
    }
    // we remove the last \n
    // not needed since the model won't show it forsenKek
    tmpString.remove(tmpString.length()-1,1);
    return tmpString;
}




QList<QStandardItem*> SoundWrapper::getSoundAsItem()
{
    QList<QStandardItem*> tempItem;
    //qDebug()<< modifiedSound->getKeySequence().toString();
    tempItem.append(new QStandardItem(this->getSoundListAsQString()));
    // if we have remote files
    int remoteFiles = 0;
    for (auto &i: this->_soundList)
        if ( i->scheme() =="http" || i->scheme() =="https" || i->scheme() =="ftp")
            remoteFiles++;

    tempItem.append(new QStandardItem(QString::number(remoteFiles) ));
    // Iterates over SFX
    int flags = 0;
    for (auto &i: _soundList)
    {
        flags+= i->getSFX().flags;
    }
    if (flags == 0)
        tempItem.append(new QStandardItem(tr("No SFX!")));
    else
    {
        QString tempString;
        for (auto &i: _soundList)
        {

            if (i->getSFX().flags & LIDL::SFX_TYPE::CHORUS)
                tempString.append(tr("Chorus, "));

            if (i->getSFX().flags & LIDL::SFX_TYPE::DISTORTION)
                tempString.append(tr("Distortion, "));

            if (i->getSFX().flags & LIDL::SFX_TYPE::ECHO)
                tempString.append(tr("Echo, "));

            if (i->getSFX().flags & LIDL::SFX_TYPE::COMPRESSOR)
                 tempString.append(tr("Compressor, "));

            if (i->getSFX().flags & LIDL::SFX_TYPE::FLANGER)
                 tempString.append(tr("Flanger, "));

            if (i->getSFX().flags & LIDL::SFX_TYPE::GARGLE)
                 tempString.append(tr("Gargle, "));


        }
// !: <- my cat did this


        tempString.remove( tempString.length() -2 ,2);
        tempItem.append(new QStandardItem(tempString));
    }

    // If shortcut is empty we write no shortcut    
    if (this->getKeySequence().isEmpty())
        tempItem.append(new QStandardItem("No Shortcut!"));
    else
        tempItem.append(new QStandardItem(this->getKeySequence().toString()));
    QString tmpMode;
    switch(this->getPlayMode())
    {
       case LIDL::Playback::Singleton: tmpMode.append("Singleton"); break;
       case LIDL::Playback::Sequential: tmpMode.append("Sequential"); break;
       case LIDL::Playback::Auto: tmpMode.append("Sequential (Auto)"); break;
       case LIDL::Playback::Cancer: tmpMode.append("Singleton (Cancer)");break;
    }
    tempItem.append(new QStandardItem(tmpMode));

    for (auto &i: tempItem)
        i->setTextAlignment(Qt::AlignVCenter);

   return tempItem;
}



int SoundWrapper::getShortcutVirtualKey()
{
    return _virtualKey;
}

/********************************************
 *                 Setters:                 *
 ********************************************/

void SoundWrapper::OutputDeviceChanged(int index)
{
    //qDebug() << "SLOT: SoundWrapper OutputDeviceChanged output device changed, new is: " << index;
    _player->SetOutputDevice(index);
    //qDebug() << "ouput device changed";
}

void SoundWrapper::VACDeviceChanged(int index)
{
    _player->SetVACDevice(index);
}

// AddSound: Instanciate a new QFile via new and append the pointer
int SoundWrapper::addSound(QString filename,float mainVolume, float vacVolume)
{

    this->_soundList.append(new LIDL::SoundFile(filename,mainVolume,vacVolume));
    return 0;

}


void SoundWrapper::setPlayerMainOutput(int index)
{
    this->_player->SetOutputDevice(index);
}

void SoundWrapper::setPlayerVACOutput(int index)
{
    this->_player->SetVACDevice(index);
}

bool SoundWrapper::checkFileExistence(QString fileName)
{
    QFile temp(fileName);
    return temp.exists();
}

void SoundWrapper::clearShorcut()
{
    QKeySequence empty;
    this->_keySequence.swap(empty);
}

// ILLEGAL IN QT
// ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL
//SoundWrapper::SoundWrapper(SoundWrapper other) : QObject(other)
//{
//    // Egalizing all non-pointers member variable
//    _soundList   = other.getSoundList();
//    _playMode    = other.getPlayMode();
//    _keySequence = other.getKeySequence();
//    _virtualKey  = other.getShortcutVirtualKey();
//    // Declaring a new player
//    this->_player = new CustomPlayer();
//    connect(_player,SIGNAL(ErrorPlaying(QString)),this,SLOT(playerErrorPlaying(QString)));
//    connect(_player,SIGNAL(NowPlaying(QString)),this,SLOT(playerNowPlaying(QString)));

//    this->setPlayerPTTScanCode(other.getPlayerPTTScanCode());
//    this->setPlayerPTTVirtualKey(other.getPlayerPTTVirtualKKey());
//    this->setPlayerMainOutput(other.getMainDevice());
//    this->setPlayerVACOutput(other.getVacDevice());
//    this->_player->SetPlaylist(this->getSoundList());
//    this->_player->SetPlaybackMode(this->getPlayMode());


//}

int SoundWrapper::getMainDevice()
{
    return this->_player->GetOutputDevice();
}

int SoundWrapper::getVacDevice()
{
    return this->_player->GetVACDevice();
}

bool operator==(const SoundWrapper &a, const SoundWrapper &b)
{
    // First of all we compare the size of the vector containing sounds
    // if it's not the same we return
    if ( a._soundList.size() != b._soundList.size() )
        return false;
    // if its the same we compare shorcuts
    if (a._keySequence != b._keySequence)
        return false;
    // than we compare shortcut VK
    if  (a._virtualKey != b._virtualKey)
        return false;

    // If all those test passed we have to iterate through the QVector<LIDL::SoundFile>
    // we return false if the sounds aren't the same
    for (int i =0; i < a._soundList.size();++i)
        if ( a._soundList.at(i) != b._soundList.at(i) )
            return false;

        return true;
}

bool operator!=(const SoundWrapper &a, const SoundWrapper &b)
{
    return !(a==b);
}


