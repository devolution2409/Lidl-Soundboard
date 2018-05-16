#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <QFile>
namespace LIDL {


class SoundFile : public QFile
{
public:
    SoundFile();
    SoundFile(const QString &name, float mainVolume = 1.0, float vacVolume = 1.0);

public:
    float getMainVolume();
    float getVacVolume();

private:
    float _mainVolume;
    float _vacVolume;
};


}
#endif // SOUNDFILE_H
