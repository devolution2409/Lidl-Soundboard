#ifndef ENUMSANDSTRUCTS_H
#define ENUMSANDSTRUCTS_H
#include <cstdint>
#include <type_traits>
#include "lib/bass.h"
#include "bitmask_operators.h"
//#include <bitset>
/*
#define DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE) \
extern "C++" { \
inline ENUMTYPE operator | (ENUMTYPE a, ENUMTYPE b) { return ENUMTYPE(((int)a) | ((int)b)); } \
inline ENUMTYPE &operator |= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(((int &)a) |= ((int)b)); } \
inline ENUMTYPE operator & (ENUMTYPE a, ENUMTYPE b) { return ENUMTYPE(((int)a) & ((int)b)); } \
inline ENUMTYPE &operator &= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(((int &)a) &= ((int)b)); } \
inline ENUMTYPE operator ~ (ENUMTYPE a) { return ENUMTYPE(~((int)a)); } \
inline ENUMTYPE operator ^ (ENUMTYPE a, ENUMTYPE b) { return ENUMTYPE(((int)a) ^ ((int)b)); } \
inline ENUMTYPE &operator ^= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(((int &)a) ^= ((int)b)); } \
}
*/
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

    enum SFX_TYPE : unsigned int
    {
        NONE = 0,
        DISTORTION = 1,
        CHORUS = 2,
        ECHO = 4

    };



    struct SFX
    {
        SFX_TYPE flags;
        BASS_DX8_DISTORTION distortion;
        BASS_DX8_CHORUS chorus;
        BASS_DX8_ECHO echo;
    };
 //DEFINE_ENUM_FLAG_OPERATORS(SFX_TYPE)
    enum class SFX_DIST_PARAM
    {
        fGain = 1,
        fEdge = 2,
        fPostEQCenterFrequency = 3,
        fPostEQBandwidth = 4,
        fPreLowpassCutoff = 5,
        DEFAULT_DISTORTION = 0,

    };
    enum class SFX_CHORUS_PARAM
    {
        fDelay,
        fDepth,
        fFeedback,
        fFrequency,
        fWetDryMix,
        lPhase,
        lWaveform
    };

    enum SHOW_SETTINGS : unsigned int
    {
        // using binary shift LUL
        NO_SETTINGS     = 0,    //0
        SHOW_SFX        = 1<<0, //1
        WRAP_SONG_LIST  = 1<<1, //2
    };

}



// Doesn't work with enum class pajaL forsenL nymnL
template<>
struct enable_bitmask_operators<LIDL::SFX_TYPE>{
    static const bool enable=true;
};



template<>
struct enable_bitmask_operators<LIDL::SHOW_SETTINGS>{
    static const bool enable=true;
};







#endif // ENUMSANDSTRUCTS_H
