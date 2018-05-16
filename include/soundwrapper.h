/*  Class containing the wrapper of a sound collection:
 *      Members:
 *          Attributes:
 *             Type                 Name                Description
 *
 *              QVector<QFile*>     _soundList          array of pointers to file:
 *              QKeySequence        _keySequence        the shortcut to play those sounds
 *              int _playMode       _playMode           the play type mode (sequential, circling, repeating etc)
 *
 *          Methods:
 *              addSound(QString filename);
 *              int removeSoundAt(int);
 *              int setKeySequence(QKeySequence);
 *              int setPlayMode(int);
 *
 * TODO: replace int _playMode by an enumeration
 */



#ifndef SOUNDWRAPPER_H
#define SOUNDWRAPPER_H

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
#include <CustomPlayer.h>
#include "EnumsAndStructs.h"
#include "CustomListWidget.h"
#include "CustomListWidgetItem.h"
#include "CustomSoundFile.h"

class SoundWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SoundWrapper(QObject *parent = nullptr);
    // Constructor to be used from the add sound dialog
    SoundWrapper(CustomListWidget *soundList,
                 LIDL::Playback playbackMode,
                 QKeySequence * shortcut,
                 QObject *parent = nullptr   );

// Constructor when we add a file from the sound property dialog
    SoundWrapper(CustomListWidget *soundList,
                 LIDL::Playback playbackMode,
                 QKeySequence * shortcut,
                 int virtualKey,
                 QObject *parent = nullptr   );

// Constructor for when we OPEN a soundboard json file
    SoundWrapper( QVector<LIDL::SoundFile*> fileList,
                 LIDL::Playback playbackMode,
                 QKeySequence  shortcut,
                 int shortcutVirtualKey =-1,
                 int mainOutput = -1,
                 int vacOutput = -1,
                 int pttVK =-1,
                 int pttSC = -1,
                 QObject *parent = nullptr   );

// Constructor when opening a EXP Json file
    SoundWrapper(QVector<QString> fileList,
                 int mainOutput = -1,
                 int vacOutput = -1,
                 QObject *parent = nullptr   );

SoundWrapper(QVector<QString> fileList,LIDL::Playback playbackMode,int mainOutput, int vacOutput,QObject * parent=nullptr);
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

    void setPlayerPTTScanCode(int);
    void setPlayerPTTVirtualKey(int);

    void setPlayerMainOutput(int);
    void setPlayerVACOutput(int);
    int getShortcutVirtualKey();



private:
    // Vector to store the soundlist
     QVector<LIDL::SoundFile*> _soundList;

  //  std::vector< std::map<QFile*, std::pair<float,float>>> _soundList;


    //  playblack mode
    // TODO: remplacer par une enum
    LIDL::Playback _playMode;

    // shortcut
    QKeySequence _keySequence;
    int _virtualKey;
    // the player to play the sounds
    CustomPlayer * _player;

    bool checkFileExistence(QString fileName);
signals:
    void UnexistantFile();
    // Signal to redirect signal from the player forsenT
    void ErrorPlaying(QString);
    void NowPlaying(QString);
public slots:
    void Play();
    void Stop();
    void OutputDeviceChanged(int);
    void VACDeviceChanged(int);
    void PTTVirtualKeyChanged(int);
    void PTTScanCodeChanged(int);

    void playerNowPlaying(QString);
    void playerErrorPlaying(QString);

};

#endif // SOUNDWRAPPER_H
