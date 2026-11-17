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

    // lfo
    inline const juce::String lfoWave = "LFO_WAVE";
    inline const juce::String lfoRate = "LFO_RATE";
    inline const juce::String lfoToPitch = "LFO_TO_PITCH";
    inline const juce::String lfoToCutoff = "LFO_TO_CUTOFF";
    inline const juce::String lfoToFmIndex = "LFO_TO_FM_INDEX";
    inline const juce::String lfoModWheel = "LFO_MODWHEEL";
    inline const juce::String lfoSync = "LFO_SYNC";
    inline const juce::String lfoSyncRate = "LFO_SYNC_RATE";
    inline const juce::String lfoFreeRun = "LFO_FREE_RUN";

    // velocity
    inline const juce::String velToAmp = "VEL_TO_AMP";
    inline const juce::String velToCutoff = "VEL_TO_CUTOFF";
    inline const juce::String velToFmIndex = "VEL_TO_FM_INDEX";

    // global
    inline const juce::String voiceMode = "VOICE_MODE";
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

// scoped, since the names overlap with WaveformType
enum class LfoWaveform
{
    Sine, Triangle, Saw, Square, SampleAndHold
};

inline const juce::StringArray waveformNames { "Sine", "Saw", "Square" };
inline const juce::StringArray filterTypeNames { "Low Pass", "Band Pass", "High Pass" };
inline const juce::StringArray lfoWaveformNames { "Sine", "Triangle", "Saw", "Square", "S&H" };

enum VoiceMode
{
    Poly, Mono, Legato
};

inline const juce::StringArray voiceModeNames { "Poly", "Mono", "Legato" };

// tempo synced lfo rates: each division is the length of one lfo cycle in quarter note beats
namespace LfoSync {
    struct Division {
        const char* name;
        float beats;
    };

    inline constexpr Division divisions[] = {
        { "4 Bars", 16.0f },  { "2 Bars", 8.0f },     { "1 Bar", 4.0f },
        { "1/2", 2.0f },      { "1/2 D", 3.0f },      { "1/2 T", 4.0f / 3.0f },
        { "1/4", 1.0f },      { "1/4 D", 1.5f },      { "1/4 T", 2.0f / 3.0f },
        { "1/8", 0.5f },      { "1/8 D", 0.75f },     { "1/8 T", 1.0f / 3.0f },
        { "1/16", 0.25f },    { "1/16 D", 0.375f },   { "1/16 T", 1.0f / 6.0f },
        { "1/32", 0.125f },
    };

    constexpr int numDivisions = (int)std::size(divisions);
    constexpr int defaultDivision = 6; // 1/4

    inline juce::StringArray names() {
        juce::StringArray result;
        for (const auto& d : divisions)
            result.add(d.name);
        return result;
    }

    // beats per second divided by beats per cycle gives cycles per second
    inline float rateHz(int divisionIndex, double bpm) {
        const auto& division = divisions[juce::jlimit(0, numDivisions - 1, divisionIndex)];
        return (float)(bpm / 60.0) / division.beats;
    }
}

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

struct LfoParameters {
    int wave = 0;
    float rate = 5.0f;          // Hz
    float toPitch = 0.0f;       // semitones
    float toCutoff = 0.0f;      // octaves
    float toFmIndex = 0.0f;     // radians
    float modWheel = 0.0f;      // 0 = full depth always, 1 = depth set entirely by the mod wheel
    bool sync = false;          // when on, the rate comes from the host tempo instead of rate
    int syncDivision = LfoSync::defaultDivision;
    bool freeRun = false;       // off = restart every note, on = shared phase (locked to the bar when synced and playing)
};

struct VelocityParameters {
    float toAmp = 1.0f;         // 0 = every note equally loud, 1 = fully velocity sensitive
    float toCutoff = 0.0f;      // octaves at full velocity
    float toFmIndex = 0.0f;     // radians at full velocity
};

// everything the voices need, loaded once per block by the processor
// TN: Osc Parameters are 
struct SynthParams {
    OscParameters osc[SynthConstants::numCarriers];
    ModulatorParameters mod;
    EnvelopeParameters amp, filterEnv, fmEnv;
    FilterParameters filter;
    LfoParameters lfo;
    VelocityParameters velocity;
    int voiceMode = VoiceMode::Poly;
    float mix = 0.5f;
    float glideTime = 0.0f;
    float masterGainDb = 0.0f;

    // from the host, not a parameter. Keeps the last known tempo (or 120) when the host doesn't report one
    double bpm = 120.0;
    // lfo phase (cycles) at the first sample of this block, used when the lfo is free running
    float lfoBlockStartPhase = 0.0f;
};

namespace LfoSync {
    // the rate the lfo actually runs at, synced or not
    inline float effectiveRateHz(const LfoParameters& lfo, double bpm) {
        return lfo.sync ? rateHz(lfo.syncDivision, bpm) : lfo.rate;
    }
}
