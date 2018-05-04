#include "CustomPlayer.h"

CustomPlayer::CustomPlayer(QObject *parent) : QObject(parent)
{
     BASS_Init(-1, 44100, 0, 0, nullptr);
//    int a, count=0;
//    BASS_DEVICEINFO info;
//    for (a=1; BASS_GetDeviceInfo(a, &info); a++)
//        if (info.flags&BASS_DEVICE_ENABLED) // device is enabled
//            qDebug() << info.name;

}

CustomPlayer::CustomPlayer(QVector<QFile*> soundList,int playMode,QObject *parent) : CustomPlayer(parent)
{
    _soundList = soundList;
    _playMode  = playMode;
    _index = 0;
  // _streamHandle.append( BASS_StreamCreateFile(true, buffer, 0, file.getSize(), BASS_STREAM_AUTOFREE);             );
    //musicStreamID = BASS_StreamCreateFile(true, buffer, 0, file.getSize(), BASS_STREAM_AUTOFREE);
}

// Can even be used to make it play from start FeelsGoodMan
void CustomPlayer::PlayNext()
{
    // if index++ isn't oob:
    if ((_index+1) <= _soundList.size())
    {

        int duration = static_cast<int>(this->PlayAt(_index++) * 1000);
        qDebug() << duration;
        // IF playmode is 3 (which is sequential auto, we register the sound to be played)
        if (this->_playMode == 3)
            QTimer::singleShot(duration, this, SLOT(OnTimerTick()));


    }
    // else if it is oob and we are NOT in sequential auto mode we circle. Else we don't.
    else if ((this->_playMode != 3))
    {
        _index = 0;
        this->PlayAt(_index++);
    }
    else
    {
        _index = 0;
    }
}





void CustomPlayer::OnTimerTick()
{
    this->PlayNext();

}

// Play the sound and return the duration in secs.
double CustomPlayer::PlayAt(int index)
{
    qDebug() << "Attempting to play file index number:" << index
             << "\nFilename: " << _soundList.at(index)->fileName().toStdString().c_str();

    // adding the channel number in the thingy
    _streamHandle.append( BASS_StreamCreateFile(false, _soundList.at(index)->fileName().toStdString().c_str() , 0, 0, 0));
    BASS_ChannelPlay(_streamHandle.at(index),true);
    double duration = BASS_ChannelBytes2Seconds(_streamHandle.at(index),
                                                     BASS_ChannelGetLength(_streamHandle.at(index),BASS_POS_BYTE));

    qDebug() << "Duration: " << duration;
    return duration;
}



/*void CustomPlayer::PlayAt(int index)
{

  //  _soundList.at(index)->open(QIODevice::ReadOnly);

 //   QMediaPlayer *player = new QMediaPlayer;



    player->setMedia(QUrl::fromLocalFile(_soundList.at(index)->fileName()));
    player->play();
//    _audio = new QAudioOutput(format, this);
    //connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
    //_audio->start(_soundList.at(index));

    qDebug()  << "playing file: " << _soundList.at(0)->fileName();
}*/


void CustomPlayer::SetOutputDevice(int deviceIndex)
{
    // if this function recieved 0 it means we passed the <no device selected> thingy
    if (deviceIndex == 0)
        return;
    else // hence the -1
        BASS_GetDeviceInfo(deviceIndex - 1, &_mainOutputDevice);

}

CustomPlayer::~CustomPlayer()
{
    // not sure if this is needed?
    for (auto i:_streamHandle)
        BASS_StreamFree(i);
}
