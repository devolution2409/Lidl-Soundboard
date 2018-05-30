#include "CustomListWidgetItem.h"

CustomListWidgetItem::CustomListWidgetItem(QListWidget *parent) : QListWidgetItem(parent)
{
    //setting default volume to 100%
    _mainVolume = 1;
    _vacVolume  = 1;
    _sfx.distortionEnabled = false;
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
    _sfx.distortionEnabled = false;
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
        qDebug() << fValue;
        break;

    case LIDL::SFX_DIST_PARAM::fGain:
        _sfx.distortion.fGain = fValue;
            qDebug() << fValue;
        break;

    case LIDL::SFX_DIST_PARAM::fPostEQBandwidth:
         _sfx.distortion.fPostEQBandwidth    = fValue;
             qDebug() << fValue;
        break;

    case LIDL::SFX_DIST_PARAM::fPostEQCenterFrequency:
        _sfx.distortion.fPostEQCenterFrequency = fValue;
     qDebug() << "wtf" << fValue;
        break;

    case LIDL::SFX_DIST_PARAM::fPreLowpassCutoff:
        _sfx.distortion.fPreLowpassCutoff = fValue;
  qDebug() << "zullul " <<fValue;
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

void CustomListWidgetItem::SetDistortionEnabled(bool newState)
{
    _sfx.distortionEnabled = newState;
}
bool CustomListWidgetItem::GetDistortionEnabled()
{
    return _sfx.distortionEnabled;
}

BASS_DX8_DISTORTION CustomListWidgetItem::GetDistortionParams()
{
    return _sfx.distortion;
}
