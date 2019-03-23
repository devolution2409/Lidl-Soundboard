#include "CustomPlayer.h"

CustomPlayer::CustomPlayer(QObject *parent) : QObject(parent)
{
    _mainOutputDevice = 0;
    _VACOutputDevice = 0;

    _timerSequentialAutoPlay = new QTimer();


//    _timerSingleton = new QTimer();
//    _timerSequential = new QTimer();
//    _timerSequentialAuto = new QTimer();
    _timerShouldPlay = new QTimer();

    connect(_timerSequentialAutoPlay,&QTimer::timeout,this,&CustomPlayer::PlayNext);
    //connect(_timerPTT,SIGNAL(timeout()),this,SLOT(unHoldPTT()));

    connect(_timerShouldPlay,&QTimer::timeout,this,[=] {this->resetShouldPlay();});
//    connect(_timerSingleton,&QTimer::timeout ,this, [=]{this->resetShouldPlay(); });
//    connect(_timerSequential,&QTimer::timeout ,this,[=]{this->resetShouldPlay(); });
//    connect(_timerSequentialAuto,&QTimer::timeout ,this,[=]{this->resetShouldPlay(); });

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
                  //_timerSingleton->start(duration);
                _timerShouldPlay->start(duration);
                // qDebug() << "duration is: " << duration;
            }
            /***********************************
             *           SEQUENTIAL            *
             ***********************************/
            else if  ((_playMode == LIDL::Playback::Sequential && _shouldPlay))
            {
                _shouldPlay = false;
                duration =  static_cast<int>(this->PlayAt(_index++)*1000);
                _timerShouldPlay->start(duration);

            }
            /***********************************
             *       SEQUENTIAL AUTO           *
             ***********************************/
            else if  (((_playMode == LIDL::Playback::Auto || _playMode == LIDL::Playback::AutoLoop )&& _shouldPlay))
            {
                _shouldPlay = false;
                duration =  static_cast<int>(this->PlayAt(_index++)*1000);
                _timerShouldPlay->start(duration);
                    // If the new index is OOB, it means we need to stop playing
                    // unless it's autoloop :)
                    if (_index < _soundList.size() || _playMode == LIDL::Playback::AutoLoop )
                        _timerSequentialAutoPlay->start(duration+100);

            }
            // We don't even test should play here since we want the ear rape to happen
            else if ((_playMode == LIDL::Playback::Cancer))
            {
                this->PlayAt(_index++);
            }
            // If playback is autoplay, we need to check sound has finished
//            else if ((_playMode == LIDL::Playback::Singleton))
//            {
//                this->PlayAt(_index++);
//            }




        }
    }
}



// used to stop sound that is playing
void CustomPlayer::Stop()
{

    // stop every timer.
    _timerSequentialAutoPlay->stop();
    this->resetShouldPlay();
    // Why the fuck is this called FIVE times?
    //qDebug() << "test:" << _mainChannel.size();

    // Clearing channels array
    for (auto i: _mainChannel)
        BASS_ChannelStop(i);
    for (auto i: _vacChannel)
        BASS_ChannelStop(i);


// No need to clear the channels anymore they'll get cleared by the timer forsenE

//    _mainChannel.clear();
//    _vacChannel.clear();

}



