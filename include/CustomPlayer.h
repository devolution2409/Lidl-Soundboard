#ifndef CUSTOMPLAYER_H
#define CUSTOMPLAYER_H

#include <QObject>
#include <QVector>
#include <QFile>

#include <QDebug>
//#include <QMediaPlayer>
//#include <QMediaService>
//#include <QAudioOutputSelectorControl>
#include "lib/bass.h"
#include <QTimer>
#include <QKeySequence>
//#include "Utility.h"
#include "windows.h"
#ifndef MAKEWORD
   #define MAKEWORD(a,b) Cast(WORD, ((a) And &hFF) Or ((b) Shl 8))
#endif
#ifndef MAKELONG
   #define MAKELONG(a,b) Cast(DWORD, ((a) And &hFFFF) Or ((b) Shl 16))
#endif
// so it knows the enum
#include "EnumsAndStructs.h"
/*
typedef DWORD HMUSIC;		// MOD music handle
typedef DWORD HSAMPLE;		// sample handle
typedef DWORD HCHANNEL;		// playing sample's channel handle
typedef DWORD HSTREAM;		// sample stream handle
typedef DWORD HRECORD;		// recording handle
typedef DWORD HSYNC;		// synchronizer handle
typedef DWORD HDSP;			// DSP handle
typedef DWORD HFX;			// DX8 effect handle
typedef DWORD HPLUGIN;		// Plugin handle
*/
#include "CustomSoundFile.h"

class CustomPlayer : public QObject
{
    Q_OBJECT
public:
    explicit CustomPlayer(QObject *parent = nullptr);
    CustomPlayer(QVector<LIDL::SoundFile*>, LIDL::Playback playMode,QObject *parent = nullptr);
    void SetOutputDevice(int);
    void SetVACDevice(int);
    int GetOutputDevice();
    int GetVACDevice();

    void SetPTTScanCode(int scanCode);
    void SetPTTVirtualKey(int virtualKey);
    int  GetPTTScanCode();
    int  GetPTTVirtualKey();


    void SetPPTKeys(int scanCode, int virtualKey);
    void SetPlaylist(QVector<LIDL::SoundFile*> soundList);

    void SetPlaybackMode(LIDL::Playback playMode);
    ~CustomPlayer();


public slots:
   // void PlayFirst();
    double PlayAt(int index);
    void PlayNext();
  //  void OnTimerTick();
    void unHoldPTT();
    void Stop();
    void resetShouldPlay();
signals:
    void ErrorPlaying(QString);
    void NowPlaying(QString);
private:
    // we only need one audio pointer
   // QAudioOutput    * _audio;
    // but we need the files as an array
    QVector<LIDL::SoundFile*> _soundList;
    LIDL::Playback _playMode;
    int _index;

    // Channels
    //int _mainChannel;
  //  int _vacChannel;
    QVector<int> _mainChannel;
    QVector<int> _vacChannel;
    QVector<int> _SFXHandle;

    // Devices numbers
    int _mainOutputDevice;
    int _VACOutputDevice;

    // PTT ScanCode and VirtualKey
    int _PTTScanCode;
    int _PTTVirtualKey;

    // Timers that needs to be canceled if the sound is stopped
    // Those 3 are the reset should play timers
    QTimer * _timerSingleton;
    QTimer * _timerSequential;
    QTimer * _timerSequentialAuto;
    // this one is the autoplay timer
    QTimer * _timerSequentialAutoPlay;
    // and this one the ptt
    QTimer * _timerPTT;


    void holdPTT(int duration);


    bool _shouldPlay;
};


#endif // CUSTOMPLAYER_H
