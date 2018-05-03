#include "CustomPlayer.h"

CustomPlayer::CustomPlayer(QObject *parent) : QObject(parent)
{

}

CustomPlayer::CustomPlayer(QVector<QFile*> soundList,int playMode)
{
    _soundList = soundList;
    _playMode  = playMode;
    _index = 0;
}


void CustomPlayer::PlayNext()
{
    this->playAt(_index++);
}


void CustomPlayer::PlayAt(int index)
{
     QAudioFormat format;
     // Set up the format, eg.
     format.setSampleRate(8000);
     format.setChannelCount(1);
     format.setSampleSize(8);
     format.setCodec("audio/pcm");
     format.setByteOrder(QAudioFormat::LittleEndian);
     format.setSampleType(QAudioFormat::UnSignedInt);
     QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
     if (!info.isFormatSupported(format))
     {
            qWarning() << "Raw audio format not supported by backend, cannot play audio.";
            return;
     }




}