// Play the sound and return the duration in secs.
double CustomPlayer::PlayAt(int index)
{

    double duration = -1;
    bool remote = false;
    if (_soundList.at(index)->scheme() == "http" || _soundList.at(index)->scheme() == "https" ||_soundList.at(index)->scheme() == "ftp")
        remote = true;

    if (_mainOutputDevice != 0)
    {
        BASS_Init(_mainOutputDevice, 44100, 0, 0, nullptr);

        //qDebug() << "Attempting to play file index number:" << index << "\nFilename: " << _soundList.at(index)->fileName().toStdString().c_str();

        // handle for main output
       // _mainChannel = BASS_StreamCreateFile(false, _soundList.at(index)->fileName().toStdString().c_str() , 0, 0, BASS_STREAM_AUTOFREE);
        //Playing the sound on main device

        // We check for scheme
        // if it is a URL we use the StreamCreateURL
        if ( _soundList.at(index)->scheme() == "http" || _soundList.at(index)->scheme() == "https" ||_soundList.at(index)->scheme() == "ftp" )
        {
            _mainChannel.append( BASS_StreamCreateURL( _soundList.at(index)->url().toStdWString().c_str(),0,  BASS_STREAM_PRESCAN | BASS_STREAM_AUTOFREE,NULL,NULL  ));

        }
        else// its a local file, we need to remove file/// because bass is OMEGAZULIDL
        {
            /* RemoveScheme just remove the "file" but not the following three /
             *so we need to remove the first 3 char
             * we also need to use unicode, so wide std string, to that c_str
             * returns a wchar_t[] instead of char[] */
            std::wstring wideString = _soundList.at(index)->url(QUrl::RemoveScheme).remove(0,3).toStdWString();
            // need to use AT LEAST unicode for some lidl character like ♂ to work
            _mainChannel.append(BASS_StreamCreateFile(false, wideString.c_str(), 0, 0, BASS_STREAM_AUTOFREE));
        }


        BASS_ChannelSetDevice(_mainChannel.last(),_mainOutputDevice);




        //http://www.un4seen.com/doc/#bass/BASS_ChannelPlay.html
        //http://www.un4seen.com/doc/#bass/BASS_ChannelSetFX.html
        BASS_ChannelSetAttribute(_mainChannel.last(), BASS_ATTRIB_VOL,  _soundList.at(index)->getMainVolume() );

        // if distortion is enabled:
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::DISTORTION)
        {
            int LUL = BASS_ChannelSetFX(_mainChannel.last(),BASS_FX_DX8_DISTORTION,255);
            BASS_DX8_DISTORTION wut = _soundList.at(index)->getSFX().distortion;
            BASS_FXSetParameters(LUL, &wut );
        }
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::CHORUS)
        {
            int LUL = BASS_ChannelSetFX(_mainChannel.last(),BASS_FX_DX8_CHORUS,254);
            BASS_DX8_CHORUS SoBayed = _soundList.at(index)->getSFX().chorus;
            BASS_FXSetParameters(LUL,&SoBayed);
        }
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::ECHO)
        {
            int LUL = BASS_ChannelSetFX(_mainChannel.last(),BASS_FX_DX8_ECHO,253);
            BASS_DX8_ECHO SoBayed = _soundList.at(index)->getSFX().echo;
            BASS_FXSetParameters(LUL,&SoBayed);
        }
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::COMPRESSOR)
        {
            int LUL = BASS_ChannelSetFX(_mainChannel.last(),BASS_FX_DX8_COMPRESSOR,252);
            BASS_DX8_COMPRESSOR cmonBruhDontThinkIt = _soundList.at(index)->getSFX().compressor ;
            BASS_FXSetParameters(LUL,&cmonBruhDontThinkIt);
        }
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::FLANGER)
        {
            int LUL = BASS_ChannelSetFX(_mainChannel.last(),BASS_FX_DX8_FLANGER,251);
            BASS_DX8_FLANGER rosesAreRedVioletsAreBlueCrossesABridgeWhatAFuckingFlanger = _soundList.at(index)->getSFX().flanger ;
            BASS_FXSetParameters(LUL,&rosesAreRedVioletsAreBlueCrossesABridgeWhatAFuckingFlanger);
        }
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::GARGLE)
        {
            int LUL = BASS_ChannelSetFX(_mainChannel.last(),BASS_FX_DX8_GARGLE,251);
            BASS_DX8_GARGLE gargleOnTheBallSack = _soundList.at(index)->getSFX().gargle ;
            BASS_FXSetParameters(LUL,&gargleOnTheBallSack);
        }

        if (duration == -1)
            duration = BASS_ChannelBytes2Seconds(_mainChannel.last(),
                                                        BASS_ChannelGetLength(_mainChannel.last(),BASS_POS_BYTE));
    }



    // same for VAC output, and we check the two outputs aren't the same
    if ((_VACOutputDevice != 0) && (_VACOutputDevice != _mainOutputDevice))
    {
        BASS_Init(_VACOutputDevice, 44100, 0, 0, nullptr);
        // We check for scheme
        // if it is a URL we use the StreamCreateURL
        if ( _soundList.at(index)->scheme() == "http" || _soundList.at(index)->scheme() == "https" ||_soundList.at(index)->scheme() == "ftp" )
        {
            _vacChannel.append( BASS_StreamCreateURL( _soundList.at(index)->url().toStdWString().c_str(),0,
                                                      BASS_STREAM_AUTOFREE,
                                                     nullptr,
                                                     nullptr));
        }
        else// its a local file, we need to remove file/// because bass is OMEGAZULIDL
        {
            /* RemoveScheme just remove the "file" but not the following three /
             *so we need to remove the first 3 char
             * we also need to use unicode, so wide std string, to that c_str
             * returns a wchar_t[] instead of char[] */
            std::wstring wideString = _soundList.at(index)->url(QUrl::RemoveScheme).remove(0,3).toStdWString();
            _vacChannel.append(BASS_StreamCreateFile(false, wideString.c_str(), 0, 0, BASS_STREAM_AUTOFREE));
        }
        // prevent the get channel length call twice (cause i think this is sending a request to the http server)



        BASS_ChannelSetDevice(_vacChannel.last(),_VACOutputDevice);
        //qDebug() << "vac volume:" <<  _soundList.at(index)->getVacVolume();
        BASS_ChannelSetAttribute(_vacChannel.last(), BASS_ATTRIB_VOL,  _soundList.at(index)->getVacVolume() );
        //qDebug() << "Is distortion enabled here" << _soundList.at(index)->getSFX().distortionEnabled;
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::DISTORTION )
        {
            int LUL = BASS_ChannelSetFX(_vacChannel.last(),BASS_FX_DX8_DISTORTION,255);
            BASS_DX8_DISTORTION wut = _soundList.at(index)->getSFX().distortion;
            BASS_FXSetParameters(LUL, &wut );

        }
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::CHORUS)
        {
            int LUL = BASS_ChannelSetFX(_vacChannel.last(),BASS_FX_DX8_CHORUS,254);
            BASS_DX8_CHORUS SoBayed = _soundList.at(index)->getSFX().chorus;
            BASS_FXSetParameters(LUL,&SoBayed);
        }
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::ECHO)
        {
            int LUL = BASS_ChannelSetFX(_vacChannel.last(),BASS_FX_DX8_ECHO,253);
            BASS_DX8_ECHO eatTheCatOMEGALEE = _soundList.at(index)->getSFX().echo;
            BASS_FXSetParameters(LUL,&eatTheCatOMEGALEE);
        }

        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::COMPRESSOR)
        {
            int LUL = BASS_ChannelSetFX(_vacChannel.last(),BASS_FX_DX8_COMPRESSOR,252);
            BASS_DX8_COMPRESSOR cmonBruhDontThinkIt = _soundList.at(index)->getSFX().compressor ;
            BASS_FXSetParameters(LUL,&cmonBruhDontThinkIt);
        }
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::FLANGER)
        {
            int LUL = BASS_ChannelSetFX(_vacChannel.last(),BASS_FX_DX8_FLANGER,251);
            BASS_DX8_FLANGER rosesAreRedVioletsAreBlueCrossesABridgeWhatAFuckingFlanger = _soundList.at(index)->getSFX().flanger ;
            BASS_FXSetParameters(LUL,&rosesAreRedVioletsAreBlueCrossesABridgeWhatAFuckingFlanger);
        }
        if (_soundList.at(index)->getSFX().flags & LIDL::SFX_TYPE::GARGLE)
        {
            int LUL = BASS_ChannelSetFX(_vacChannel.last(),BASS_FX_DX8_GARGLE,251);
            BASS_DX8_GARGLE gargleOnTheBallSack = _soundList.at(index)->getSFX().gargle ;
            BASS_FXSetParameters(LUL,&gargleOnTheBallSack);
        }



        if (duration == -1)
            duration = BASS_ChannelBytes2Seconds(_vacChannel.last(),
                                                    BASS_ChannelGetLength(_vacChannel.last(),BASS_POS_BYTE));
    }

    // for some reason bass can't fetch the exact duration of ogg and a second is missing
    // even if the file is local forsenD
    if (_soundList.at(index)->url().contains(".ogg") || (_soundList.at(index)->url().contains(".flac")  ))
        duration+=1;

    qDebug() << "duration: " << duration;

    if (_mainOutputDevice != 0)
        BASS_ChannelPlay(_mainChannel.last(),false);
    // apparentely this crash on linux if this test isn't present? :WutFace:
    if ((_VACOutputDevice != 0) && (_VACOutputDevice != _mainOutputDevice))
        BASS_ChannelPlay(_vacChannel.last(),false);
    // testing shit :monkaOMEGA:
    // working :feelsokay man
    // Basically the thread will check for the state of the channel, when it is playing
    // it will hold ptt.
    if (remote)
    {

        QThread *thread = QThread::create([=]{
            int test = 0;
            do{
                test = BASS_ChannelIsActive( _vacChannel.last() );
            } while(test!=1);

        });
        connect(thread,&QThread::finished,this, [=]{
            emit holdPTT(static_cast<int>(duration*1000));
        });
        thread->start();
    }
    // If any of the previous if were passed, the duration isn't -1.
    if (!remote)
        emit holdPTT(static_cast<int>(duration*1000) );

    int tempIndex = _mainChannel.size() - 1 ;
    if (tempIndex != -1)
    {
        // need to remove the appended index in the vectors else it will keep growing
        QTimer::singleShot(static_cast<int>((duration + 0.5)*1000),this, [=]{
                _mainChannel.pop_front();
                _vacChannel.pop_front();
            } );
    }
    return duration;
}


