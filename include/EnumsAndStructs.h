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

//http://www.un4seen.com/doc/#bass/BASS_DX8_CHORUS.html

struct BASS_CHORUS_PARAM{
    /* Ratio of wet (processed) signal to dry (unprocessed) signal.
     * Must be in the range from 0 through 100 (all wet). The default value is 50. */
    float fWetDryMix;
    /* Percentage by which the delay time is modulated by the low-frequency oscillator (LFO).
     * Must be in the range from 0 through 100. The default value is 10.    */
    float fDepth;
    /*Percentage of output signal to feed back into the effect's input,
     * in the range from -99 to 99. The default value is 25. */
    float fFeedback;
    /* Frequency of the LFO, in the range from 0 to 10. The default value is 1.1.*/
    float fFrequency;
    /* Waveform of the LFO... 0 = triangle, 1 = sine. By default, the waveform is sine.*/
    bool lWaveform;
    /* Number of milliseconds the input is delayed before it is played back, in the range from 0 to 20.
     *  The default value is 16 ms. */
    float fDelay;
    /* Phase differential between left and right LFOs, one of
     * BASS_DX8_PHASE_NEG_180, BASS_DX8_PHASE_NEG_90,
     * BASS_DX8_PHASE_ZERO, BASS_DX8_PHASE_90 and BASS_DX8_PHASE_180.
     * The default value is BASS_DX8_PHASE_90.*/
    //http://jerome.jouvie.free.fr/nativebass/javadoc/constant-values.html
    int lPhase;

    BASS_CHORUS_PARAM(): fWetDryMix(50),
                         fDepth(10),
                         fFeedback(25),
                         fFrequency(1.1),
                         lWaveform(true),
                         fDelay(16),
                         lPhase(BASS_DX8_PHASE_90){}

    // idk why Qt doesn't set BASS_DX8_PHASE_90 green here nor autocomplete it
    // but the constant is defined here too forsenT

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
