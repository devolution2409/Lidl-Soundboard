
#include "PresetController.h"

namespace LIDL {

PresetController* PresetController::self = nullptr;

PresetController::PresetController(QObject *parent) : QObject(parent)
{
//    SFX_DIST_PARAM test;
//    _distortionPreset.insert("test",test);
//    _distortionPreset.insert("test2",test);
}


PresetController * PresetController::GetInstance()
{
    if (self == nullptr)
        self = new PresetController();

    return self;

}

QStringList  PresetController::GetExistingPresetsList(LIDL::SFX_TYPE type) const
{
    if (type == LIDL::SFX_TYPE::DISTORTION)
    {
        if ( ! _distortionPreset.keys().isEmpty())
            return QStringList( _distortionPreset.keys());
        return QStringList() << tr("No presets for distortion SFX !");
    }
    if (type == LIDL::SFX_TYPE::ECHO)
    {
        if ( ! _echoPreset.keys().isEmpty())
            return QStringList( _echoPreset.keys());
        return QStringList() << tr("No presets for echo SFX !");
    }
    if (type == LIDL::SFX_TYPE::CHORUS)
    {
        if ( ! _chorusPreset.keys().isEmpty())
            return QStringList( _chorusPreset.keys());
        return QStringList() << tr("No presets for chorus SFX !");
    }
    if (type == LIDL::SFX_TYPE::COMPRESSOR)
    {
        if ( ! _compressorPreset.keys().isEmpty())
            return QStringList( _compressorPreset.keys());
        return QStringList() << tr("No presets for compressor SFX !");
    }
    if (type == LIDL::SFX_TYPE::FLANGER)
    {
        if ( ! _flangerPreset.keys().isEmpty())
            return QStringList( _flangerPreset.keys());
        return QStringList() << tr("No presets for flanger SFX !");
    }
    if (type == LIDL::SFX_TYPE::GARGLE)
    {
        if ( ! _garglePreset.keys().isEmpty())
            return QStringList( _garglePreset.keys());
        return QStringList() << tr("No presets for gargle SFX !");
    }
    // none:
    return QStringList() << tr("No SFX selected !");
}

QStringList PresetController::GetExistingSFXList() const
{
    return ( QStringList()  << tr("Distortion")
                            << tr("Chorus")
                            << tr("Echo")
                            << tr("Compressor")
                            << tr("Flanger")
                            << tr("Gargle")
             );
}


}//end namespace LIDL
