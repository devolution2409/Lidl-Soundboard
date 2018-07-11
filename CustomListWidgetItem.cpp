#include "CustomListWidgetItem.h"

CustomListWidgetItem::CustomListWidgetItem(QListWidget *parent) : QListWidgetItem(parent)
{
    //setting default volume to 100%
    _mainVolume = 1;
    _vacVolume  = 1;
    _sfx.flags = LIDL::SFX_TYPE::NONE;
}

void CustomListWidgetItem::setMainVolume(float newMainVolume)
{
    _mainVolume = newMainVolume;
}

void CustomListWidgetItem::setVacVolume(float newVacVolume)
{
    _vacVolume  = newVacVolume;
}

//! This constructor will attempt to deduct a valid
//! URI scheme from the string. If it is local it will prepend file:///
CustomListWidgetItem::CustomListWidgetItem(const QString & text,float mainVolume,
                                           float vacVolume,unsigned long long size,QListWidget * parent, int type)
    : QListWidgetItem(QUrl::fromUserInput(text).toString(),parent,type)
{
    _mainVolume = mainVolume;
    _vacVolume  = vacVolume;
    _sfx.flags  = LIDL::SFX_TYPE::NONE;
    _size       = size;
}


CustomListWidgetItem::CustomListWidgetItem(const QString & text,float mainVolume,
                                           float vacVolume, LIDL::SFX sfx,unsigned long long size,QListWidget * parent, int type)
    : CustomListWidgetItem(text,mainVolume,vacVolume,size,parent,type)
{
    this->_sfx = sfx;
}



float CustomListWidgetItem::getMainVolume()
{
    return _mainVolume;
}
float CustomListWidgetItem::getVacVolume()
{
    return _vacVolume;
}
//void CustomListWidgetItem::setSFXDistortionDefault()
//{
//    _sfx.distortion.fGain = -18 ;
//    _sfx.distortion.fEdge = 15;
//    _sfx.distortion.fPostEQCenterFrequency = 2400;
//    _sfx.distortion.fPostEQBandwidth = 2400;
//    _sfx.distortion.fPreLowpassCutoff = 8000;

//}

void CustomListWidgetItem::setSFXDistortion(LIDL::SFX_DIST_PARAM param, int value)
{
    float fValue = static_cast<float>(value);
    switch(param){

    case LIDL::SFX_DIST_PARAM::fEdge:
        _sfx.distortion.fEdge = fValue;
        break;

    case LIDL::SFX_DIST_PARAM::fGain:
        _sfx.distortion.fGain = fValue;
        break;

    case LIDL::SFX_DIST_PARAM::fPostEQBandwidth:
         _sfx.distortion.fPostEQBandwidth    = fValue;
        break;

    case LIDL::SFX_DIST_PARAM::fPostEQCenterFrequency:
        _sfx.distortion.fPostEQCenterFrequency = fValue;
        break;

    case LIDL::SFX_DIST_PARAM::fPreLowpassCutoff:
        _sfx.distortion.fPreLowpassCutoff = fValue;
        break;

    default:
        qDebug() << "Wrong usage of setSFXDistortion";
        return;
        break;
    }
}

LIDL::SFX CustomListWidgetItem::getSFX()
{
    return _sfx;
}

//void CustomListWidgetItem::SetDistortionEnabled(bool newState)
//{
//    if (newState)
//        _sfx.flags  |= LIDL::SFX_TYPE::DISTORTION;
//    else
//        _sfx.flags &=  ~LIDL::SFX_TYPE::DISTORTION;
//}
void CustomListWidgetItem::SetSFXEnabled(LIDL::SFX_TYPE type, bool enabled)
{
    if (enabled)
       _sfx.flags  |= type;
    else
       _sfx.flags &= ~type;
}

