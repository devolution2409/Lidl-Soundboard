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


    /*!
     * \brief Main constructor of sound wrappers.
     *
     *  Used to construct a soundwrapper object, used 99% of the time, except when opening EXP SOUNDBOARD json files.
     * \param fileList The wrapper's sound list. Each sound file is associated with two volumes (Main and VAC).
     * \param playbackMode The playback mode of this wrapper.
     * \param shortcut The keysequence to be displayed in the UI.
     * \param shortcutVirtualKey =-1, The shortcut's virtual key to be registered in the windows api.
     * \param mainOutput The index of the main output.
     * \param vacOutput The index of the VAC ouput
     * \param *parent A pointer to the parent (not used).
     */
    SoundWrapper(QVector<LIDL::SoundFile*> fileList,
                LIDL::Playback playbackMode,
                QKeySequence  shortcut,
                int shortcutVirtualKey =-1,
                int mainOutput = -1,
                int vacOutput = -1,
                QObject *parent = nullptr   );


///ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE ILL EAGLE

    /*!
     * \brief Main constructor of sound wrappers.
     *
     *  Used to construct a soundwrapper object when opening an EXP SOUNDBOARD json files.
     * \param fileList The wrapper's sound list. Each sound file is associated with two volumes (Main and VAC).
     * \param playbackMode The playback mode of this wrapper.
     * \param shortcut The keysequence to be displayed in the UI.
     * \param shortcutVirtualKey =-1, The shortcut's virtual key to be registered in the windows api.
     * \param mainOutput The index of the main output.
     * \param vacOutput The index of the VAC ouput
     * \param *parent A pointer to the parent (not used).
     */
    SoundWrapper(QVector<QString> fileList,
                LIDL::Playback playbackMode,
                int mainVolume,
                int vacVolume,
                int mainOutput,
                int vacOutput,
                QObject * parent=nullptr);



//Getters

    /*!
     * \return QVector<LIDL::SoundFile*> The sound list of the wrapper.
     */
    QVector<LIDL::SoundFile*> getSoundList();

    /*!
     * \return  QKeySequence
     */
    QKeySequence getKeySequence();

    /*!
     * \return  LIDL::Playback One of the playbackmode defined in "EnumsAndStruct.h".
     */
    LIDL::Playback getPlayMode();

    /*!
     * \return  QString Sound list as a QString. Sounds are separated by "\#n".
     */
    QString getSoundListAsQString();

    /*!
     * \return  QList<QStandardItem*> Sound list as QList of QStandarItems*. Each sound will take 5 slots in the list:
     * ListItem << Sound names << number of remotes files << SFX << Shortcut << Playback mode;
     */
    QList<QStandardItem*> getSoundAsItem();

    /*!
    * \brief Used by OpenEXPJson to add the revelant sound files.
    *
    * \param filename the file to be added.
    * \param mainVolume main output volume.
    * \param vacVolume VAC output volume.
    */
    int addSound(QString filename, float mainVolume = 1.0, float vacVolume = 1.0);



    /*! \brief Set the index of the main output device.
     *  \param index The new index
     */
    void setPlayerMainOutput(int index);

    /*! \brief Set the index of the VAC output device.
     *  \param index The new index
     */
    void setPlayerVACOutput(int index);
    /*!
     * \return int The index of the main output device.
     */
    int getMainDevice();

    /*!
     * \return int The index of the VAC output device.
     */
    int getVacDevice();

    /*!
     * \return int The shortcut virtual key.
     */
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
