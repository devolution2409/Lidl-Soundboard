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
    /*!
     * \brief The Playback enum
     */
    enum class Playback {
        Singleton = 1, /*!< DEPRECATED: Only here for compatibility. The wrapper can contain 1 or less sounds, that can't be played several times at once. */
        Sequential = 2, /*!< The wrapper can contain several sounds, that will cycle every time the play button is called, and the previous one finished playing. */
        Auto =  3, /*!< The wrapper can contain several sounds. Once the PlayNext signal is received, it will play the first sound and the next one after it ended, until all sounds are played. */
        Cancer = 4, /*!< The wrappper can contain 1 or less sounds. It can be spammed, hence the name, cancer. */
        AutoLoop = 5 /*!< Like auto, but will loop until canceled */
    };

    /*!
     * \brief The SFX_TYPE enum is used as flag enumeration thanks to bitmask_operators.h
     *
     * This enumeration is encapsulated in the stuct SFX.
     */
    enum SFX_TYPE : unsigned int
    {
        NONE        = 0,    /*!< Internally 0. Means no sfx. */
        DISTORTION  = 1<<0, /*!< Internally 1. Means Distortion sfx. */
        CHORUS      = 1<<1, /*!< Internally 2. Means Chorus sfx. */
        ECHO        = 1<<2, /*!< Internally 4. Means Echo sfx. */
        COMPRESSOR  = 1<<3, /*!< Internally 8. Means Compressor sfx. */
        FLANGER     = 1<<4, /*!< Internally 16. Means Flanger sfx. */
        GARGLE      = 1<<5, /*!< Internally 32. Means Gargle sfx. */
    };


    /*!
     * \brief The SFX struct.
     *
     * Every LIDL::SoundFile a SFX enum as a member variable, to account
     * for SFX happening in said wrapper.
     * Every CustomListWidgetItem also has a SFX struct to account for displaying them.
     *
     * Each SFX is stored as a flag. If the flag is set, then the player will
     * read the settings from the corresponding BASS_DX8_SFX structure.
     * (Those are BASS api structures: http://bass.radio42.com/help/html/824b82c4-f5cd-e544-b8a7-2176b4c3ebed.htm )
     *
     */
    struct SFX
    {
        SFX_TYPE flags;
        BASS_DX8_DISTORTION distortion;
        BASS_DX8_CHORUS chorus;
        BASS_DX8_ECHO echo;
        BASS_DX8_FLANGER flanger;
        BASS_DX8_COMPRESSOR compressor;
        BASS_DX8_GARGLE gargle;
    };

    /*!
     * \brief The SFX_DIST_PARAM enum
     *
     * Used internally to describe the same bass struct, but to allow flexibility with
     * our code design (see the lambdas deal<SFX> in WrapperProperties::ItemWasClicked
     * as well as their usage in CustomPlayer.cpp).
     */
    enum class SFX_DIST_PARAM : unsigned int
    {
        fGain = 0, /*!< Amount of signal change after distortion, in the range from -60 through 0. The default value is 0 dB. */
        fEdge = 1, /*!< Percentage of distortion intensity, in the range in the range from 0 through 100. The default value is 50 percent. */
        fPostEQCenterFrequency = 2, /*!<  Center frequency of harmonic content addition, in the range from 100 through 8000. The default value is 4000 Hz.*/
        fPostEQBandwidth = 3, /*!< Width of frequency band that determines range of harmonic content addition, in the range from 100 through 8000. The default value is 4000 Hz.*/
        fPreLowpassCutoff = 4, /*!< Filter cutoff for high-frequency harmonics attenuation, in the range from 100 through 8000. The default value is 4000 Hz. */
        ITER_END = 5 /*!< Used in for loop as the stop condition */
    };
    /*!
     * \brief The SFX_CHORUS_PARAM enum
     *
     * Used internally to describe the same bass struct, but to allow flexibility with
     * our code design (see the lambdas deal<SFX> in WrapperProperties::ItemWasClicked
     * as well as their usage in CustomPlayer.cpp).
     */
    enum class SFX_CHORUS_PARAM : unsigned int // ok checked
    {
        fDelay      = 0, /*!< Number of milliseconds the input is delayed before it is played back, in the range from 0 to 20. The default value is 0 ms. */
        fDepth      = 1, /*!< Percentage by which the delay time is modulated by the low-frequency oscillator, in hundredths of a percentage point. Must be in the range from 0 through 100. The default value is 25. */
        fFeedback   = 2, /*!< Percentage of output signal to feed back into the effect's input, in the range from -99 to 99. The default value is 0.*/
        fFrequency  = 3, /*!< Frequency of the LFO, in the range from 0 to 10. The default value is 0.*/
        fWetDryMix  = 4, /*!< Ratio of wet (processed) signal to dry (unprocessed) signal. Must be in the range from 0 (default) through 100 (all wet).*/
        lPhase      = 5, /*!< Phase differential between left and right LFOs, one of BASSFXPhase values. Default is (int)BASSFXPhase.BASS_FX_PHASE_ZERO.*/
        lWaveform   = 6, /*!< Waveform of the LFO... 0 = triangle, 1 = sine. By default, the waveform is a sine.*/
        ITER_END    = 7 /*!< Used in for loop as the stop condition */
    };

   /*!
    * \brief The SFX_ECHO_PARAM enum
     *
     * Used internally to describe the same bass struct, but to allow flexibility with
     * our code design (see the lambdas deal<SFX> in WrapperProperties::ItemWasClicked
     * as well as their usage in CustomPlayer.cpp).
     */
    enum class SFX_ECHO_PARAM : unsigned int // to check
    {
        fWetDryMix  = 0, /*!< Ratio of wet (processed) signal to dry (unprocessed) signal. Must be in the range from 0 (default) through 100 (all wet).*/
        fFeedback   = 1, /*!< Percentage of output fed back into input, in the range from 0 through 100. The default value is 0. */
        fLeftDelay  = 2, /*!< Delay for left channel, in milliseconds, in the range from 1 through 2000. The default value is 333 ms. */
        fRightDelay = 3, /*!< Delay for right channel, in milliseconds, in the range from 1 through 2000. The default value is 333 ms. */
        lPanDelay   = 4, /*!< Value that specifies whether to swap left and right delays with each successive echo. The default value is false, meaning no swap.*/
        ITER_END    = 5  /*!< Used in for loop as the stop condition */
    };

    /*!
     * \brief The SFX_COMPRESSOR_PARAM enum
     *
     * Used internally to describe the same bass struct, but to allow flexibility with
     * our code design (see the lambdas deal<SFX> in WrapperProperties::ItemWasClicked
     * as well as their usage in CustomPlayer.cpp).
     */
    enum class SFX_COMPRESSOR_PARAM : unsigned int // ok checked
    {
        fAttack     = 0, /*!< Time in ms before compression reaches its full value, in the range from 0.01 to 500. The default value is 10 ms. */
        fGain       = 1, /*!< Output gain of signal after compression, in the range from -60 to 60. The default value is 0 dB. */
        fPredelay	= 2, /*!< Time after fThreshold is reached before attack phase is started, in milliseconds, in the range from 0 to 4. The default value is 4 ms. */
        fRatio      = 3, /*!< Compression ratio, in the range from 1 to 100. The default value is 3, which means 3:1 compression. */
        fRelease    = 4, /*!< Time (speed) at which compression is stopped after input drops below fThreshold, in the range from 50 to 3000. The default value is 200 ms. */
        fThreshold	= 5, /*!< Point at which compression begins, in decibels, in the range from -60 to 0. The default value is -20 dB. */
        ITER_END    = 6  /*!< Used in for loop as the stop condition */
    };

    /*!
     * \brief The SFX_FLANGER_PARAM enum
     *
     * Used internally to describe the same bass struct, but to allow flexibility with
     * our code design (see the lambdas deal<SFX> in WrapperProperties::ItemWasClicked
     * as well as their usage in CustomPlayer.cpp).
     */
    enum class SFX_FLANGER_PARAM : unsigned int // ok checked
    {
        fDelay        = 0, /*!< Number of milliseconds the input is delayed before it is played back, in the range from 0 to 4. The default value is 0 ms. */
        fDepth        = 1, /*!< Percentage by which the delay time is modulated by the low-frequency oscillator (LFO), in hundredths of a percentage point. Must be in the range from 0 through 100. The default value is 25. */
        fFeedback     = 2, /*!< Percentage of output signal to feed back into the effect's input, in the range from -99 to 99. The default value is 0.*/
        fFrequency    = 3, /*!< Frequency of the LFO, in the range from 0 to 10. The default value is 0.*/
        fWetDryMix    = 4, /*!< Ratio of wet (processed) signal to dry (unprocessed) signal. Must be in the range from 0 (default) through 100 (all wet). */
        lPhase        = 5, /*!< Phase differential between left and right LFOs, one of the BASSFXPhase values. Default (int)BASSFXPhase.BASS_FX_PHASE_ZERO.*/
        lWaveform     = 6, /*!< Waveform of the LFO... 0 = triangle, 1 = sine. By default, the waveform is a sine.*/
        ITER_END      = 7  /*!< Used in for loop as the stop condition */
    };

   /*!
    * \brief The BASS_DX8_GARGLE enum
    *
    * Used internally to describe the same bass struct, but to allow flexibility with
    * our code design (see the lambdas deal<SFX> in WrapperProperties::ItemWasClicked
    * as well as their usage in CustomPlayer.cpp).
    */
    enum class SFX_GARGLE_PARAM : unsigned int
    {
        dwRateHz = 0, //Rate of modulation, in Hertz. Must be in the range from 1 through 1000. The default value is 20.
        dwWaveShape = 1, //Shape of the modulation waveform... 0 = triangle, 1 = square. By default, the waveform is triangle.
        ITER_END = 2 /*!< */
    };

    enum SHOW_SETTINGS : unsigned int
    {
        // using binary shift LUL
        NO_SETTINGS     = 0,    //0
        SHOW_SFX        = 1<<0, //1
        WRAP_SONG_LIST  = 1<<1, //2
    };

    enum PROFILE_COPY_MODE : unsigned int
    {
        NO_COPY = 0,
        COPY = 1,
        MIRROR = 2

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
