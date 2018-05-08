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
class SoundWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SoundWrapper(QObject *parent = nullptr);
    // Constructor to be used from the add sound dialog
    SoundWrapper(QListWidget *soundList,
                 int playbackMode,
                 QKeySequence * shortcut,
                 QObject *parent = nullptr   );


    SoundWrapper(QListWidget *soundList,
                 int playbackMode,
                 QKeySequence * shortcut,
                 int virtualKey,
                 QObject *parent = nullptr   );

    //Accesseurs
    // getSoundList renvoie un tableau de pointeurs
    QVector<QFile*> getSoundList();
    QKeySequence getKeySequence();
    int getPlayMode();
    QString getSoundListAsQString();
    QList<QStandardItem*> getSoundAsItem();

    //Mutateurs
    int addSound(QString filename);
    int removeSoundAt(int);
    int setKeySequence(QKeySequence);
    int setPlayMode(int);

    void setPlayerPTTScanCode(int);
    void setPlayerPTTVirtualKey(int);

    void setPlayerMainOutput(int);
    void setPlayerVACOutput(int);
    int getShortcutVirtualKey();

private:
    // Vector to store the soundlist
     QVector<QFile*> _soundList;
    //  playblack mode
    // TODO: remplacer par une enum
    int _playMode;

    // shortcut
    QKeySequence _keySequence;
    int _virtualKey;
    // the player to play the sounds
    CustomPlayer * _player;
signals:

public slots:
    void Play();
    void OutputDeviceChanged(int);
    void VACDeviceChanged(int);
    void PTTVirtualKeyChanged(int);
    void PTTScanCodeChanged(int);

};

#endif // SOUNDWRAPPER_H
