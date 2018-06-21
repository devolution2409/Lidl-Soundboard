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

namespace LIDL{


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
    void SaveState( QVector<SoundWrapper*> sounds, CustomShortcutEdit* pttEdit, CustomShortcutEdit* stopEdit );
    bool SaveIsDifferentFrom( QVector<SoundWrapper*> sounds, CustomShortcutEdit* pttEdit, CustomShortcutEdit* stopEdit       );

    bool IsThisFirstTimeUser();
    void unHoldPTT();
    void holdPTT(int duration);

    // showSFX, showNumber, showFullSoundList;

    void addShowFlag(LIDL::SHOW_SETTINGS addedFlag);
    void removeShowFlag(LIDL::SHOW_SETTINGS removedFlag);
    bool checkShowFlags(LIDL::SHOW_SETTINGS checkedFlag);
private:
    QString fileName;
    // Singleton class
    SettingsController();
    // the pointer will exist even if class isn't instancied yet
    // but we need to initalize it in the .cpp with a weird syntax
    static SettingsController * self;


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

        QVector<SoundWrapper *> savedSounds;
        CustomShortcutEdit savedPTT;
        CustomShortcutEdit savedStop;

    bool fileAlreadyExisted;


    /******************************************
     *              PTT TIMER                 *
     ******************************************/
    QTimer _activePttTimer;
    int  _activePttScanCode;
    int  _activePttVitualKey;

    // SHOW MENU SETTINGS
    LIDL::SHOW_SETTINGS _showFlags;

signals:
    void RecentFilesChanged();
    void SettingsChanged();

public slots:
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
    void addFile(QFileInfo fileInfo);

};
}
//#endif // INCLUDESETTINGSCONTROLLER_H
