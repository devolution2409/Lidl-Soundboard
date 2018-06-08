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


CustomListWidgetItem::CustomListWidgetItem(const QString & text,float mainVolume,
                                           float vacVolume,QListWidget * parent, int type)
    : QListWidgetItem(text,parent,type)
{
    _mainVolume = mainVolume;
    _vacVolume  = vacVolume;
    _sfx.flags  = LIDL::SFX_TYPE::NONE;
}


CustomListWidgetItem::CustomListWidgetItem(const QString & text,float mainVolume,
                                           float vacVolume, LIDL::SFX sfx ,QListWidget * parent, int type)
    : CustomListWidgetItem(text,mainVolume,vacVolume,parent,type)
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
void CustomListWidgetItem::setSFXDistortionDefault()
{
    _sfx.distortion.fGain = -18 ;
    _sfx.distortion.fEdge = 15;
    _sfx.distortion.fPostEQCenterFrequency = 2400;
    _sfx.distortion.fPostEQBandwidth = 2400;
    _sfx.distortion.fPreLowpassCutoff = 8000;

}

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

LIDL::SFX CustomListWidgetItem::GetSFX()
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

        _sfx.chorus.lPhase = value;
        break;
    case LIDL::SFX_CHORUS_PARAM::lWaveform:
        _sfx.chorus.lWaveform;
        break;

    default:
        qDebug() << "Wrong usage of setSFXDistortion";
        return;
        break;
    }
}

void CustomListWidgetItem::setSFXChorusDefault()
{
    _sfx.chorus.fDelay = 0;
    _sfx.chorus.fDepth = 25;
    _sfx.chorus.fFeedback = 0;
    _sfx.chorus.fWetDryMix = 50;
    _sfx.chorus.lPhase = 2;
    _sfx.chorus.lWaveform = 1;
}

