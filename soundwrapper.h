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

#include <QObject>
#include <QFile>
#include <QVector>
#include <QShortcut>
#include <QKeySequence>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QList>
#include <QStandardItem>
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


private:
    // Vector pour stocker la liste des fichiers.
    // Ce tableau est un tableau de pointeur
     QVector<QFile*> _soundList;
    // mode de lecture
    // TODO: remplacer par une enum
    int _playMode;

    // Raccourci sous forme de QKeySequence
    QKeySequence _keySequence;


signals:

public slots:

};

#endif // SOUNDWRAPPER_H