//http://www.un4seen.com/doc/#bass/BASS_DX8_CHORUS.html
//http://bass.radio42.com/help/html/f23be39f-2720-aca0-9b58-ef3a54af2c34.htm
void CustomListWidgetItem::setSFXChorus(LIDL::SFX_CHORUS_PARAM param, int value)
{
    float fValue = static_cast<float>(value);

    switch(param){

    case LIDL::SFX_CHORUS_PARAM::fDelay:
        _sfx.chorus.fDelay = fValue;
        break;

    case LIDL::SFX_CHORUS_PARAM::fDepth:
        _sfx.chorus.fDepth = fValue;
        break;

    case LIDL::SFX_CHORUS_PARAM::fFeedback:
         _sfx.chorus.fFeedback    = fValue;
        break;

    case LIDL::SFX_CHORUS_PARAM::fFrequency:
        _sfx.chorus.fFrequency = fValue;
        break;

    case LIDL::SFX_CHORUS_PARAM::fWetDryMix:
        _sfx.chorus.fWetDryMix = fValue;
        break;

    case LIDL::SFX_CHORUS_PARAM::lPhase:
        _sfx.chorus.lPhase = value; // is enumeration Brokeback (therefore int)
        break;
    case LIDL::SFX_CHORUS_PARAM::lWaveform:
        _sfx.chorus.lWaveform = static_cast<bool>(value);
        break;

    default:
        qDebug() << "Wrong usage of setSFXDistortion";
        return;
        break;
    }
}


void CustomListWidgetItem::setSFXEcho(LIDL::SFX_ECHO_PARAM param, int value)
{
    float fValue = static_cast<float>(value);

    switch(param){

    case LIDL::SFX_ECHO_PARAM::fFeedback:
        _sfx.echo.fFeedback = fValue;
        break;

    case LIDL::SFX_ECHO_PARAM::fLeftDelay:
        _sfx.echo.fLeftDelay = fValue;
        break;

    case LIDL::SFX_ECHO_PARAM::fRightDelay:
        _sfx.echo.fRightDelay = fValue;
        break;
    case LIDL::SFX_ECHO_PARAM::fWetDryMix:
        _sfx.echo.fWetDryMix = fValue;
        break;
    case LIDL::SFX_ECHO_PARAM::lPanDelay:
        _sfx.echo.lPanDelay = static_cast<bool>(value);
        break;
    default:
        qDebug() << "Wrong usage of setSFXEcho";
        return;
        break;
    }
}


void CustomListWidgetItem::setSFXCompressor(LIDL::SFX_COMPRESSOR_PARAM param, int value)
{
    float fValue = static_cast<float>(value);

    switch(param){

    case LIDL::SFX_COMPRESSOR_PARAM::fAttack:
        _sfx.compressor.fAttack = fValue;
        break;

    case LIDL::SFX_COMPRESSOR_PARAM::fGain:
        _sfx.compressor.fGain = fValue;
        break;

    case LIDL::SFX_COMPRESSOR_PARAM::fPredelay:
        _sfx.compressor.fPredelay = fValue;
        break;

    case LIDL::SFX_COMPRESSOR_PARAM::fRatio:
        _sfx.compressor.fRatio = fValue;
        break;

    case LIDL::SFX_COMPRESSOR_PARAM::fRelease:
        _sfx.compressor.fRelease = fValue;
        break;

    case LIDL::SFX_COMPRESSOR_PARAM::fThreshold:
        _sfx.compressor.fThreshold = fValue;
        break;
    default:
        qDebug() << "Wrong usage of setSFXCompressor";
        return;
        break;
    }
}

void CustomListWidgetItem::setSFXFlanger(LIDL::SFX_FLANGER_PARAM param, int value)
{
    float fValue = static_cast<float>(value);

    switch(param){

    case LIDL::SFX_FLANGER_PARAM::fDelay:
        _sfx.flanger.fDelay = fValue;
        break;

    case LIDL::SFX_FLANGER_PARAM::fDepth:
        _sfx.flanger.fDepth = fValue;
        break;

    case LIDL::SFX_FLANGER_PARAM::fFeedback:
        _sfx.flanger.fFeedback = fValue;
        break;

    case LIDL::SFX_FLANGER_PARAM::fFrequency:
        _sfx.flanger.fFrequency = fValue;
        break;
    case LIDL::SFX_FLANGER_PARAM::fWetDryMix:
        _sfx.flanger.fWetDryMix = fValue;
        break;
    case LIDL::SFX_FLANGER_PARAM::lPhase:
        _sfx.flanger.lPhase = value;
        break;
    case LIDL::SFX_FLANGER_PARAM::lWaveform:
        _sfx.flanger.lWaveform = static_cast<bool>(value);
        break;
    default:
        qDebug() << "Wrong usage of setSFXFlanger";
        return;
        break;
    }
}

