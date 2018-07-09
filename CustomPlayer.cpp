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
    //connect(_timerPTT,SIGNAL(timeout()),this,SLOT(unHoldPTT()));

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
                     //qDebug() << _index << "soundlist size:" << _soundList.size();
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
    //qDebug() << "test:" << _mainChannel.size();

    // Clearing channels array
    for (auto i: _mainChannel)
        BASS_ChannelStop(i);
    for (auto i: _vacChannel)
        BASS_ChannelStop(i);

    _mainChannel.clear();
    _vacChannel.clear();

 //   emit unHoldPTT();
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

        // We check for scheme
        // if it is a URL we use the StreamCreateURL
        if ( _soundList.at(index)->scheme() == "http" || _soundList.at(index)->scheme() == "https" ||_soundList.at(index)->scheme() == "ftp" )
        {
            _mainChannel.append( BASS_StreamCreateURL( _soundList.at(index)->url().toStdWString().c_str(),0,  BASS_STREAM_PRESCAN | BASS_STREAM_AUTOFREE,NULL,NULL  ));
            if (duration == -1)
                duration = BASS_ChannelBytes2Seconds(_mainChannel.last(),
                                                            BASS_ChannelGetLength(_mainChannel.last(),BASS_POS_BYTE));
          duration += 1;
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

        if (duration == -1)
            duration = BASS_ChannelBytes2Seconds(_mainChannel.last(),
                                                        BASS_ChannelGetLength(_mainChannel.last(),BASS_POS_BYTE));
        BASS_ChannelSetDevice(_mainChannel.last(),_mainOutputDevice);


    //http://www.un4seen.com/doc/#bass/BASS_ChannelPlay.html

   // http://www.un4seen.com/doc/#bass/BASS_ChannelSetFX.html

        // Trying to implement HYPER
        //qDebug() << "Volume :" << _soundList.at(index)->getMainVolume();
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
        BASS_ChannelPlay(_mainChannel.last(),_mainOutputDevice);
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

           // BASS_ChannelSetSync(_vacChannel.last(), BASS_SYNC_STALL, 0, &CustomPlayer::SyncProc,  this->_soundList.at(index));



            duration = BASS_ChannelBytes2Seconds(_vacChannel.last(),
                                                        BASS_ChannelGetLength(_mainChannel.last(),BASS_POS_BYTE));
            duration += 1;

            // testing shit :monkaOMEGA:
            QThread *thread = QThread::create([=]{
                int test = 0;
                do{
                    test = BASS_ChannelIsActive( _vacChannel.last() );
                } while(test!=1);
                qDebug() << "NOTHING HERE :ZULOL";
            });
            thread->start();

        }
        else// its a local file, we need to remove file/// because bass is OMEGAZULIDL
        {
            /* RemoveScheme just remove the "file" but not the following three /
             *so we need to remove the first 3 char
             * we also need to use unicode, so wide std string, to that c_str
             * returns a wchar_t[] instead of char[] */
            std::wstring wideString = _soundList.at(index)->url(QUrl::RemoveScheme).remove(0,3).toStdWString();
            _vacChannel.append(BASS_StreamCreateFile(false, wideString.c_str(), 0, 0, BASS_STREAM_AUTOFREE));
            if (duration == -1)
                duration = BASS_ChannelBytes2Seconds(_vacChannel.last(),
                                                        BASS_ChannelGetLength(_vacChannel.last(),BASS_POS_BYTE));
            BASS_ChannelSetDevice(_vacChannel.last(),_VACOutputDevice);
        }
        // prevent the get channel length call twice (cause i think this is sending a request to the http server)


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
//        if (_PTTScanCode !=-1 )
//            emit holdPTT(static_cast<int>(duration*1000) );
        BASS_ChannelPlay(_vacChannel.last(),true);
    }


    // If any of the previous if were passed, the duration isn't -1.
    // if the PTT Scan code is valid (!=-1) we can hold it, but this test is made in SettingsController anyway
    if (duration != -1)
        emit holdPTT(static_cast<int>(duration*1000) );


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


//void CALLBACK CustomPlayer::SyncProc(HSYNC handle,DWORD channel,DWORD data,void *user)
//{
//    qDebug() << "hyperniggerW";

//}


int CustomPlayer::getLastVacChannel()
{
    return this->_vacChannel.last();
}
int CustomPlayer::getLastMainChannel()
{
    return this->_mainChannel.last();
}
