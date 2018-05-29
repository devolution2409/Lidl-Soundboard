#ifndef ENUMSANDSTRUCTS_H
#define ENUMSANDSTRUCTS_H

#include "lib/bass.h"




namespace LIDL{
    enum class Playback {
        Singleton = 1,
        Sequential = 2,
        Auto =  3,
        Cancer = 4
    };
    enum class Shortcut{
        GENERATE,
        DONT_GENERATE
    };
    //http://www.un4seen.com/doc/#bass/BASS_DX8_DISTORTION.html
    struct SFX
    {
        bool distortionEnabled;
        BASS_DX8_DISTORTION distortion;
        BASS_DX8_CHORUS chorus;
        BASS_DX8_ECHO echo;
    };

    enum class SFX_DIST_PARAM
    {
        fGain = 1,
        fEdge = 2,
        fPostEQCenterFrequency = 3,
        fPostEQBandwidth = 4,
        fPreLowpassCutoff = 5,
        DEFAULT_DISTORTION = 0,

    };


}















#endif // ENUMSANDSTRUCTS_H
