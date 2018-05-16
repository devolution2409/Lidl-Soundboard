#include "CustomSoundFile.h"
namespace LIDL {



SoundFile::SoundFile() : QFile()
{

}


SoundFile::SoundFile(const QString &name, float mainVolume, float vacVolume) : 	QFile(name)
{
    _mainVolume = mainVolume;
    _vacVolume  = vacVolume;
}


float SoundFile::getMainVolume()
{
    return _mainVolume;
}

float SoundFile::getVacVolume()
{
    return _vacVolume;
}

}
