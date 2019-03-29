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
#include <limits> //quiet_NaN()
#include <cmath> // std::isnan()


/*!
 * \file CustomPlayer.h
 * \brief File describing the CustomPlayer class.
 *
 * \author Devolution
 * \version 1.7.0
 * \since 0.3
 * \todo Precaching settings
 *
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
     * \brief PlayNext PlayNext function is called whenever the shortcut, or play button, is pressed.
     *
     * It will either play the same sound over and over again (Singleton, Singleton Cancer)
     * Or cycle through the files in the wrapper (Sequential, Sequential Auto)
     */
    void PlayNext();

    /*!
     * \brief Stop all the channels of this player.
     *
     * Iterates through channels and calls  BASS_ChannelStop() on them.
     * BASS_ChannelStop will not crash the soundword if channel isn't valid.
     * (Which <i>could</i> be the case if the iteration starts and somehow the vector is popped_front right at the same time.)
     * It should not happen tho, as stop it called, it means the sound is still playing.
     */
    void Stop();

signals:
    /*!
     * \brief ErrorPlaying Signal to tell the main UI (via the proxy set up in the SoundWrapper class) that an error happened playing the sound <sound name> (will be displayed in status bar).
     */
    void ErrorPlaying(QString);
    /*!
     * \brief NowPlaying Signal to tell the main UI (via the proxy set up in the SoundWrapper class) to display now playing sound <soundName> in the status bar.
     */
    void NowPlaying(QString);
    /*!
     * \brief holdPTT Signal to tell the main UI (via the proxy set up in the SoundWrapper class) to hold ptt for the duration.
     *
     * \param duration
     */
    void holdPTT(int duration);

private:

    /*!
     * \brief PlayAt Will play the song at the specified index.
     *
     * Will play the next sound in line (_soundlist.at(index) ) if and only if one of the output is valid.
     *
     * If the file is remote, it will play it in streaming, so a few seconds to cache it are to be expected.
     * Remote files will use a QThread to check on the channel state repeatedly until the status is playing,
     * and then it will send the signal to auto-hold PTT.
     * If the remote file is .ogg, the duration is somehow flawed by -1 second, so we manually add a second
     * for ogg or flacs files.
     *
     * If the file is local, it will just play it and won't use a QThread to start auto-holding push to talk
     * since it is instantly played.
     *
     * It will also starts a QTimer::singleShot to pop_front the arrays containing the channel indexes.
     *
     * \param index
     * \return The duration of the sound in seconds, account for ogg/flac glitch.
     */
    double PlayAt(int index);


    /*!
     * \brief resetShouldPlay will reset the _shouldPlay boolean to true;
     *
     * It will stop all the timers and allows playing sounds again.
     * (The _shouldPlay boolean isn't used if the playback mode is Singleton Cancer).
     */
    void resetShouldPlay();

    //need the files as an array
    QVector<LIDL::SoundFile*> _soundList; /*!< Vector containing the soundlist.*/
    LIDL::Playback _playMode; /*!< Playmode, either Singleton, Singleton Cancer, Sequential, Sequential auto.*/
    int _index; /*!< The index of the next song to be played.*/

    // Channels

    QVector<unsigned long> _mainChannel; /*!< The array containing the handles to the channels being played.*/
    QVector<unsigned long> _vacChannel; /*!< */



    // Devices numbers
    int _mainOutputDevice; /*!< The index of the main output device.*/
    int _VACOutputDevice; /*!< The index of the main output device.*/



    // Timers that needs to be canceled if the sound is stopped
    QTimer * _timerShouldPlay; /*!< Timer to reset the _shouldPlay boolean (calls the resetShouldPlay method).*/
    QTimer * _timerSequentialAutoPlay;/*!< Timer to know when to auto-play next song.*/






    bool _shouldPlay; /*!<Boolean to know  whether this player is allowed to play next song or not.*/
};


#endif // CUSTOMPLAYER_H
