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
namespace LIDL{
namespace Controller {



class SettingsController : public QObject
{
    Q_OBJECT
public:

    static SettingsController * GetInstance();

    // getters
    int GetDefaultMainVolume() const;
    int GetDefaultVacVolume() const;
    QString GetDefaultSoundboardFolder() const;
    QString GetDefaultSoundFolder() const;
    // Public methods
    bool OpenSettings();
    // Can still use constructor to set default valuues
    // Because it should be called before any other methods
    //void SetDefaultValues();
    void SaveSettings();

    QString GetLastOpenedSoundboard();
    std::deque<QFileInfo> GetRecentFiles();

    int GetRecentFilesCount() const;

    // Literally a snapshot whenever we open or save
    //             Sounds to be compared (including shorcuts) pointer to pttKeySequenceEdit and stop KeySequenceEdit
    void SaveState( QJsonObject object);
    bool SaveIsDifferentFrom( QJsonObject  newObject);

    bool IsThisFirstTimeUser();
    void unHoldPTT();
    void holdPTT(int duration);

    int CompareSaves(QJsonObject newObject);
    // showSFX, showNumber, showFullSoundList;

    void addShowFlag(LIDL::SHOW_SETTINGS addedFlag);
    void removeShowFlag(LIDL::SHOW_SETTINGS removedFlag);
    bool checkShowFlags(LIDL::SHOW_SETTINGS checkedFlag) const;
    LIDL::SHOW_SETTINGS getShowFlags() const;
    QStringList GetSupportedMimeTypes() const;
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


    QMap<QString, QString> _gameList; /*!< A Map between executable and "nice name" to be displayed in various part of the UI */

signals:
    /*!
     * \brief RecentFilesChanged Signal is caught by the main UI to changes the Open Recent Menu.
     */
    void RecentFilesChanged();
    void SettingsChanged();

public slots:

    /*!
     * \brief GetGameList
     * \return The QMap of the Game list
     */
    QMap<QString, QString> GetGameList() const;


    void SetDefaultMainVolume(int);
    void SetDefaultVacVolume(int);
    void SetRecentFileCount(int);
    void ShowSettingsWindow();

    void SetPTTScanCode(int sc);
    void SetPTTVirtualKey(int vk);



    // slots for button in the ui
    void buttonBrowseLidl();
    void buttonBrowseSound();
  //  void setPTTtimer(int duration);
    void addFileToRecent(QFileInfo fileInfo);

};
} //end namespace Controller
} //end namespace LIDL
//#endif // INCLUDESETTINGSCONTROLLER_H
