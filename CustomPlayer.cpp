#include "CustomPlayer.h"

CustomPlayer::CustomPlayer(QObject *parent) : QObject(parent)
{
    _mainOutputDevice = 0;
    _VACOutputDevice = 0;
    //_count = 0;
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
        //qDebug() <<"index is:" <<_index << "soundlist size is:" << _soundList.size();
    if ((_index +1) <= _soundList.size() )
    {
        // play at returns duration of the sound in double, or -1
        int duration = static_cast<int>(this->PlayAt(_index++) * 1000);
        // qDebug() << duration;
        // IF playmode is 3 (which is sequential auto, we register the sound to be played)
        if (this->_playMode == 3)
            QTimer::singleShot(duration, this, SLOT(OnTimerTick()));


    }
    // else if it is oob and we are NOT in sequential auto mode we circle. Else we don't.
    else if ((this->_playMode == 2))
    {
        _index = 0;
        this->PlayAt(_index++);
    }
    else if ((this->_playMode == 1))
    {
        _index = 0;
        this->PlayAt(_index);
    }


}





void CustomPlayer::OnTimerTick()
{
    this->PlayNext();

}

// Play the sound and return the duration in secs.
double CustomPlayer::PlayAt(int index)
{
    // if it's 0 it SHOULD NOT play but since it does anyway we are going to add a test forsenE
    double duration = -1;
    if (_mainOutputDevice != 0)
    {
        BASS_Init(_mainOutputDevice, 44100, 0, 0, nullptr);

        //qDebug() << "Attempting to play file index number:" << index << "\nFilename: " << _soundList.at(index)->fileName().toStdString().c_str();

        // handle for main output
        int mainChannel = BASS_StreamCreateFile(false, _soundList.at(index)->fileName().toStdString().c_str() , 0, 0, BASS_STREAM_AUTOFREE);
        //Playing the sound on main device
        //qDebug() << "Setting output on device number: " << _mainOutputDevice;
        BASS_ChannelSetDevice(mainChannel,_mainOutputDevice);
        BASS_ChannelPlay(mainChannel,true);

        duration = BASS_ChannelBytes2Seconds(mainChannel,
                                                    BASS_ChannelGetLength(mainChannel,BASS_POS_BYTE));

    }
    // same for VAC output, and we check the two outputs aren't the same
    if ((_VACOutputDevice != 0) && (_VACOutputDevice != _mainOutputDevice))
    {
        BASS_Init(_VACOutputDevice, 44100, 0, 0, nullptr);
        int vacChannel = BASS_StreamCreateFile(false, _soundList.at(index)->fileName().toStdString().c_str() , 0, 0, BASS_STREAM_AUTOFREE);
        BASS_ChannelSetDevice(vacChannel,_VACOutputDevice);
        BASS_ChannelPlay(vacChannel,true);
        duration = BASS_ChannelBytes2Seconds(vacChannel,
                                                    BASS_ChannelGetLength(vacChannel,BASS_POS_BYTE));
    }
    // We check if any of the outputs are valid, if they are, we hold the PTT key
    // if it's not empty
    if (( (_VACOutputDevice != 0) || (_mainOutputDevice != 0)) && !(_pushToTalkKey.isEmpty()))
    {
        this->holdPTT(static_cast<int>(duration*1000) );
       // SendInput();
    }


    //qDebug() << "Duration: " << duration;


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

    //qDebug()  << "playing file: " << _soundList.at(0)->fileName();
}*/


void CustomPlayer::SetOutputDevice(int deviceIndex)
{
    // if this function recieved 0 it means we passed the <no device selected> thingy
    // http://www.un4seen.com/doc/#bass/BASS_ChannelSetDevice.html
    _mainOutputDevice = deviceIndex;

}

void CustomPlayer::SetVACDevice(int deviceIndex)
{
    _VACOutputDevice = deviceIndex;
}



CustomPlayer::~CustomPlayer()
{
    // not sure if this is needed?
    for (auto i:_streamHandle)
        BASS_StreamFree(i);
}

void CustomPlayer::SetPTTKey(QKeySequence sequence)
{
    _pushToTalkKey = sequence;
}

// Duration is in milli sec
void CustomPlayer::holdPTT(int duration)
{
    // Pressing key as a SCAN CODE so that it is "physically" pressed
    keybd_event(0,Utility::GetKeyAsScanCode(_pushToTalkKey.toString()),KEYEVENTF_EXTENDEDKEY, 0);
    QTimer::singleShot(duration+100,this,SLOT(unHoldPTT()));
}

void CustomPlayer::unHoldPTT()
{
    // Unpressing the key physically
    keybd_event(0,Utility::GetKeyAsScanCode(_pushToTalkKey.toString()),KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}


