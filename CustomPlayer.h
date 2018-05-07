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
#include "Utility.h"
#include "windows.h"
#ifndef MAKEWORD
   #define MAKEWORD(a,b) Cast(WORD, ((a) And &hFF) Or ((b) Shl 8))
#endif
#ifndef MAKELONG
   #define MAKELONG(a,b) Cast(DWORD, ((a) And &hFFFF) Or ((b) Shl 16))
#endif


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


class CustomPlayer : public QObject
{
    Q_OBJECT
public:
    explicit CustomPlayer(QObject *parent = nullptr);
    CustomPlayer(QVector<QFile*> soundList,int playMode,QObject *parent = nullptr);
    void SetOutputDevice(int);
    void SetVACDevice(int);
    void SetPTTKey(QKeySequence);
    ~CustomPlayer();

signals:

public slots:
   // void PlayFirst();
    double PlayAt(int index);
    void PlayNext();
    void OnTimerTick();
    void unHoldPTT();
private:
    // we only need one audio pointer
   // QAudioOutput    * _audio;
    // but we need the files as an array
    QVector<QFile*> _soundList;
    int _playMode;
    int _index;
 //   int _count;
    // 1rst Output device info
    //BASS_DEVICEINFO _mainOutputDevice;
    //BASS_DEVICEINFO _VACOutputDevice;
    int _mainOutputDevice;
    int _VACOutputDevice;

    QKeySequence _pushToTalkKey;
    // handle
    QVector<int>    _streamHandle;
    void holdPTT(int duration);


};

#endif // CUSTOMPLAYER_H
