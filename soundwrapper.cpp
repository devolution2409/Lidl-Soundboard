#include "soundwrapper.h"

//I
SoundWrapper::SoundWrapper(QObject *parent) : QObject(parent)
{
    this->_player = new CustomPlayer();
    connect(_player,SIGNAL(ErrorPlaying(QString)),this,SLOT(playerErrorPlaying(QString)));
    connect(_player,SIGNAL(NowPlaying(QString)),this,SLOT(playerNowPlaying(QString)));
}

//V: Constructor when opening a EXP Json file
SoundWrapper::SoundWrapper(QVector<QString> fileList,LIDL::Playback playbackMode,int mainVolume, int vacVolume,int mainOutput, int vacOutput,QObject * parent)
    : SoundWrapper::SoundWrapper(parent)
{
    qDebug() << "forsen1" << mainVolume << vacVolume;
    // Adding sound to the QVector<QFile*>
    for (auto i: fileList)
        this->addSound(i,static_cast<float>(mainVolume/100.0),static_cast<float>(vacVolume/100.0));
    this->setPlayMode(playbackMode);
    this->_player->SetPlaylist(this->getSoundList());
    this->_player->SetPlaybackMode( this->getPlayMode());

    this->setPlayerMainOutput(mainOutput);
    this->setPlayerVACOutput(vacOutput);
}

//II: Constructor to be used in the add sound window
SoundWrapper::SoundWrapper(CustomListWidget *soundList, LIDL::Playback playbackMode, QKeySequence * shortcut, int virtualKey, QObject * parent)
    :SoundWrapper::SoundWrapper(parent)
{
    _virtualKey  = virtualKey;
    //qDebug() << "ZULULWARRIOR";
    for(int row = 0; row < soundList->count(); row++)
    {
             // get a pointer on the list item and fetches its text
             CustomListWidgetItem *item = dynamic_cast<CustomListWidgetItem*>(soundList->item(row));
             if (item == nullptr)
                 return;
             this->addSound(item->text(), item->getMainVolume(),item->getVacVolume());
    }

    this->setPlayMode(playbackMode);
    this->setKeySequence(*shortcut);

   this->_player->SetPlaylist(this->getSoundList());
   this->_player->SetPlaybackMode(this->getPlayMode());
}



//IV: Constructor used when opening a LIDLJSON file
SoundWrapper::SoundWrapper(QVector<LIDL::SoundFile *> fileList, LIDL::Playback playbackMode, QKeySequence shortcut, int shortcutVirtualKey,
                           int mainOutput, int vacOutput, int pttVK, int pttSC, QObject *parent)
            : SoundWrapper(parent)
{

    this->_soundList = fileList;
    this->_playMode = playbackMode;
    this->setKeySequence(shortcut);
    this->_virtualKey = shortcutVirtualKey;
    this->setPlayerPTTScanCode(pttSC);
    this->setPlayerPTTVirtualKey(pttVK);
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
    QFileInfo fileInfo;
    QString tmpString;
    // auto &i instead of auto i because the array contains pointers
    for (auto &i: _soundList)
    {
        fileInfo.setFile(*i);
        // if the file doesn't exist we put a warning sign
        if (!fileInfo.exists())
        {
            tmpString.append("⚠️");
            emit UnexistantFile();
        }
        tmpString.append(fileInfo.fileName());
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

void SoundWrapper::PTTScanCodeChanged(int scanCode)
{
    _player->SetPTTScanCode(scanCode);
}

void SoundWrapper::PTTVirtualKeyChanged(int key)
{
    _player->SetPTTVirtualKey(key);
}



// AddSound: Instanciate a new QFile via new and append the pointer
// removeSound need to free the memory, and the destructor aswell
int SoundWrapper::addSound(QString filename,float mainVolume, float vacVolume)
{

    this->_soundList.append(new LIDL::SoundFile(filename,mainVolume,vacVolume));
    return 0;

}

// removeSoundAt: delete the pointer (free memory) and then delete the
// actual slot in the vector
int SoundWrapper::removeSoundAt(int index)
{
    delete this->_soundList.at(index);
    this->_soundList.removeAt(index);
    return 0;
}

int SoundWrapper::setKeySequence(QKeySequence sequence)
{
    this->_keySequence = sequence;
    return 0;
}

int SoundWrapper::setPlayMode(LIDL::Playback playmode)
{
    this->_playMode = playmode;
    return 0;
}

// SETTERS for the players, need when constructing new objects forsenT
void SoundWrapper::setPlayerPTTScanCode(int pttScanCode)
{
    this->_player->SetPTTScanCode(pttScanCode);
}
void SoundWrapper::setPlayerPTTVirtualKey(int vKey)
{
    this->_player->SetPTTVirtualKey(vKey);
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


void SoundWrapper::playerNowPlaying(QString songName)
{
    emit NowPlaying(songName);
}

void SoundWrapper::playerErrorPlaying(QString songName)
{
    emit ErrorPlaying(songName);
}


void SoundWrapper::clearShorcut()
{
    QKeySequence empty;
    this->_keySequence.swap(empty);
    this->setPlayerPTTScanCode(-1);
    this->setPlayerPTTVirtualKey(-1);
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


int  SoundWrapper::getPlayerPTTScanCode()
{
    return this->_player->GetPTTScanCode();
}

int  SoundWrapper::getPlayerPTTVirtualKey()
{
      return this->_player->GetPTTVirtualKey();
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
