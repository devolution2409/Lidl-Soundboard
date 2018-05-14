#include "soundwrapper.h"

//I
SoundWrapper::SoundWrapper(QObject *parent) : QObject(parent)
{
    this->_player = new CustomPlayer();
    connect(_player,SIGNAL(ErrorPlaying(QString)),this,SLOT(playerErrorPlaying(QString)));
    connect(_player,SIGNAL(NowPlaying(QString)),this,SLOT(playerNowPlaying(QString)));
}

SoundWrapper::SoundWrapper(QVector<QString> fileList,LIDL::Playback playbackMode,int mainOutput, int vacOutput,QObject * parent)
    : SoundWrapper::SoundWrapper(parent)
{
    // Adding sound to the QVector<QFile*>
    for (auto i: fileList)
        this->addSound(i);
    this->setPlayMode(playbackMode);
    this->_player->SetPlaylist(this->getSoundList());
    this->_player->SetPlaybackMode( this->getPlayMode());

    this->setPlayerMainOutput(mainOutput);
    this->setPlayerVACOutput(vacOutput);
}

//II: Constructor to be used in the add sound window
SoundWrapper::SoundWrapper(QListWidget* soundList, LIDL::Playback playbackMode,QKeySequence * shortcut, QObject * parent)
    :SoundWrapper::SoundWrapper(parent)
{
    for(int row = 0; row < soundList->count(); row++)
    {
             // get a pointer on the list item and fetches its text
             QListWidgetItem *item = soundList->item(row);
          //   qDebug() << item->text();
             this->addSound(item->text());
    }
    //qDebug() << shortcut->toString();
    this->setPlayMode(playbackMode);
    this->setKeySequence(*shortcut);
     //qDebug() << this->_playMode;
     //qDebug() << this->_keySequence.toString();

   this->_player->SetPlaylist(this->getSoundList());
   this->_player->SetPlaybackMode(this->getPlayMode());
}


//III: Do we really need II again?
SoundWrapper::SoundWrapper(QListWidget* soundList, LIDL::Playback playbackMode,QKeySequence * shortcut,int virtualKey, QObject * parent)
    : SoundWrapper(soundList, playbackMode,shortcut,parent)
{
   _virtualKey  = virtualKey;
}

//IV: Constructor used when opening a file
SoundWrapper::SoundWrapper(QVector<QString> fileList, LIDL::Playback playbackMode, QKeySequence shortcut, int shortcutVirtualKey,
                           int mainOutput, int vacOutput, int pttVK, int pttSC, QObject *parent)
            : SoundWrapper::SoundWrapper(fileList, mainOutput,vacOutput)
{


    this->setKeySequence(shortcut);
    this->_virtualKey = shortcutVirtualKey;
    this->setPlayerPTTScanCode(pttSC);
    this->setPlayerPTTVirtualKey(pttVK);


}


//V: Constructor when opening a EXP Json file
SoundWrapper::SoundWrapper(QVector<QString> fileList,int mainOutput, int vacOutput, QObject *parent)
    : SoundWrapper::SoundWrapper(fileList,LIDL::Playback::Singleton ,mainOutput, vacOutput,parent)
{

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
    _player->Stop();
    _player->unHoldPTT();
}


/********************************************
 *                Getters:                  *
 ********************************************/

// Soundlist
QVector<QFile*> SoundWrapper::getSoundList()
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
        tempItem.append(new QStandardItem("No shortcut!"));
    else
        tempItem.append(new QStandardItem(this->getKeySequence().toString()));
    QString tmpMode;
    switch(this->getPlayMode())
    {
       case LIDL::Playback::Singleton: tmpMode.append("Singleton"); break;
       case LIDL::Playback::Sequential: tmpMode.append("Sequential"); break;
       case LIDL::Playback::Auto: tmpMode.append("Sequential (Auto)"); break;
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
int SoundWrapper::addSound(QString filename)
{
    this->_soundList.append(new QFile(filename));
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


