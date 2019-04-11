//#ifndef INCLUDESETTINGSCONTROLLER_H
//#define INCLUDESETTINGSCONTROLLER_H
#pragma once

#include <QString>
#include <QVector>
#include <QObject>
#include <QWidget>

#include <ui_Settings.h>
#include <QFileDialog>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QFileInfo>
#include <deque>
#include <algorithm>
#include <QMenu>
#include <QAction>
//#include <QDateTime>
// Class handling the settings
// we make it a singleton so we are gucci

#include "soundwrapper.h"
#include "CustomShortcutEdit.h"
#include <QTimer>
#include "windows.h"
#include <QJsonObject>
#include <QMessageBox>
#include "Profile.h"
#include "EnumsAndStructs.h"
#include <vector>

namespace LIDL{
namespace Controller {



class SettingsController : public QObject
{
    Q_OBJECT
public:

    static SettingsController * GetInstance();

    /*!
     * \brief GetDefaultMainVolume
     * \return The default main volume (either 100 or defined by the opened json)
     */
    int GetDefaultMainVolume() const;

    /*!
     * \brief GetDefaultVacVolume
     * \return The default VAC volume (either 100 or defined by the opened json)
     */
    int GetDefaultVacVolume() const;

    /*!
     * \brief GetDefaultSoundboardFolder
     * \return The default soundboard folder (either the folder where lidl soundboard is installed, or set by the opened json)
     */
    QString GetDefaultSoundboardFolder() const;

    /*!
     * \brief GetDefaultSoundFolder
     * \return The default soundboard folder (either the folder where lidl soundboard is installed, or set by the opened json)
     */
    QString GetDefaultSoundFolder() const;

    /*!
     * \brief OpenSettings
     *
     * Used to open the lidlsettings.json file, that contain several stuff such as default locations, default volumes, and recent files. Will probably contain profiles later on :)
     *
     * \return true if it was opened, false otherwise
     */
    bool OpenSettings();

    /*!
     * \brief SaveSettings
     *
     * Will save settings to lidlsettings.json
     *
     */
    void SaveSettings();

    /*!
     * \brief GetLastOpenedSoundboard
     * \return The most recent opened soundboard, so we can open it automatically for the user
     *
     * TODO: probably add an option to toggle this off
     *
     */
    QString GetLastOpenedSoundboard();

    /*!
     * \brief GetRecentFiles
     * \return A deck of the recently opened files
     */
    std::deque<QFileInfo> GetRecentFiles();

    /*!
     * \brief GetRecentFilesCount
     * \return The count of recent files stored inside lidlsettings
     */
    int GetRecentFilesCount() const;

    // Literally a snapshot whenever we open or save
    //             Sounds to be compared (including shorcuts) pointer to pttKeySequenceEdit and stop KeySequenceEdit

    /*!
     * \brief SaveState
     * \param object A json "snapshot" from the soundboard
     * TODO: account for profiles
     */
    void SaveState( QJsonObject object);

    /*!
     * \brief SaveIsDifferentFrom
     * \param newObject A json "snapshot" from the soundboard
     * \return Result of comparison between "old" snapshot and this one
     * TODO: account for profiles
     */
    bool SaveIsDifferentFrom( QJsonObject  newObject);

    /*!
     * \brief IsThisFirstTimeUser
     * \return true if first time user, so we show the welcome message
     */
    bool IsThisFirstTimeUser();

    /*!
     * \brief unHoldPTT
     *
     * Will unhold the push to talk (when using auto hold ptt)
     */
    void unHoldPTT();

    /*!
     * \brief holdPTT
     *
     * Will hold push to talk key for given duration.
     * If several sounds are played, it will hold it for the longest duration.
     * Interally this uses a timer that will be reset if (new duration > old duration)
     * \param duration The duration to hold the PTT for
     */
    void holdPTT(int duration);

    /*!
     * \brief CompareSaves
     * Will comparse old and new save and prompt user if a change was detected.
     * Seems a litle bit redundant with SaveIsDifferentFrom( QJsonObject  newObject);
     * TODO: is it tho?
     * \param newObject
     * \return eShrug
     */
    int CompareSaves(QJsonObject newObject);

    /*!
     * \brief addShowFlag
     * Will add a flag to the list of things to be displayed (stored as flags)
     * \param addedFlag
     */
    void addShowFlag(LIDL::SHOW_SETTINGS addedFlag);

    /*!
     * \brief removeShowFlag
     * Will remove a flag to the list of things to be displayed (stored as flags)
     * \param removedFlag
     */
    void removeShowFlag(LIDL::SHOW_SETTINGS removedFlag);

    /*!
     * \brief checkShowFlags
     * \param checkedFlag$
     * Will check if a flag is activated (using &)
     * \return
     */
    bool checkShowFlags(LIDL::SHOW_SETTINGS checkedFlag) const;

    /*!
     * \brief getShowFlags
     * Get the flags
     * \return the flags :)
     */
    LIDL::SHOW_SETTINGS getShowFlags() const;

    /*!
     * \brief GetSupportedMimeTypes
     * \return The list of supported MIME types for the player as a QStringList (QList<QString>)
     */
    QStringList GetSupportedMimeTypes() const;


