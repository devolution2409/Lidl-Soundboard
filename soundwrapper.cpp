#include "soundwrapper.h"

//I
SoundWrapper::SoundWrapper(QObject *parent) : QObject(parent)
{
    qDebug() << "WTF I WAS CREATED DANSGAME";
    this->_player = new CustomPlayer();
    connect(_player,&CustomPlayer::ErrorPlaying,this,[=](QString songName){
         emit ErrorPlaying(songName);
    });

    connect(_player,&CustomPlayer::NowPlaying ,this,[=](QString songName){
        emit NowPlaying(songName);
    });

    connect(_player, &CustomPlayer::holdPTT, [=] (int duration){
            emit holdPTTProxy(duration);
    });

    connect(this,&CustomPlayer::destroyed, this, [=]{
       qDebug() << "[~SoundWrapper()] I'm either destroyed or about to be !";

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
    this->_virtualKey = static_cast<unsigned int>(shortcutVirtualKey);
    this->setPlayerMainOutput(mainOutput);
    this->setPlayerVACOutput(vacOutput);
    this->_player->SetPlaylist(this->getSoundList());
    this->_player->SetPlaybackMode(this->getPlayMode());

   // qDebug() << "Virtual Key here:" << _virtualKey;
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

// Key sequence as string, including numpads, mb

//QString SoundWrapper::getKeySequenceText()
//{

//}


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
    {

        // trying to accoutn for numpads
        if (this->_virtualKey <= 0x69 && this->_virtualKey >= 0x60)
        {
            //the number stored here is, in fact, from numpad. Thanks qt for not telling forsenD
            // splitting over + in case there is modifiers, using string stream
            /*
            std::istringstream ss(this->keySequence().toString().toStdString());
            std::string token;

            while(std::getline(ss, token, '+')) {
                qDebug() << token.c_str() << '\n';
            }
            */

            // or more simply, just use reverse find to find the last + and add the text there
            std::string text = this->getKeySequence().toString().toStdString();
            std::size_t found = text.rfind("+");
             // if it has at least one modifier, we insert right after it
             if (found!=std::string::npos)
             {
                // qDebug() << "string: " << text.c_str() << " pos:" << found;
                text.insert(found+1,"Numpad ");
             }
             else // we prepend
             {
                 text = "Numpad " + text;
             }

             tempItem.append( new QStandardItem( QString::fromStdString(text)));


        }
        else
        {
             tempItem.append(new QStandardItem(this->getKeySequence().toString()));
        }

    }
        //tempItem.append(new QStandardItem();

      //
    QString tmpMode;
    switch(this->getPlayMode())
    {

       case LIDL::Playback::Sequential: tmpMode.append("Sequential"); break;
       case LIDL::Playback::Auto: tmpMode.append("Sequential (Auto)"); break;
       case LIDL::Playback::Cancer: tmpMode.append("Singleton (Cancer)");break;
       case LIDL::Playback::AutoLoop: tmpMode.append("Sequential (Auto Loop)");break;

       // legacy, should never be called
       case LIDL::Playback::Singleton: tmpMode.append("Singleton"); break;
    }
    tempItem.append(new QStandardItem(tmpMode));

    for (auto &i: tempItem)
        i->setTextAlignment(Qt::AlignVCenter);

   return tempItem;
}



unsigned int SoundWrapper::getShortcutVirtualKey()
{
    return _virtualKey;
}

QJsonObject SoundWrapper::GetWrapperAsObject() const
{
    // creating temp sound collection
    QJsonObject tempSound;
    tempSound.insert("Playback Mode",static_cast<int>(_playMode));
    // qDebug() << i->getPlayMode();
    QJsonObject key;
    key.insert("Key", _keySequence.toString());
    key.insert("VirtualKey", static_cast<int>(_virtualKey));
    tempSound.insert("Shortcut",key);
    // The sound collection
    QJsonArray soundCollection;
    for (auto &j: _soundList)
    {
        QJsonObject properties;
            properties.insert("Main Volume",static_cast<int>(j->getMainVolume() *100));
            properties.insert("VAC Volume" ,static_cast<int>(j->getVacVolume() *100));
            properties.insert("SFX Flags", static_cast<int>(j->getSFX().flags));

        QJsonObject soundEffects;

            QJsonObject distortion;
            distortion.insert("Gain", static_cast<int>(j->getSFX().distortion.fGain));
            distortion.insert("Edge", static_cast<int>(j->getSFX().distortion.fEdge));
            distortion.insert("EQCenterFrequency",static_cast<int>(j->getSFX().distortion.fPostEQCenterFrequency));
            distortion.insert("EQBandwidth",static_cast<int>(j->getSFX().distortion.fPostEQBandwidth));
            distortion.insert("Cutoff",static_cast<int>(j->getSFX().distortion.fPreLowpassCutoff));
            soundEffects.insert("Distortion",distortion);

            QJsonObject chorus;
            chorus.insert("Delay",static_cast<int>(j->getSFX().chorus.fDelay));
            chorus.insert("Depth",static_cast<int>(j->getSFX().chorus.fDepth));
            chorus.insert("Feedback",static_cast<int>(j->getSFX().chorus.fFeedback));
            chorus.insert("Frequency",static_cast<int>(j->getSFX().chorus.fFrequency));
            chorus.insert("Wet Dry Mix",static_cast<int>(j->getSFX().chorus.fWetDryMix));
            chorus.insert("Phase",static_cast<int>(j->getSFX().chorus.lPhase));
            chorus.insert("Waveform",static_cast<int>(j->getSFX().chorus.lWaveform));
            soundEffects.insert("Chorus",chorus);

            QJsonObject echo;

            echo.insert("Feedback",static_cast<int>(j->getSFX().echo.fFeedback) );
            echo.insert("Left Delay",static_cast<int>(j->getSFX().echo.fLeftDelay) );
            echo.insert("Right Delay",static_cast<int>(j->getSFX().echo.fRightDelay) );
            echo.insert("Wet Dry Mix",static_cast<int>(j->getSFX().echo.fWetDryMix) );
            echo.insert("Swap",static_cast<int>(j->getSFX().echo.lPanDelay) );
            soundEffects.insert("Echo",echo);

            QJsonObject flanger;
            flanger.insert("Delay",static_cast<int>(j->getSFX().flanger.fDelay));
            flanger.insert("Depth",static_cast<int>(j->getSFX().flanger.fDepth));
            flanger.insert("Feedback",static_cast<int>(j->getSFX().flanger.fFeedback));
            flanger.insert("Frequency",static_cast<int>(j->getSFX().flanger.fFrequency));
            flanger.insert("Phase",static_cast<int>(j->getSFX().flanger.lPhase));
            flanger.insert("Waveform",static_cast<bool>(j->getSFX().flanger.lWaveform));
            soundEffects.insert("Flanger",flanger);


            QJsonObject compressor;
            compressor.insert("Attack",static_cast<int>(j->getSFX().compressor.fAttack));
            compressor.insert("Gain"  ,static_cast<int>(j->getSFX().compressor.fGain));
            compressor.insert("Predelay",static_cast<int>(j->getSFX().compressor.fPredelay));
            compressor.insert("Ratio",static_cast<int>(j->getSFX().compressor.fRatio));
            compressor.insert("Release",static_cast<int>(j->getSFX().compressor.fRelease));
            compressor.insert("Threshold",static_cast<int>(j->getSFX().compressor.fThreshold));
            soundEffects.insert("Compressor",compressor);

            QJsonObject gargle;
            gargle.insert("Rate",static_cast<int>(j->getSFX().gargle.dwRateHz));
            gargle.insert("Waveform",static_cast<bool>(j->getSFX().gargle.dwWaveShape));
            soundEffects.insert("Gargle",compressor);



        //soundCollection.insert(  j->url(), properties); // old wey
        // new way: (VER > 1.7.0.)
        QJsonObject numberedSound;
        properties.insert("SFX",soundEffects);
        numberedSound.insert( j->url(), properties);

        soundCollection.append(numberedSound);


    }
    tempSound.insert("Sound Collection",soundCollection);
    return tempSound;
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
    {
        if ( a._soundList.at(i) != b._soundList.at(i) )
            return false;
    }

        return true;
}

bool operator!=(const SoundWrapper &a, const SoundWrapper &b)
{
    return !(a==b);
}


