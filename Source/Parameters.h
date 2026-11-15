/*
  ==============================================================================

    Parameters.h
    Author:  Thomas

    Parameter IDs, parameter structs and synth-wide constants shared by the
    DSP and the UI.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace ParamIDs {
    // prefixes, combined with the suffixes below (e.g. "OSC1_WAVE", "AMP_ATTACK")
    inline const juce::String osc1 = "OSC1";
    inline const juce::String osc2 = "OSC2";
    inline const juce::String fm = "FM";
    inline const juce::String amp = "AMP";
    inline const juce::String filterEnv = "FILTER_ENV";

    // oscillator suffixes
    inline const juce::String wave = "_WAVE";
    inline const juce::String coarse = "_COARSE";
    inline const juce::String fine = "_FINE";

    // envelope suffixes
    inline const juce::String attack = "_ATTACK";
    inline const juce::String decay = "_DECAY";
    inline const juce::String sustain = "_SUSTAIN";
    inline const juce::String release = "_RELEASE";

    // fm
    inline const juce::String fmRatio = "FM_RATIO";
    inline const juce::String fmIndex = "FM_INDEX";
    inline const juce::String fmToOsc1 = "FM_TO_OSC1";
    inline const juce::String fmToOsc2 = "FM_TO_OSC2";

    // filter
    inline const juce::String filterType = "FILTER_TYPE";
    inline const juce::String filterCutoff = "FILTER_CUTOFF";
    inline const juce::String filterResonance = "FILTER_RESONANCE";
    inline const juce::String filterEnvAmount = "FILTER_ENV_AMOUNT";
    inline const juce::String filterKeytrack = "FILTER_KEYTRACK";

    // global
    inline const juce::String oscMix = "OSC_MIX";
    inline const juce::String glideTime = "GLIDE_TIME";
    inline const juce::String masterGain = "MASTER_GAIN";
}

namespace SynthConstants {
    constexpr int numVoices = 12;
    constexpr int numCarriers = 2;
    constexpr float outputGain = 0.25f;
    constexpr double mixSmoothingSeconds = 0.2;
    constexpr double detuneSmoothingSeconds = 0.01;
    constexpr double controlSmoothingSeconds = 0.02;   // fm index/ratio, cutoff, master gain
    constexpr float pitchBendRangeSemitones = 2.0f;
    constexpr float minCutoffHz = 20.0f;
    constexpr float maxCutoffRatio = 0.45f;            // of the sample rate
}

enum WaveformType
{
    Sine, Saw, Square
};

enum FilterType
{
    LowPass, BandPass, HighPass
};

inline const juce::StringArray waveformNames { "Sine", "Saw", "Square" };
inline const juce::StringArray filterTypeNames { "Low Pass", "Band Pass", "High Pass" };

struct OscParameters {
    int wave = 0;
    int coarse = 0;
    float fine = 0.0f;
};

struct ModulatorParameters {
    int wave = 0;
    float ratio = 1.0f;
    float fine = 0.0f;
    float index = 0.0f;
    bool toOsc1 = false;
    bool toOsc2 = false;
};

struct EnvelopeParameters {
    float attack = 0.005f;
    float decay = 0.1f;
    float sustain = 0.3f;
    float release = 0.015f;
};

struct FilterParameters {
    int type = 0;
    float cutoff = 20000.0f;
    float resonance = 0.707f;
    float envAmount = 0.0f;     // octaves
    float keytrack = 0.0f;      // 0 = none, 1 = cutoff follows the note fully
};

// everything the voices need, loaded once per block by the processor
// TN: Osc Parameters are 
struct SynthParams {
    OscParameters osc[SynthConstants::numCarriers];
    ModulatorParameters mod;
    EnvelopeParameters amp, filterEnv, fmEnv;
    FilterParameters filter;
    float mix = 0.5f;
    float glideTime = 0.0f;
    float masterGainDb = 0.0f;
};
