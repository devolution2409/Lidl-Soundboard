#include "CustomSoundFile.h"
namespace LIDL {



SoundFile::SoundFile() : QUrl()
{

}

SoundFile::SoundFile(const QString &name, int mainVolumeBase100,  int vacVolumeBase100, unsigned long long size) : QUrl(name)
{
    _mainVolume = static_cast<float>(mainVolumeBase100/100);
    _vacVolume  = static_cast<float>(vacVolumeBase100/100);
    _size       = size;
}



SoundFile::SoundFile(const QString &name, float mainVolume, float vacVolume, unsigned long long size) : 	QUrl(name)
{
    _mainVolume = mainVolume;
    _vacVolume  = vacVolume;
    _size       = size;
}
SoundFile::SoundFile(const QString &name, float mainVolume, float vacVolume, LIDL::SFX SFX, unsigned long long size) :
    SoundFile(name,mainVolume,vacVolume,size)
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

unsigned long long SoundFile::getSize() const
{
    return this->_size;
}



LIDL::SFX SoundFile::getSFX() const
{
    return sfx;
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



} // end namespace LIDL

// Ok so, we declare operator== as a friend to this class
// IN the namespace
// but we implement it OUTSIDE the namespace else ADL doesn't work forsenT
//bool operator==(const LIDL::SoundFile &a,const  LIDL::SoundFile &b)