void CustomListWidgetItem::setSFXGargle(LIDL::SFX_GARGLE_PARAM param, int value)
{
    switch(param){

    case LIDL::SFX_GARGLE_PARAM::dwRateHz:
        _sfx.gargle.dwRateHz = value;
        break;
    case LIDL::SFX_GARGLE_PARAM::dwWaveShape:
        _sfx.gargle.dwWaveShape = value;
        break;
    default:
        qDebug() << "Wrong usage of setSFXGargle";
        return;
        break;

    }
}

int CustomListWidgetItem::getSFXEcho(LIDL::SFX_ECHO_PARAM which) const
{
    switch(which){

    case LIDL::SFX_ECHO_PARAM::fFeedback:
        return static_cast<int>(_sfx.echo.fFeedback);
        break;

    case LIDL::SFX_ECHO_PARAM::fLeftDelay:
        return static_cast<int>(_sfx.echo.fLeftDelay);
        break;

    case LIDL::SFX_ECHO_PARAM::fRightDelay:
        return static_cast<int>(_sfx.echo.fRightDelay);
        break;
    case LIDL::SFX_ECHO_PARAM::fWetDryMix:
        return static_cast<int>(_sfx.echo.fWetDryMix);
        break;
    case LIDL::SFX_ECHO_PARAM::lPanDelay:
        return static_cast<int>(_sfx.echo.lPanDelay);
        break;
    default:
        qDebug() << "Wrong usage of setSFXEcho";
        return -1;
        break;
    }
}




//void CustomListWidgetItem::setSFXChorusDefault()
//{
//    _sfx.chorus.fDelay = 10;
//    _sfx.chorus.fDepth = 25;
//    _sfx.chorus.fFeedback = 2;
//    _sfx.chorus.fWetDryMix = 70;
//    _sfx.chorus.lPhase = 0;
//    _sfx.chorus.lWaveform = 1;
//}

int CustomListWidgetItem::getSFXChorus(LIDL::SFX_CHORUS_PARAM which) const
{

    switch(which){

    case LIDL::SFX_CHORUS_PARAM::fDelay:
        return static_cast<int>(_sfx.chorus.fDelay);
        break;

    case LIDL::SFX_CHORUS_PARAM::fDepth:
        return static_cast<int>(_sfx.chorus.fDepth);
        break;

    case LIDL::SFX_CHORUS_PARAM::fFeedback:
        return static_cast<int>(_sfx.chorus.fFeedback);
        break;

    case LIDL::SFX_CHORUS_PARAM::fFrequency:
        return static_cast<int>(_sfx.chorus.fFrequency);
        break;

    case LIDL::SFX_CHORUS_PARAM::fWetDryMix:
        return static_cast<int>(_sfx.chorus.fWetDryMix);
        break;

    case LIDL::SFX_CHORUS_PARAM::lPhase:
        return static_cast<int>(_sfx.chorus.lPhase); // is enumeration Brokeback (therefore int)
        break;
    case LIDL::SFX_CHORUS_PARAM::lWaveform:
        return static_cast<int>(_sfx.chorus.lWaveform);
        break;

    default:
        qDebug() << "Wrong usage of getSFXChorus";
        return -1;
        break;
    }

}

int CustomListWidgetItem::getSFXDistortion(LIDL::SFX_DIST_PARAM which) const
{
    switch(which){

    case LIDL::SFX_DIST_PARAM::fEdge:
        return static_cast<int>(_sfx.distortion.fEdge);
        break;

    case LIDL::SFX_DIST_PARAM::fGain:
        return static_cast<int>(_sfx.distortion.fGain);
        break;

    case LIDL::SFX_DIST_PARAM::fPostEQBandwidth:
        return static_cast<int>(_sfx.distortion.fPostEQBandwidth);
        break;

    case LIDL::SFX_DIST_PARAM::fPostEQCenterFrequency:
        return static_cast<int>(_sfx.distortion.fPostEQCenterFrequency);
        break;

    case LIDL::SFX_DIST_PARAM::fPreLowpassCutoff:
        return static_cast<int>(_sfx.distortion.fPreLowpassCutoff);
        break;

    default:
        qDebug() << "Wrong usage of getSFXDistortion";
        return -1;
        break;
    }



}

unsigned long long CustomListWidgetItem::getSize() const
{
    return this->_size;
}
//void CustomListWidgetItem::CheckForIllegalValues()
//{




//}

