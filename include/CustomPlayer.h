#ifndef CUSTOMPLAYER_H
#define CUSTOMPLAYER_H
#pragma once
#include <QObject>
#include <QVector>
#include <QFile>

#include <QDebug>
//#include <QMediaPlayer>
//#include <QMediaService>
//#include <QAudioOutputSelectorControl>
#include "lib/bass.h"
// flac and OGG vorbis support
#include "lib/bassflac.h"
#include <QTimer>
#include <QKeySequence>
//#include "Utility.h"
//#include "windows.h"
//#ifndef MAKEWORD
//   #define MAKEWORD(a,b) Cast(WORD, ((a) And &hFF) Or ((b) Shl 8))
//#endif
//#ifndef MAKELONG
//   #define MAKELONG(a,b) Cast(DWORD, ((a) And &hFFFF) Or ((b) Shl 16))
//#endif
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
#include <QThread>

/*!
 * \file CustomPlayer.h
 * \brief File describing the CustomPlayer class.
 *
 * \author Devolution
 * \version 1.7.0
 * \todo Precaching settings, check if we can know whenever a stream from url had enough data to start playing
 */


/*!
 * \brief The CustomPlayer class, inherits QObject to access signals and slots.*
 *
 * This player uses bass library as a backend to deal with audio streams.
 * It also uses some bass extensions (namely bassflac.h) to deal with the flac format.
 * It uses ssl protocol for streaming over https, so installer includes openssl dll to allow it.
 */
class CustomPlayer : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief downloadCallBack Testing something with a callback function forsenE
     *
     * CALLBACK is either a macro or a type, i don't really know forsenKek
     *
     * \param buffer
     * \param length
     * \param user
     */
    //static void CALLBACK downloadCallBack(const void *buffer, unsigned long length, void *user);
    // DWORD is actually unsigned long in windef.h
    // static void CALLBACK downloadCallBack(const void *buffer, DWORD length, void *user);

    /*!
     * \brief SyncProc
     * \param handle
     * \param channel
     * \param data
     * \param user
     */
//    static void CALLBACK SyncProc(HSYNC handle,DWORD channel,DWORD data,void *user);

    /*!
     * \brief CustomPlayer Default contructor, used by the default constructor of SoundWrapper
     * \param parent nullptr as usual.
     */
    explicit CustomPlayer(QObject *parent = nullptr);
    /*!
     * \brief CustomPlayer Overloaded constructor, doesn't serve any purpose at the moment.
     *
     * Will probably use it to add a "preview" sound button latter.
     * \param soundlist The soundlist, should only contain one file if used for preview
     * \param playMode Default to singleton because if used to preview a sound, it will only contain ONE sound.
     * \param parent nullptr as usual.
     */
    CustomPlayer(QVector<LIDL::SoundFile*> soundlist, LIDL::Playback playMode = LIDL::Playback::Singleton,QObject *parent = nullptr);

    /*!
     * \brief SetOutputDevice Sets the main output device of the player.
     * \param index Device index.
     */
    void SetOutputDevice(int index);
    /*!
     * \brief SetVACDevice Sets the VAC output device of the player.
     * \param index Device index.
     */
    void SetVACDevice(int index);
    /*!
     * \brief GetOutputDevice
     * \return  The index of the main output device.
     */
    int GetOutputDevice();
    /*!
     * \brief GetVACDevice
     * \return  The index of the VAC output device.
     */
    int GetVACDevice();



    /*!
     * \brief SetPlaylist Set the playlist of the CustomPlayer.
     *
     * Will attempt to clear the existing soundlist pointers if it exists.
     * \param soundList The soundlist to be set.
     */
    void SetPlaylist(QVector<LIDL::SoundFile*> soundList);

    /*!
     * \brief SetPlaybackMode Sets the playback mode of the CustomPlayer.
     * \param playMode One of playback mode defined in EnumsAndStructs.h
     */
    void SetPlaybackMode(LIDL::Playback playMode);
    /*! \brief Destructor
    */
    ~CustomPlayer();


public slots:
    /*!
     * \brief PlayAt Will play the song at the specified index.
     *
     * Will play the next sound in line (_soundlist.at(index) ) if and only if one of the output is valid.
     *
     * If the file is remote, it will play it in streaming, so a few seconds to cache it are to be expected.
     * If the file is local,
     *
     *
     *
     *
     * \param index
     * \return The duration of the sound.
     */
    double PlayAt(int index);
    void PlayNext();
  //  void OnTimerTick();
    //void unHoldPTT();
    void Stop();
    void resetShouldPlay();
signals:
    void ErrorPlaying(QString);
    void NowPlaying(QString);
    void holdPTT(int );
    void unHoldPTT();
private:


    //need the files as an array
    QVector<LIDL::SoundFile*> _soundList;
    LIDL::Playback _playMode;
    int _index;

    // Channels

    QVector<int> _mainChannel;
    QVector<int> _vacChannel;

    int getLastVacChannel();
    int getLastMainChannel();
   // QVector<int> _SFXHandle;

    // Devices numbers
    int _mainOutputDevice;
    int _VACOutputDevice;



    // Timers that needs to be canceled if the sound is stopped
    // Those 3 are the reset should play timers
    QTimer * _timerSingleton;
    QTimer * _timerSequential;
    QTimer * _timerSequentialAuto;
    // this one is the autoplay timer
    QTimer * _timerSequentialAutoPlay;
    // and this one the ptt
    QTimer * _timerPTT;




    bool _shouldPlay;
};


#endif // CUSTOMPLAYER_H
