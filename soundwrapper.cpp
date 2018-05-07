#include "soundwrapper.h"

SoundWrapper::SoundWrapper(QObject *parent) : QObject(parent)
{

}


// Constructor to be used in the add sound window
SoundWrapper::SoundWrapper(QListWidget* soundList, int playbackMode,QKeySequence * shortcut, QObject * parent) : QObject(parent)
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
    this->_player = new CustomPlayer(this->getSoundList(),this->getPlayMode());

}


/********************************************
 *                SLOT                      *
 ********************************************/

void SoundWrapper::Play()
{
    _player->PlayNext();
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
int SoundWrapper::getPlayMode()
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
        case 1: tmpMode.append("Singleton"); break;
        case 2: tmpMode.append("Sequential"); break;
        case 3: tmpMode.append("Sequential (Auto)"); break;
    }
    tempItem.append(new QStandardItem(tmpMode));

   return tempItem;
}




void SoundWrapper::OutputDeviceChanged(int index)
{
    //qDebug() << "SLOT: SoundWrapper OutputDeviceChanged output device changed, new is: " << index;
    _player->SetOutputDevice(index);
}

void SoundWrapper::VACDeviceChanged(int index)
{
    _player->SetVACDevice(index);
}

void SoundWrapper::PTTKeyChanged(QKeySequence seq)
{
    _player->SetPTTKey(seq);
}



/********************************************
 *                 Setters:                 *
 ********************************************/


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

int SoundWrapper::setPlayMode(int playmode)
{
    this->_playMode = playmode;
    return 0;
}

// SETTERS for the players, need when constructing new objects forsenT
void SoundWrapper::setPlayerPTTKeySequence(QKeySequence sequence)
{
    this->_player->SetPTTKey(sequence);
}

void SoundWrapper::setPlayerMainOutput(int index)
{
    this->_player->SetOutputDevice(index);
}

void SoundWrapper::setPlayerVACOutput(int index)
{
    this->_player->SetVACDevice(index);
}
