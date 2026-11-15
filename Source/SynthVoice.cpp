/*
  ==============================================================================

    SynthVoice.cpp
    Created: 8 Nov 2026 1:51:46pm
    Author:  Thomas

  ==============================================================================
*/

#include "SynthVoice.h"

void SynthVoice::prepareToPlay(double newSampleRate, int samplesPerBlock) {
    sampleRate = newSampleRate;

    for (auto& carrier : carriers) {
        carrier.prepare(sampleRate);
    }
    modulator.prepare(sampleRate);

    ampEnv.prepare(sampleRate);
    filterEnv.prepare(sampleRate);
    fmEnv.prepare(sampleRate);

    // the voice is mono until something (pan, unison) makes the two sides differ
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 1 };
    filter.prepare(spec);

    mixSmoothed.reset(sampleRate, SynthConstants::mixSmoothingSeconds);
    fmIndexSmoothed.reset(sampleRate, SynthConstants::controlSmoothingSeconds);
    cutoffSmoothed.reset(sampleRate, SynthConstants::controlSmoothingSeconds);

    applyParams();
    mixSmoothed.setCurrentAndTargetValue(params.mix);
    fmIndexSmoothed.setCurrentAndTargetValue(params.mod.index);
    cutoffSmoothed.setCurrentAndTargetValue(params.filter.cutoff);
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound*) {
    return true;
}

static float wheelToSemitones(int wheelValue) {
    return (wheelValue - 8192) / 8192.0f * SynthConstants::pitchBendRangeSemitones;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int currentPitchWheelPosition) {
    applyParams();

    level = velocity;
    keytrackOctaves = (midiNoteNumber - 60) / 12.0f;

    // glide from whatever note was played before this one
    const float noteTarget = (float)juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    const float glideSource = engine.getGlideSourceHz();
    const bool glideActive = params.glideTime > 0.001f && glideSource > 0.0f;
    const float from = glideActive ? glideSource : noteTarget;
    const float glideSeconds = glideActive ? params.glideTime : 0.0f;

    setBendOnAll(wheelToSemitones(currentPitchWheelPosition));
    for (auto& carrier : carriers) {
        carrier.startNote(from, noteTarget, glideSeconds);
    }
    modulator.startNote(from, noteTarget, glideSeconds);

    filter.reset();
    ampEnv.noteOn();
    filterEnv.noteOn();
    fmEnv.noteOn();
}

void SynthVoice::stopNote(float, bool allowTailOff) {
    if (allowTailOff) {
        ampEnv.noteOff();
        filterEnv.noteOff();
        fmEnv.noteOff();
    }
    else {
        clearCurrentNote();
        level = 0.0f;
        ampEnv.reset();
        filterEnv.reset();
        fmEnv.reset();
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {
    setBendOnAll(wheelToSemitones(newPitchWheelValue));
}

void SynthVoice::controllerMoved(int, int) {
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    applyParams();

    if (!isVoiceActive()) return;

    const bool fmToOsc1 = params.mod.toOsc1;
    const bool fmToOsc2 = params.mod.toOsc2;
    const int numChannels = outputBuffer.getNumChannels();

    for (int s = 0; s < numSamples; ++s)
    {
        // phase modulation: index is in radians, the oscillators take cycles
        const float modSample = modulator.processSample() * fmEnv.getNextSample();
        const float phaseOffset = fmIndexSmoothed.getNextValue() * modSample / juce::MathConstants<float>::twoPi;

        const float c1 = carriers[0].processSample(fmToOsc1 ? phaseOffset : 0.0f);
        const float c2 = carriers[1].processSample(fmToOsc2 ? phaseOffset : 0.0f);
        const float m = mixSmoothed.getNextValue();
        float sample = c1 * (1.0f - m) + c2 * m;

        filter.setCutoffFrequency(computeCutoff(filterEnv.getNextSample()));
        sample = filter.processSample(0, sample);

        sample *= ampEnv.getNextSample() * level * SynthConstants::outputGain;

        const float left = sample;
        const float right = sample;
        outputBuffer.addSample(0, startSample + s, left);
        if (numChannels > 1)
            outputBuffer.addSample(1, startSample + s, right);
    }

    if (!ampEnv.isActive())
        clearCurrentNote();
}

void SynthVoice::applyParams() {
    for (int i = 0; i < numCarriers; ++i) {
        carriers[i].setWaveform(static_cast<WaveformType>(params.osc[i].wave));
        carriers[i].setDetune(params.osc[i].coarse, params.osc[i].fine);
    }

    modulator.setWaveform(static_cast<WaveformType>(params.mod.wave));
    modulator.setDetune(0, params.mod.fine);
    modulator.setFrequencyMultiplier(params.mod.ratio);

    ampEnv.setParameters(params.amp);
    filterEnv.setParameters(params.filterEnv);
    fmEnv.setParameters(params.fmEnv);

    switch (static_cast<FilterType>(params.filter.type))
    {
    case FilterType::LowPass:  filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);  break;
    case FilterType::BandPass: filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass); break;
    case FilterType::HighPass: filter.setType(juce::dsp::StateVariableTPTFilterType::highpass); break;
    }
    filter.setResonance(params.filter.resonance);

    mixSmoothed.setTargetValue(params.mix);
    fmIndexSmoothed.setTargetValue(params.mod.index);
    cutoffSmoothed.setTargetValue(params.filter.cutoff);
}

void SynthVoice::setBendOnAll(float bendSemitones) {
    for (auto& carrier : carriers) {
        carrier.setBend(bendSemitones);
    }
    modulator.setBend(bendSemitones);
}

float SynthVoice::computeCutoff(float filterEnvValue) {
    const float octaves = filterEnvValue * params.filter.envAmount + keytrackOctaves * params.filter.keytrack;
    const float cutoff = cutoffSmoothed.getNextValue() * std::exp2(octaves);
    return juce::jlimit(SynthConstants::minCutoffHz, SynthConstants::maxCutoffRatio * (float)sampleRate, cutoff);
}