void CustomPlayer::resetShouldPlay()
{
    _timerShouldPlay->stop();
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
    if (_soundList.size() != 0)
        for (auto &i: _soundList)
            delete i;
}



void CustomPlayer::SetPlaylist(QVector<LIDL::SoundFile *> soundList)
{
    // Testing this for 1.7.0 release since we will add preview
    // deleted the soundfiles before clearing everything and setting new soundlist
    if (_soundList.size() != 0)
        for (auto &i: _soundList)
            delete i;
    _soundList.clear();
    _soundList = soundList;
}
void CustomPlayer::SetPlaybackMode(LIDL::Playback playMode)
{
    _playMode = playMode;
}


int CustomPlayer::GetOutputDevice()
{
    return this->_mainOutputDevice;
}
int CustomPlayer::GetVACDevice()
{
    return this->_VACOutputDevice;
}

// we can use (this) as user data or this.soundfile or w/e
// to delete the last chan maybe we could use last+1 provided the size of the array is in fact already incremented when the callback
// function is called.


/* BASS_STREAM_RESTRATE	Restrict the download rate of the file to the rate required to sustain playback.
 * If this flag is not used, then the file will be downloaded as quickly as the user's internet connection allows.*/
//void CALLBACK CustomPlayer::downloadCallBack(const void *buffer, DWORD length, void *user)
//{
//   //NaM SHUTTHEFUCKUPCOMPILER, this is a pointer to the file being played
//    LIDL::SoundFile* test = reinterpret_cast<LIDL::SoundFile*>(user);
//    // download work by chunk. This function is called until we have 0 bytes left to download.
//    // the last call is when what we need to download is 0, so we can use that to reset the downloaded var.
//    static int downloaded = 0;
//    if (length == 0)
//        downloaded = 0;
//    else
//    {
//        downloaded+= length;
//        qDebug() << QString("Downloaded %1 out of %2 bytes.").arg(downloaded).arg(test->getSize() );

//    }
//}

