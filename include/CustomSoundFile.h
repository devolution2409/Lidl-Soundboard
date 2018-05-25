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
    float getMainVolume() const;
    float getVacVolume() const;
    // WE JAVA NOW
    // HYPERDANSGAME
    bool IsEqualTo(const LIDL::SoundFile &other) const;
private:
    float _mainVolume;
    float _vacVolume;
    friend bool operator==(const LIDL::SoundFile &a, const LIDL::SoundFile &b);
    // doesn't work? forsenY

};
// doesnt work pajaL

}//end namespace lidl



#endif // SOUNDFILE_H
