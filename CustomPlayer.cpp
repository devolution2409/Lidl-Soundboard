#include "CustomPlayer.h"

CustomPlayer::CustomPlayer(QObject *parent) : QObject(parent)
{
    _mainOutputDevice = 0;
    _VACOutputDevice = 0;

    _timerSequentialAutoPlay = new QTimer();
    _timerPTT = new QTimer();


    _timerSingleton = new QTimer();
    _timerSequential = new QTimer();
    _timerSequentialAuto = new QTimer();
//    _timerSequential->setSingleShot(true);
//    _timerPTT->setSingleShot(true);

    connect(_timerSequentialAutoPlay,SIGNAL(timeout()),this,SLOT(PlayNext()));
    connect(_timerPTT,SIGNAL(timeout()),this,SLOT(unHoldPTT()));

    connect(_timerSingleton,QTimer::timeout ,this,CustomPlayer::resetShouldPlay);
    connect(_timerSequential,QTimer::timeout ,this,CustomPlayer::resetShouldPlay);
    connect(_timerSequentialAuto,QTimer::timeout ,this,CustomPlayer::resetShouldPlay);

    _shouldPlay = true;
}

CustomPlayer::CustomPlayer(QVector<LIDL::SoundFile *> soundList, LIDL::Playback playMode, QObject *parent) : CustomPlayer(parent)
{
    _soundList = soundList;
    _playMode  = playMode;
    _index = 0;
}


// index is already initialized at 0 in constructor
void CustomPlayer::PlayNext()
{
    // stop the sequential auto timer in case it is running.
    _timerSequentialAutoPlay->stop();

    // if index is OOB from previous playing, we reset it
    // soundlist size returns actual size
    // ie 2 when we have [0] and [1] element.
    // so we need to substract 1.
    // qDebug() <<"shouldplay when called" << _shouldPlay;
    if (_index > _soundList.size() -1 )
        _index = 0;

    // check if next file exists
    if (! _soundList.at(_index)->exists())
    {
        emit ErrorPlaying(_soundList.at(_index)->fileName() );
        return;
    }
    else // if it does exist
    {
        emit NowPlaying(_soundList.at(_index)->fileName());
     //   qDebug() << _index << "soundlist size:" << _soundList.size();
        // if we have no  sounds or no audio devices to play on we return


        if ( (_soundList.size() >= 1) && ((_mainOutputDevice != 0) || (_VACOutputDevice != 0)  )    )
        {
            int duration;
            /***********************************
             *           SINGLETON             *
             ***********************************/
            if (_playMode == LIDL::Playback::Singleton && _shouldPlay)
            {
                _shouldPlay = false;
               // qDebug() << "ZULULUL" << _mainOutputDevice << _VACOutputDevice;
                duration =  static_cast<int>(this->PlayAt(_index)*1000);
                  _timerSingleton->start(duration);
                 // qDebug() << "duration is: " << duration;
            }
            /***********************************
             *           SEQUENTIAL            *
             ***********************************/
            else if  ((_playMode == LIDL::Playback::Sequential && _shouldPlay))
            {
                _shouldPlay = false;
                duration =  static_cast<int>(this->PlayAt(_index++)*1000);
                _timerSequential->start(duration);

            }
            /***********************************
             *       SEQUENTIAL AUTO           *
             ***********************************/
            else if  ((_playMode == LIDL::Playback::Auto && _shouldPlay))
            {

                _shouldPlay = false;
                duration =  static_cast<int>(this->PlayAt(_index++)*1000);
                    _timerSequentialAuto->start(duration);
                    // If the new index is OOB, it means we need to stop playing
                    // else we continue
                     qDebug() << _index << "soundlist size:" << _soundList.size();
                    if (_index < _soundList.size()  )
                        _timerSequentialAutoPlay->start(duration+100);
            }
            // We don't even test should play here since we want the ear rape to happen
            else if ((_playMode == LIDL::Playback::Cancer))
            {
                this->PlayAt(_index++);

            }

        }
    }
}



// used to stop sound that is playing
void CustomPlayer::Stop()
{

    // stop every timer.
    _timerSequentialAutoPlay->stop();
    _timerPTT->stop();
    this->resetShouldPlay();
    // Why the fuck is this called FIVE times?
    qDebug() << "test:" << _mainChannel.size();

    // Clearing channels array
    for (auto i: _mainChannel)
        BASS_ChannelStop(i);
    for (auto i: _vacChannel)
        BASS_ChannelStop(i);

    _mainChannel.clear();
    _vacChannel.clear();

    this->unHoldPTT();
}


//void CustomPlayer::OnTimerTick()
//{
//    this->PlayNext();

//}

