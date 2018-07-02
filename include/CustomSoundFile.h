#ifndef SOUNDFILE_H
#define SOUNDFILE_H
#include <utility>

#include <QFile>
#include <QDebug>
#include "lib/bass.h"
#include "EnumsAndStructs.h"
#include <QFileInfo>
#include <QUrl>
#include <QTcpSocket>
#include <QSslSocket>

namespace LIDL {


class SoundFile : public QUrl
{
public:
    SoundFile();
    SoundFile(const QString &name, int mainVolumeBase100 = 100,  int vacVolumeBase100 = 100);
    SoundFile(const QString &name, float mainVolume = 1.0, float vacVolume = 1.0);
    SoundFile(const QString &name, float mainVolume, float vacVolume, LIDL::SFX SFX);
public:
    float getMainVolume() const;
    float getVacVolume() const;
    // WE JAVA NOW
    // HYPERDANSGAME
    bool IsEqualTo(const LIDL::SoundFile &other) const;
    LIDL::SFX getSFX() const;
    bool exists() const;

private:
    float _mainVolume;
    float _vacVolume;
    friend bool operator==(const LIDL::SoundFile &a, const LIDL::SoundFile &b);
    friend bool operator!=(const LIDL::SoundFile &a, const LIDL::SoundFile &b);
    // ajouter les putain de sound effect

    // sfx struct
    LIDL::SFX sfx;
};


}//end namespace lidl



#endif // SOUNDFILE_H