    /*!
     * \brief GetDragAndDropSeveralWrappers
     * \return The behavior of the drag and drop
     */
    bool GetDragAndDropSeveralWrappers() const;

    /*!
      * \return bool
      */
    bool getEventProcessing() const;


    /*!
     * \brief isEditing
     * Is editing basically serves to know whether we should process shortcuts being pressed and play the sound,
     * or not because the user is editing a sound (or in another menu for instance)
     * The boolean will be set in the constructor of the soundwrapper (wrapperproperties) edition window, and unset in the destructor
     * \return boolean
     */
    bool isEditing() const;

    /*!
     * \brief setEditing
     * Allows shortcut to be dismissed if they are pressed while a user is editing a wrapper (see isEditing())
     */
    void setEditing(bool);

private:
    QString fileName;
    // Singleton class
    SettingsController();
    // the pointer will exist even if class isn't instancied yet
    // but we need to initalize it in the .cpp with a weird syntax
    static SettingsController * self;
    bool _isEditing;

    // Default folder to open sounboard (lidljson) files
    QString defaultSoundboardFolder;
    // Default folder to add sound
    QString defaultSoundsFolder;

    // Number of files to show in recent files (and to be saved)
    int recentFileCount;


    // Default volume for the add sound dialog (or the open exp soundboard)
    int defaultMainVolume;
    int defaultVacVolume;

    // QVector of recents opened file (either from the file or from after opening/saving a file)
    // todo: use std::deque
   // QVector<QFileInfo> recentFiles;
    std::deque<QFileInfo> recentFiles;
    // The pointer to the ui
    Ui::Settings * ui;
    // The recent menu pointer
    QMenu * _recentMenu;

    /******************************************
     *           DETECT MODIFICATIONS         *
     ******************************************/
     QJsonObject oldObject;
    bool fileAlreadyExisted;


    /******************************************
     *              PTT TIMER                 *
     ******************************************/
    QTimer _activePttTimer;
    int  _activePttScanCode;
    int  _activePttVitualKey;
    bool _eventProcessing; /*!< boolean to be tested in MainUI to know if events (key sequences) should be processed  */
    // SHOW MENU SETTINGS
    LIDL::SHOW_SETTINGS _showFlags;


    bool dragAndDropSeveralWrappers; /*!<Boolean holding how drag and drop several files will be handled (several wrapper or one) */


    std::vector<Profile> _profiles; /*!< Array of available profile, not a QVector because it doesnt work kek*/

signals:
    /*!
     * \brief RecentFilesChanged Signal is caught by the main UI to changes the Open Recent Menu.
     */
    void RecentFilesChanged();
    void SettingsChanged();

    void ProfileConfigurationChanged();

public slots:

    /*!
     * \brief GetProfiles
     * \return An array of profile (see Profile.h)
     */
    std::vector<Profile> GetProfiles() const;

    /*!
     * \brief AddProfile
     * Add a profile to the profiles array.
     * Copy mode is either nothing, copy sounds from another profile, or mirror sounds from another profile
     * \param profile
     */
    void AddProfile(Profile profile, LIDL::PROFILE_COPY_MODE copyMode = LIDL::PROFILE_COPY_MODE::NO_COPY);

    /*!
     * \brief ReplaceProfiles
     * Empty this
     * \param profiles
     */
    void ReplaceProfiles(std::vector<Profile> profiles);



    /*!
     * \brief AutoGameConfigurationChange
     * \param executable
     *
     * Changes the PTT and loaded sound based on the autodetected game
     */
 //   void AutoGameConfigurationChange(QString executable);

    /*!
     * \brief ManualGameConfigurationChanged
     * \param name
     *
     * Changes the same stuff but based on the selected thing by the user in the combobox
     */
    void ManualGameConfigurationChanged(const QString &name);

    /*!
     * \brief SetDefaultMainVolume
     * Sets the default main volume inside the controller, to be used when creating wrappers, and saved in the lidlsettings.
     */
    void SetDefaultMainVolume(int);

    /*!
     * \brief SetDefaultVacVolume
     * Sets the default VAC volume inside the controller, to be used when creating wrappers, and saved in the lidlsettings.
     */
    void SetDefaultVacVolume(int);

    void SetRecentFileCount(int);

    void ShowSettingsWindow();

    /*!
     * \brief SetPTTScanCode
     * Register the key to be auto-held while playing a sound
     * \param sc The key scan code
     */
    void SetPTTScanCode(int sc);

    /*!
     * \brief SetPTTVirtualKey
     * Register the key to be auto-held while playing a sound
     * \param vk The key virtual key
     */
    void SetPTTVirtualKey(int vk);



    /*!
     * \brief buttonBrowseLidl
     * Slot connected to a file browser button
     * TODO: refactor as a lambda
     */
    void buttonBrowseLidl();
    /*!
     * \brief buttonBrowseSound
     * Slot connected to a file browser button
     * TODO: refactor as a lambda
     */
    void buttonBrowseSound();

    /*!
     * \brief addFileToRecent
     * \param fileInfo
     */
    void addFileToRecent(QFileInfo fileInfo);

    /*!
     * \brief searchProfileForExe
     * \param exe
     */
    QString searchProfileForExe(QString exe);

};
} //end namespace Controller
} //end namespace LIDL
//#endif // INCLUDESETTINGSCONTROLLER_H