// Play the sound and return the duration in secs.
double CustomPlayer::PlayAt(int index)
{
    double duration = -1;
    if (_mainOutputDevice != 0)
    {

        BASS_Init(_mainOutputDevice, 44100, 0, 0, nullptr);

        //qDebug() << "Attempting to play file index number:" << index << "\nFilename: " << _soundList.at(index)->fileName().toStdString().c_str();

        // handle for main output
       // _mainChannel = BASS_StreamCreateFile(false, _soundList.at(index)->fileName().toStdString().c_str() , 0, 0, BASS_STREAM_AUTOFREE);
        //Playing the sound on main device

        _mainChannel.append(BASS_StreamCreateFile(false, _soundList.at(index)->fileName().toStdString().c_str() , 0, 0, BASS_STREAM_AUTOFREE));
        //BASS_ChannelSetDevice(_mainChannel,_mainOutputDevice);
        BASS_ChannelSetDevice(_mainChannel.last(),_mainOutputDevice);


    //http://www.un4seen.com/doc/#bass/BASS_ChannelPlay.html

   // http://www.un4seen.com/doc/#bass/BASS_ChannelSetFX.html
        BASS_ChannelPlay(_mainChannel.last(),_mainOutputDevice);
        // Trying to implement HYPER
        BASS_ChannelSetAttribute(_mainChannel.last(), BASS_ATTRIB_VOL,  _soundList.at(index)->getMainVolume() );

        int LUL = BASS_ChannelSetFX(_mainChannel.last(),BASS_FX_DX8_DISTORTION,255);
       BASS_DISTORTION_PARAM test;
       test.fGain = -5;
        test.fGain = -40;
       BASS_FXSetParameters(LUL,&test);

        //}


        //qDebug() << "Main Volume should be: " << _soundList.at(index)->getMainVolume();
        duration = BASS_ChannelBytes2Seconds(_mainChannel.last(),
                                                    BASS_ChannelGetLength(_mainChannel.last(),BASS_POS_BYTE));
    }
    // same for VAC output, and we check the two outputs aren't the same
    if ((_VACOutputDevice != 0) && (_VACOutputDevice != _mainOutputDevice))
    {
        BASS_Init(_VACOutputDevice, 44100, 0, 0, nullptr);
        _vacChannel.append(BASS_StreamCreateFile(false, _soundList.at(index)->fileName().toStdString().c_str() , 0, 0, BASS_STREAM_AUTOFREE));
        BASS_ChannelSetDevice(_vacChannel.last(),_VACOutputDevice);
        BASS_ChannelPlay(_vacChannel.last(),true);
        BASS_ChannelSetAttribute(_vacChannel.last(), BASS_ATTRIB_VOL,  _soundList.at(index)->getVacVolume() );
    int LUL =  BASS_ChannelSetFX(_vacChannel.last(),BASS_FX_DX8_DISTORTION,255);
     BASS_DISTORTION_PARAM test;
     test.fGain = -5;
        BASS_FXSetParameters(LUL,&test);


       //qDebug() << "VAC Volume should be: " << _soundList.at(index)->getVacVolume();
        duration = BASS_ChannelBytes2Seconds(_vacChannel.last(),
                                                   BASS_ChannelGetLength(_vacChannel.last(),BASS_POS_BYTE));
    }

    // We check if any of the outputs are valid, if they are, we hold the PTT key
    // if it's not empty (clearing it in the main ui will set both
    // VirtualKey and ScanCode to to -1
    if (( (_VACOutputDevice != 0) || (_mainOutputDevice != 0)) && (_PTTScanCode !=-1 ))
    {
         // this is for cancer mode: we reset the timer, else it doesn't really matter since it's stopped
        _timerPTT->stop();
        this->holdPTT(static_cast<int>(duration*1000) );
        _timerPTT->start(static_cast<int>(duration*1000) );
    }


    return duration;
}


void CustomPlayer::resetShouldPlay()
{
    _timerSingleton->stop();
    _timerSequential->stop();
    _timerSequentialAuto->stop();
    _shouldPlay = true;
}


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
}

void CustomPlayer::SetPTTScanCode(int scanCode)
{
    _PTTScanCode = scanCode;
}

void CustomPlayer::SetPPTKeys(int scanCode, int virtualKey)
{
    _PTTScanCode = scanCode;
    _PTTVirtualKey = virtualKey;
}

void CustomPlayer::SetPTTVirtualKey(int virtualKey)
{
    _PTTVirtualKey = virtualKey;
}
// Duration is in milli sec
void CustomPlayer::holdPTT(int duration)
{
    // Pressing key as a SCAN CODE so that it is "physically" pressed
    keybd_event(_PTTVirtualKey,_PTTScanCode,KEYEVENTF_EXTENDEDKEY, 0);
    //QTimer::singleShot(duration,this,SLOT(unHoldPTT()));
}

void CustomPlayer::unHoldPTT()
{
    qDebug() << "unholding ptt";
    // Unpressing the key physically
    keybd_event(_PTTVirtualKey,_PTTScanCode,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    // stopping the timer else PTT will be unhold on each tick forsenT
    _timerPTT->stop();
}

void CustomPlayer::SetPlaylist(QVector<LIDL::SoundFile *> soundList)
{
    _soundList = soundList;
}
void CustomPlayer::SetPlaybackMode(LIDL::Playback playMode)
{
    _playMode = playMode;
}

int  CustomPlayer::GetPTTScanCode()
{
    return this->_PTTScanCode;

}

int  CustomPlayer::GetPTTVirtualKey()
{
    return this->_PTTVirtualKey;
}

int CustomPlayer::GetOutputDevice()
{
    return this->_mainOutputDevice;
}
int CustomPlayer::GetVACDevice()
{
    return this->_VACOutputDevice;
}

//CustomPlayer::~CustomPlayer()
//{

//    QObject::~QObject();
//}
