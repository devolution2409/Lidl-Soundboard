
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


void PresetController::AddPreset(QString name, BASS_DX8_DISTORTION preset, bool override)
{
    if (_distortionPreset.find(name) != _distortionPreset.end() && !override)
        return; // key already exists we return if we don't want to overried
    _distortionPreset.insert(name,preset);
}



void PresetController::AddPreset(QString name, BASS_DX8_CHORUS preset, bool override)
{
    if (_chorusPreset.find(name) != _chorusPreset.end() && !override)
        return; // key already exists
    _chorusPreset.insert(name,preset);
}
void PresetController::AddPreset(QString name, BASS_DX8_COMPRESSOR preset, bool override)
{
    if (_compressorPreset.find(name) != _compressorPreset.end()&& !override)
        return; // key already exists
    _compressorPreset.insert(name,preset);
}
void PresetController::AddPreset(QString name, BASS_DX8_ECHO preset, bool override)
{
    if (_echoPreset.find(name) != _echoPreset.end() && !override)
        return; // key already exists
    _echoPreset.insert(name,preset);
}
void PresetController::AddPreset(QString name, BASS_DX8_FLANGER preset, bool override)
{
    if (_flangerPreset.find(name) != _flangerPreset.end() && !override)
        return; // key already exists
    _flangerPreset.insert(name,preset);
}
void PresetController::AddPreset(QString name, BASS_DX8_GARGLE preset, bool override)
{
    if (_garglePreset.find(name) != _garglePreset.end() && !override)
        return; // key already exists
    _garglePreset.insert(name,preset);
}


}//end namespace LIDL
