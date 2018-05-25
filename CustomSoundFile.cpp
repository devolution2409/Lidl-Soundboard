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


float SoundFile::getMainVolume() const
{
    return _mainVolume;
}

float SoundFile::getVacVolume() const
{
    return _vacVolume;
}


//bool SoundFile::IsEqualTo(const LIDL::SoundFile &other) const
//{
//    // comparing file name
//    if (this->fileName() != other.fileName())
//        return false;
//    // Checking main volume
//    if ( this->getMainVolume() != other.getMainVolume() )
//        return false;
//    // Checking vac volume
//    if (this->getVacVolume() != other.getVacVolume())
//        return false;

//    return true;
//}

} // end namespace LIDL

// Ok so, we declare operator== as a friend to this class
// IN the namespace
// but we implement it OUTSIDE the namespace else ADL doesn't work forsenT
bool operator==(const LIDL::SoundFile &a,const  LIDL::SoundFile &b)
{
    if (a.fileName() != b.fileName())
        return false;
    // Checking main volume
    if ( a.getMainVolume() != b.getMainVolume() )
        return false;
    // Checking vac volume
    if (a.getVacVolume() != b.getVacVolume())
        return false;
    return true;
}
bool operator!=(const LIDL::SoundFile& a,const LIDL::SoundFile& b)
{
    // need to cast a and b to const references object even tho they are already const and references forsenT
    // Somehow we cant do this:
    // return ! (a==b);
    // because: error: call of overloaded 'operator==(const LIDL::SoundFile&, const LIDL::SoundFile&)' is ambiguous
    //candidate: bool operator==(const LIDL::SoundFile&, const LIDL::SoundFile&)
    //forsenT

    if (a.fileName() != b.fileName())
        return true;
    // Checking main volume
    if ( a.getMainVolume() != b.getMainVolume() )
        return true;
    // Checking vac volume
    if (a.getVacVolume() != b.getVacVolume())
        return true;
    return false;
}


// CANT OVERLOAD OPERATOR== WHY GOD WHY
//
//bool operator==( LIDL::SoundFile &a, LIDL::SoundFile &b )
//{
//    // comparing file name
//    if (a.fileName() != b.fileName())
//        return false;
//    // Checking main volume
//    if (a.getMainVolume() != b.getMainVolume() )
//        return false;
//    // Checking vac volume
//    if (a.getVacVolume() != b.getVacVolume())
//        return false;

//    return true;
//}



/*bool operator!=( LIDL::SoundFile &a, LIDL::SoundFile &b )
{
    // return the opposite of operator==
    return ! (a==b);
}*/

