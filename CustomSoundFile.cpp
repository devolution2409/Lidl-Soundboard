#include "CustomSoundFile.h"
namespace LIDL {



SoundFile::SoundFile() : QUrl()
{

}


SoundFile::SoundFile(const QString &name, float mainVolume, float vacVolume) : 	QUrl(name)
{
    _mainVolume = mainVolume;
    _vacVolume  = vacVolume;
}
SoundFile::SoundFile(const QString &name, float mainVolume, float vacVolume, LIDL::SFX SFX) :
    SoundFile(name,mainVolume,vacVolume)
{
    this->sfx = SFX;
}

float SoundFile::getMainVolume() const
{
    return _mainVolume;
}

float SoundFile::getVacVolume() const
{
    return _vacVolume;
}

bool SoundFile::exists() const
{
//    qDebug() << "scheme for this url:" << this->scheme();
//    qDebug() << "url: " << this->url().toUtf8();
    if (this->scheme() == "file")
    {
        QFileInfo info(this->toLocalFile());
        if (info.exists())
                return true;
        return false;
    }
    else if (this->scheme() == "http")
    {
        QTcpSocket socket;
        socket.connectToHost(this->host() , 80);
        if (socket.waitForConnected())
        {
            socket.write("HEAD " + this->path().toUtf8() + " HTTP/1.1\r\n"
            "Host: " + this->host().toUtf8() + "\r\n"
            "\r\n");
            if (socket.waitForReadyRead())
            {
                QByteArray bytes = socket.readAll();
                if (bytes.contains("200 OK"))
                    return true;
            }
        }
        return false;
    }
    else if (this->scheme() == "https")
    {
        // https is 443, and we need a SSL socket instead of a reguler TCP socket
        // (SSLsocket is derived from tcp socket)
        QSslSocket socket;
        socket.connectToHostEncrypted(this->host() , 443);
        if (socket.waitForConnected())
        {

            qDebug() << "wutfpoezrjfopez";
            socket.write("HEAD " + this->path().toUtf8() + " HTTP/1.1\r\n"
            "Host: " + this->host().toUtf8() + "\r\n"
            "\r\n");
            if (socket.waitForReadyRead())
            {
                QByteArray bytes = socket.readAll();
                if (bytes.contains("200 OK"))
                    return true;
            }
        }
        return false;
    }
    else if (this->scheme() == "ftp")
    {
        // and ANOTHER ONE (port)
    }
    return false;
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

LIDL::SFX SoundFile::getSFX() const
{
    return sfx;
}

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
//    if (a.getSFX().distortion != b.getSFX().distortion)
 //       return false;
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

