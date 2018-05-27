#ifndef ENUMSANDSTRUCTS_H
#define ENUMSANDSTRUCTS_H



struct BASS_DISTORTION_PARAM {
    float fGain;
    // default gain is -18 i think;
    float fEdge;
    float fPostEQCenterFrequency;
    float fPostEQBandwidth;
    float fPreLowpassCutoff;
};



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
}















#endif // ENUMSANDSTRUCTS_H
