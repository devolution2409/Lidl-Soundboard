//#ifndef SOUNDWRAPPER_H
//#define SOUNDWRAPPER_H
#pragma once
//#include <QDebug>

#include <QObject>
#include <QFile>
#include <QVector>
#include <QShortcut>
#include <QKeySequence>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QFileInfo>
#include <QList>
#include <QStandardItem>

#include "CustomPlayer.h"
#include "EnumsAndStructs.h"
#include "CustomListWidget.h"
#include "CustomListWidgetItem.h"
#include "CustomSoundFile.h"

/*! \class SoundWrapper
  * \brief Inherits QObject
  *
  * Soundwrapper class contains all the data necessary to play one or several sounds,
  * as well as a player capable of playing them.
  */
class SoundWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SoundWrapper(QObject *parent = nullptr);
    // Constructor to be used from the add sound dialog
    SoundWrapper(CustomListWidget *soundList,
                 LIDL::Playback playbackMode,
                 QKeySequence * shortcut,
                 int virtualKey = -1,
                 QObject *parent = nullptr   );

// Constructor for when we OPEN a LIDLJSON soundboard json file
    SoundWrapper( QVector<LIDL::SoundFile*> fileList,
                 LIDL::Playback playbackMode,
                 QKeySequence  shortcut,
                 int shortcutVirtualKey =-1,
                 int mainOutput = -1,
                 int vacOutput = -1,
                 QObject *parent = nullptr   );


///ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE

// CONSTRUCTOR FOR EXP JSON
SoundWrapper(QVector<QString> fileList,LIDL::Playback playbackMode,int mainVolume, int vacVolume, int mainOutput, int vacOutput,QObject * parent=nullptr);
    //Getters
    QVector<LIDL::SoundFile*> getSoundList();
    QKeySequence getKeySequence();
    LIDL::Playback getPlayMode();
    QString getSoundListAsQString();
    QList<QStandardItem*> getSoundAsItem();

    //Setters
    int addSound(QString filename, float mainVolume = 1.0, float vacVolume = 1.0);
    int removeSoundAt(int);
    int setKeySequence(QKeySequence);
    int setPlayMode(LIDL::Playback);

    void setPlayerMainOutput(int);
    void setPlayerVACOutput(int);
    int getVacDevice();

    int getMainDevice();


    int getShortcutVirtualKey();



private:

     QVector<LIDL::SoundFile*> _soundList; /*!< Vector containing the files.*/

    LIDL::Playback _playMode; /*!< Variable holding the playback mode (enumeration defined in EnumsAndStruct.h .*/

    // shortcut
    QKeySequence _keySequence; /*!< Key Sequence of the shortcut playing the sound. Can be empty. Used for displayed purposes. */
    int _virtualKey; /*!< Virtual Key of the shortcut playing the sound. Needed to register it into windows API.*/
    // the player to play the sounds
    CustomPlayer * _player; /*!< Player that will play the sound according to their volume, SFX, and playback settings.*/

    bool checkFileExistence(QString fileName);

    // need this to check for soundboard modification forsenT
    // will implement SettingsController:WasModified() or something
    /*!
     * \brief Deprecated.
     *
     * \deprecated Used to be used to detect modification to prompt the user to save. Now, we compare entry save file, and what would be the output save file.
     * \param a The soundwrapper to be compared against.
     * \param b The second soundwrapper.
     */
    friend bool operator==(const SoundWrapper & a, const SoundWrapper & b);
    /*!
    * \brief Deprecated.
    *
    * \deprecated Used to be used to detect modification to prompt the user to save. Now, we compare entry save file, and what would be the output save file.
    * \param a The soundwrapper to be compared against.
    * \param b The second soundwrapper.
    */
    friend bool operator!=(const SoundWrapper & a, const SoundWrapper & b); /*!< */
signals:
    /*!
     * \brief Used to send warning to main ui that a file doesn't exist.
     *
     * This function will check if the local file exists (when opening a soundboard).
     * It will also check if a remote file exists using a tcp/tsl socket to check the MIME type.
     */
    void UnexistantFile();


    /*!
     * \brief Used to send a warning to main ui that an error happened playing a file.
     *
     */
    void ErrorPlaying(QString);

    /*!
     * \brief Used to send a message to main ui that a file is playing
     *
     */
    void NowPlaying(QString);

    /*!
     * \brief This signal is sent whenever the  "holdPTT" signal of the player is sent.
     *
     * It is used to tell the settingscontroller to hold the PTT key for the duration of the sound played.
     */
    void holdPTTProxy(int);

public slots:
    /*!
     * \brief Tell the player to play the sound according to the playback mode.
     */
    void Play();

    /*!
     * \brief Tell the player to stop playing the sound.
     */
    void Stop();

    /*!
     * \brief Set the main output device of the player according to the combo box in the UI.
     */

    void OutputDeviceChanged(int);
    /*!
     * \brief Set the VAC output device of the player according to the combo box in the UI.
     */
    void VACDeviceChanged(int);

    /*!
     * \brief Proxy for the player now playing signal.
     *
     * Redirect the signal of the player to the main UI.
     */
    void playerNowPlaying(QString);

    /*!
     * \brief Proxy for the player error playing signal.
     *
     * Redirect the signal of the player to the main UI.
     */
    void playerErrorPlaying(QString);

    /*!
     * \brief Clears the shortcut of the wrapper. (Display purposes)
     *
     * Will get called whenever clearing the shortcut is needed.
     * Swap the existing key sequence with a blank one.
     */
    void clearShorcut();


};

//#endif // SOUNDWRAPPER_H
