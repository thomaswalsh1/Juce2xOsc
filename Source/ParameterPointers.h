/*
  ==============================================================================

    ParameterPointers.h
    Author:  Thomas

    Raw apvts parameter pointers, looked up once and loaded into a SynthParams
    snapshot every block. Adding a parameter: add a pointer here, attach it,
    and load it into its SynthParams field.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

namespace ParameterPointerDetail {
    inline std::atomic<float>* get(juce::AudioProcessorValueTreeState& state, const juce::String& id) {
        auto* p = state.getRawParameterValue(id);
        jassert(p != nullptr); // parameter id missing from the layout
        return p;
    }
}

struct OscPointers {
    std::atomic<float>* wave = nullptr;
    std::atomic<float>* coarse = nullptr;
    std::atomic<float>* fine = nullptr;

    void attach(juce::AudioProcessorValueTreeState& state, const juce::String& prefix) {
        using ParameterPointerDetail::get;
        wave = get(state, prefix + ParamIDs::wave);
        coarse = get(state, prefix + ParamIDs::coarse);
        fine = get(state, prefix + ParamIDs::fine);
    }

    void load(OscParameters& p) const {
        p.wave = (int)wave->load();
        p.coarse = (int)coarse->load();
        p.fine = fine->load();
    }
};

struct EnvelopePointers {
    std::atomic<float>* attack = nullptr;
    std::atomic<float>* decay = nullptr;
    std::atomic<float>* sustain = nullptr;
    std::atomic<float>* release = nullptr;

    void attach(juce::AudioProcessorValueTreeState& state, const juce::String& prefix) {
        using ParameterPointerDetail::get;
        attack = get(state, prefix + ParamIDs::attack);
        decay = get(state, prefix + ParamIDs::decay);
        sustain = get(state, prefix + ParamIDs::sustain);
        release = get(state, prefix + ParamIDs::release);
    }

    void load(EnvelopeParameters& p) const {
        p.attack = attack->load();
        p.decay = decay->load();
        p.sustain = sustain->load();
        p.release = release->load();
    }
};

struct ParameterPointers {
    OscPointers osc[SynthConstants::numCarriers];
    EnvelopePointers amp, filterEnv, fmEnv;

    std::atomic<float>* modWave = nullptr;
    std::atomic<float>* modFine = nullptr;
    std::atomic<float>* fmRatio = nullptr;
    std::atomic<float>* fmIndex = nullptr;
    std::atomic<float>* fmToOsc1 = nullptr;
    std::atomic<float>* fmToOsc2 = nullptr;

    std::atomic<float>* filterType = nullptr;
    std::atomic<float>* filterCutoff = nullptr;
    std::atomic<float>* filterResonance = nullptr;
    std::atomic<float>* filterEnvAmount = nullptr;
    std::atomic<float>* filterKeytrack = nullptr;

    std::atomic<float>* lfoWave = nullptr;
    std::atomic<float>* lfoRate = nullptr;
    std::atomic<float>* lfoToPitch = nullptr;
    std::atomic<float>* lfoToCutoff = nullptr;
    std::atomic<float>* lfoToFmIndex = nullptr;
    std::atomic<float>* lfoModWheel = nullptr;
    std::atomic<float>* lfoSync = nullptr;
    std::atomic<float>* lfoSyncRate = nullptr;
    std::atomic<float>* lfoFreeRun = nullptr;

    std::atomic<float>* velToAmp = nullptr;
    std::atomic<float>* velToCutoff = nullptr;
    std::atomic<float>* velToFmIndex = nullptr;

    std::atomic<float>* mix = nullptr;
    std::atomic<float>* glideTime = nullptr;
    std::atomic<float>* masterGain = nullptr;

    void attach(juce::AudioProcessorValueTreeState& state) {
        using ParameterPointerDetail::get;
        osc[0].attach(state, ParamIDs::osc1);
        osc[1].attach(state, ParamIDs::osc2);
        amp.attach(state, ParamIDs::amp);
        filterEnv.attach(state, ParamIDs::filterEnv);
        fmEnv.attach(state, ParamIDs::fm);

        modWave = get(state, ParamIDs::fm + ParamIDs::wave);
        modFine = get(state, ParamIDs::fm + ParamIDs::fine);
        fmRatio = get(state, ParamIDs::fmRatio);
        fmIndex = get(state, ParamIDs::fmIndex);
        fmToOsc1 = get(state, ParamIDs::fmToOsc1);
        fmToOsc2 = get(state, ParamIDs::fmToOsc2);

        filterType = get(state, ParamIDs::filterType);
        filterCutoff = get(state, ParamIDs::filterCutoff);
        filterResonance = get(state, ParamIDs::filterResonance);
        filterEnvAmount = get(state, ParamIDs::filterEnvAmount);
        filterKeytrack = get(state, ParamIDs::filterKeytrack);

        lfoWave = get(state, ParamIDs::lfoWave);
        lfoRate = get(state, ParamIDs::lfoRate);
        lfoToPitch = get(state, ParamIDs::lfoToPitch);
        lfoToCutoff = get(state, ParamIDs::lfoToCutoff);
        lfoToFmIndex = get(state, ParamIDs::lfoToFmIndex);
        lfoModWheel = get(state, ParamIDs::lfoModWheel);
        lfoSync = get(state, ParamIDs::lfoSync);
        lfoSyncRate = get(state, ParamIDs::lfoSyncRate);
        lfoFreeRun = get(state, ParamIDs::lfoFreeRun);

        velToAmp = get(state, ParamIDs::velToAmp);
        velToCutoff = get(state, ParamIDs::velToCutoff);
        velToFmIndex = get(state, ParamIDs::velToFmIndex);

        mix = get(state, ParamIDs::oscMix);
        glideTime = get(state, ParamIDs::glideTime);
        masterGain = get(state, ParamIDs::masterGain);
    }

    void load(SynthParams& p) const {
        for (int i = 0; i < SynthConstants::numCarriers; ++i)
            osc[i].load(p.osc[i]);
        amp.load(p.amp);
        filterEnv.load(p.filterEnv);
        fmEnv.load(p.fmEnv);

        p.mod.wave = (int)modWave->load();
        p.mod.fine = modFine->load();
        p.mod.ratio = fmRatio->load();
        p.mod.index = fmIndex->load();
        p.mod.toOsc1 = fmToOsc1->load() > 0.5f;
        p.mod.toOsc2 = fmToOsc2->load() > 0.5f;

        p.filter.type = (int)filterType->load();
        p.filter.cutoff = filterCutoff->load();
        p.filter.resonance = filterResonance->load();
        p.filter.envAmount = filterEnvAmount->load();
        p.filter.keytrack = filterKeytrack->load();

        p.lfo.wave = (int)lfoWave->load();
        p.lfo.rate = lfoRate->load();
        p.lfo.toPitch = lfoToPitch->load();
        p.lfo.toCutoff = lfoToCutoff->load();
        p.lfo.toFmIndex = lfoToFmIndex->load();
        p.lfo.modWheel = lfoModWheel->load();
        p.lfo.sync = lfoSync->load() > 0.5f;
        p.lfo.syncDivision = (int)lfoSyncRate->load();
        p.lfo.freeRun = lfoFreeRun->load() > 0.5f;

        p.velocity.toAmp = velToAmp->load();
        p.velocity.toCutoff = velToCutoff->load();
        p.velocity.toFmIndex = velToFmIndex->load();

        p.mix = mix->load();
        p.glideTime = glideTime->load();
        p.masterGainDb = masterGain->load();
    }
};
