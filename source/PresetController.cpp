
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
            return (QStringList() << tr("No preset selected") << (_distortionPreset.keys()));
        return QStringList() << tr("No presets for distortion SFX !");
    }
    if (type == LIDL::SFX_TYPE::ECHO)
    {
        if ( ! _echoPreset.keys().isEmpty())
            return (QStringList() << tr("No preset selected") << _echoPreset.keys());
        return QStringList() << tr("No presets for echo SFX !");
    }
    if (type == LIDL::SFX_TYPE::CHORUS)
    {
        if ( ! _chorusPreset.keys().isEmpty())
            return (QStringList() << tr("No preset selected") << _chorusPreset.keys());
        return QStringList() << tr("No presets for chorus SFX !");
    }
    if (type == LIDL::SFX_TYPE::COMPRESSOR)
    {
        if ( ! _compressorPreset.keys().isEmpty())
            return (QStringList() << tr("No preset selected") << _compressorPreset.keys());
        return QStringList() << tr("No presets for compressor SFX !");
    }
    if (type == LIDL::SFX_TYPE::FLANGER)
    {
        if ( ! _flangerPreset.keys().isEmpty())
            return (QStringList() << tr("No preset selected") << _flangerPreset.keys());
        return QStringList() << tr("No presets for flanger SFX !");
    }
    if (type == LIDL::SFX_TYPE::GARGLE)
    {
        if ( ! _garglePreset.keys().isEmpty())
            return (QStringList() << tr("No preset selected") << _garglePreset.keys());
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

// implementing specialization here to not fuck the ODR.
template<>
std::map<int,int> PresetController::GetPreset<BASS_DX8_DISTORTION>(QString name)
{
    BASS_DX8_DISTORTION preset = _distortionPreset.value(name);
    std::map<int,int> enumMap;

    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_DIST_PARAM::fGain),  preset.fGain  ));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_DIST_PARAM::fEdge),  preset.fEdge  ));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_DIST_PARAM::fPostEQBandwidth), preset.fPostEQBandwidth));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_DIST_PARAM::fPostEQCenterFrequency), preset.fPostEQCenterFrequency ));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_DIST_PARAM::fPreLowpassCutoff), preset.fPreLowpassCutoff ));

    return enumMap;
}

template<>
std::map<int,int> PresetController::GetPreset<BASS_DX8_CHORUS>(QString name)
{
    BASS_DX8_CHORUS preset = _chorusPreset.value(name);
    std::map<int,int> enumMap;

    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_CHORUS_PARAM::fDelay), preset.fDelay));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_CHORUS_PARAM::fDepth), preset.fDepth));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_CHORUS_PARAM::fFeedback),preset.fFeedback));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_CHORUS_PARAM::fFrequency), preset.fFrequency));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_CHORUS_PARAM::fWetDryMix), preset.fWetDryMix));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_CHORUS_PARAM::lPhase), preset.lPhase));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_CHORUS_PARAM::lWaveform),preset.lWaveform));

    return enumMap;
}


template<>
std::map<int,int> PresetController::GetPreset<BASS_DX8_ECHO>(QString name)
{
    BASS_DX8_ECHO preset = _echoPreset.value(name);
    std::map<int,int> enumMap;

    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_ECHO_PARAM::fFeedback ), preset.fFeedback));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_ECHO_PARAM::fLeftDelay ), preset.fLeftDelay));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_ECHO_PARAM::fRightDelay ), preset.fRightDelay));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_ECHO_PARAM::fWetDryMix ), preset.fWetDryMix));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_ECHO_PARAM::lPanDelay ), preset.lPanDelay));

    return enumMap;
}

template<>
std::map<int,int> PresetController::GetPreset<BASS_DX8_COMPRESSOR>(QString name)
{
    BASS_DX8_COMPRESSOR preset = _compressorPreset.value(name);
    std::map<int,int> enumMap;

    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_COMPRESSOR_PARAM::fAttack ), preset.fAttack));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_COMPRESSOR_PARAM::fGain), preset.fGain));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_COMPRESSOR_PARAM::fPredelay ), preset.fPredelay));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_COMPRESSOR_PARAM::fRatio ), preset.fRatio));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_COMPRESSOR_PARAM::fRelease ), preset.fRelease));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_COMPRESSOR_PARAM::fThreshold), preset.fThreshold));

    return enumMap;
}

template<>
std::map<int,int> PresetController::GetPreset<BASS_DX8_FLANGER>(QString name)
{
    BASS_DX8_FLANGER preset = _flangerPreset.value(name);
    std::map<int,int> enumMap;

    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_FLANGER_PARAM::fDelay ), preset.fDelay));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_FLANGER_PARAM::fDepth ), preset.fDepth));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_FLANGER_PARAM::fFeedback ), preset.fFeedback));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_FLANGER_PARAM::fFrequency ), preset.fFrequency));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_FLANGER_PARAM::fWetDryMix ), preset.fWetDryMix));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_FLANGER_PARAM::lPhase ), preset.lPhase));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_FLANGER_PARAM::lWaveform ), preset.lWaveform));


    return enumMap;
}


template<>
std::map<int,int> PresetController::GetPreset<BASS_DX8_GARGLE>(QString name)
{
    BASS_DX8_GARGLE preset = _garglePreset.value(name);
    std::map<int,int> enumMap;

    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_GARGLE_PARAM::dwRateHz), preset.dwRateHz));
    enumMap.insert(std::pair<int,int>( static_cast<int>(SFX_GARGLE_PARAM::dwWaveShape), preset.dwWaveShape));


    return enumMap;
}





}//end namespace LIDL
