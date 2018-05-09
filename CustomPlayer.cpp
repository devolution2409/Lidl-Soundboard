#include "CustomPlayer.h"

CustomPlayer::CustomPlayer(QObject *parent) : QObject(parent)
{
    _mainOutputDevice = 0;
    _VACOutputDevice = 0;

}

CustomPlayer::CustomPlayer(QVector<QFile*> soundList,int playMode,QObject *parent) : CustomPlayer(parent)
{
    _soundList = soundList;
    _playMode  = playMode;
    _index = 0;
    _shouldPlay = true;
}


// index is already initialized at 0 in constructor
void CustomPlayer::PlayNext()
{
    // if index is OOB from previous playing, we reset it
    // soundlist size returns actual size
    // ie 2 when we have [0] and [1] element.
    // so we need to substract 1.
    qDebug() <<"shouldplay when called" << _shouldPlay;
    if (_index > _soundList.size() -1 )
        _index = 0;

 //   qDebug() << _index << "soundlist size:" << _soundList.size();
    // if we have no  sounds or no audio devices to play on we return
    if ( (_soundList.size() >= 1) && ((_mainOutputDevice != 0) || (_VACOutputDevice != 0)  )    )
    {
        int duration;
        /***********************************
         *           SINGLETON             *
         ***********************************/
        if (_playMode == 1 && _shouldPlay)
        {
            _shouldPlay = false;
            duration =  static_cast<int>(this->PlayAt(_index)*1000);
              QTimer::singleShot(duration,this,SLOT(resetShouldPlay()));
        }
        /***********************************
         *           SEQUENTIAL            *
         ***********************************/
        else if  ((_playMode == 2 && _shouldPlay))
        {
            _shouldPlay = false;
            duration =  static_cast<int>(this->PlayAt(_index++)*1000);
              QTimer::singleShot(duration,this,SLOT(resetShouldPlay()));

        }
        /***********************************
         *       SEQUENTIAL AUTO           *
         ***********************************/
        else if  ((_playMode == 3 && _shouldPlay))
        {

            _shouldPlay = false;
            duration =  static_cast<int>(this->PlayAt(_index++)*1000);
                QTimer::singleShot(duration,this,SLOT(resetShouldPlay()));
                // If the new index is OOB, it means we need to stop playing
                // else we continue
                 qDebug() << _index << "soundlist size:" << _soundList.size();
                if (_index < _soundList.size()  )
                {
                    qDebug() << "DansGame" << _index;
                    QTimer::singleShot(duration+100,this,SLOT(PlayNext()));
                }
        }


    }
}



// used to stop sound that is playing
void CustomPlayer::Stop()
{
    BASS_ChannelStop(_mainChannel);
    BASS_ChannelStop(_vacChannel);
    _shouldPlay = true;
}


void CustomPlayer::OnTimerTick()
{
    this->PlayNext();

}

// Play the sound and return the duration in secs.
double CustomPlayer::PlayAt(int index)
{
    double duration = -1;
    if (_mainOutputDevice != 0)
    {

        BASS_Init(_mainOutputDevice, 44100, 0, 0, nullptr);

        //qDebug() << "Attempting to play file index number:" << index << "\nFilename: " << _soundList.at(index)->fileName().toStdString().c_str();

        // handle for main output
        _mainChannel = BASS_StreamCreateFile(false, _soundList.at(index)->fileName().toStdString().c_str() , 0, 0, BASS_STREAM_AUTOFREE);
        //Playing the sound on main device
        //qDebug() << "Setting output on device number: " << _mainOutputDevice;
        BASS_ChannelSetDevice(_mainChannel,_mainOutputDevice);
//      BOOL BASS_ChannelPlay(DWORD handle,BOOL restart);
//      http://www.un4seen.com/doc/#bass/BASS_ChannelPlay.html
        BASS_ChannelPlay(_mainChannel,true);

        duration = BASS_ChannelBytes2Seconds(_mainChannel,
                                                    BASS_ChannelGetLength(_mainChannel,BASS_POS_BYTE));
    }
    // same for VAC output, and we check the two outputs aren't the same
    if ((_VACOutputDevice != 0) && (_VACOutputDevice != _mainOutputDevice))
    {
        BASS_Init(_VACOutputDevice, 44100, 0, 0, nullptr);
        int _vacChannel = BASS_StreamCreateFile(false, _soundList.at(index)->fileName().toStdString().c_str() , 0, 0, BASS_STREAM_AUTOFREE);
        BASS_ChannelSetDevice(_vacChannel,_VACOutputDevice);
        BASS_ChannelPlay(_vacChannel,true);
        duration = BASS_ChannelBytes2Seconds(_vacChannel,
                                                    BASS_ChannelGetLength(_vacChannel,BASS_POS_BYTE));
    }

    // We check if any of the outputs are valid, if they are, we hold the PTT key
    // if it's not empty (clearing it in the main ui will set both
    // VirtualKey and ScanCode to to -1
    if (( (_VACOutputDevice != 0) || (_mainOutputDevice != 0)) && (_PTTScanCode !=-1 ))
        this->holdPTT(static_cast<int>(duration*1000) );




    return duration;
}


void CustomPlayer::resetShouldPlay()
{
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
    // not sure if this is needed?
    for (auto i:_streamHandle)
        BASS_StreamFree(i);
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
    QTimer::singleShot(duration,this,SLOT(unHoldPTT()));
}

void CustomPlayer::unHoldPTT()
{
    // Unpressing the key physically
    keybd_event(_PTTVirtualKey,_PTTScanCode,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}


