#ifndef CUSTOMPLAYER_H
#define CUSTOMPLAYER_H

#include <QObject>
#include <QVector>
#include <QFile>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QDebug>

class CustomPlayer : public QObject
{
    Q_OBJECT
public:
    explicit CustomPlayer(QObject *parent = nullptr);
    CustomPlayer(QVector<QFile*> soundList,int playMode);

signals:

public slots:
   // void PlayFirst();
    void PlayAt(int index);
    void PlayNext();
private:
    // we only need one audio pointer
    QAudioOutput    * _audio;
    // but we need the files as an array
    QVector<QFile*> _soundList;
    int _playMode;
    int _index;


};

#endif // CUSTOMPLAYER_H
