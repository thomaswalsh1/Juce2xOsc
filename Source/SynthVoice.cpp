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
    lfo.prepare(sampleRate);

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

void SynthVoice::startNote(int midiNoteNumber, float noteVelocity, juce::SynthesiserSound*, int currentPitchWheelPosition) {
    applyParams();

    velocity = noteVelocity;
    // velocity sensitivity: at 0 every note is full volume, at 1 volume follows velocity
    level = 1.0f - params.velocity.toAmp + params.velocity.toAmp * velocity;
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
    // free running lfos take their phase from the processor in renderNextBlock instead
    if (!params.lfo.freeRun)
        lfo.reset();
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

    const auto& lfoParams = params.lfo;
    // free running: every voice follows the shared phase, offset to where this sub-block starts in the buffer
    if (lfoParams.freeRun) {
        const float p = params.lfoBlockStartPhase + startSample * lfo.getIncrement();
        lfo.setPhase(p - std::floor(p));
    }
    // the mod wheel fades the lfo in, fully when lfo modWheel is 1
    const float lfoDepth = 1.0f - lfoParams.modWheel + lfoParams.modWheel * engine.getModWheel();
    // velocity is fixed for the note, so its contributions are constant across the block
    const float velocityCutoffOctaves = velocity * params.velocity.toCutoff;
    const float velocityFmIndex = velocity * params.velocity.toFmIndex;

    for (int s = 0; s < numSamples; ++s)
    {
        const float lfoValue = lfo.processSample() * lfoDepth;
        const float pitchRatio = lfoParams.toPitch > 0.0f ? std::exp2(lfoValue * lfoParams.toPitch / 12.0f) : 1.0f;

        // phase modulation: index is in radians, the oscillators take cycles
        const float fmIndex = juce::jmax(0.0f, fmIndexSmoothed.getNextValue() + lfoValue * lfoParams.toFmIndex + velocityFmIndex);
        // the modulator gets the vibrato too, so the fm ratio stays constant
        const float modSample = modulator.processSample(0.0f, pitchRatio) * fmEnv.getNextSample();
        const float phaseOffset = fmIndex * modSample / juce::MathConstants<float>::twoPi;

        const float c1 = carriers[0].processSample(fmToOsc1 ? phaseOffset : 0.0f, pitchRatio);
        const float c2 = carriers[1].processSample(fmToOsc2 ? phaseOffset : 0.0f, pitchRatio);
        const float m = mixSmoothed.getNextValue();
        // TN: Calculate Mixed Value
        float sample = c1 * (1.0f - m) + c2 * m;

        const float extraCutoffOctaves = lfoValue * lfoParams.toCutoff + velocityCutoffOctaves;
        filter.setCutoffFrequency(computeCutoff(filterEnv.getNextSample(), extraCutoffOctaves));
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
    // TN: Run params for oscillators
    for (int i = 0; i < numCarriers; ++i) {
        carriers[i].setWaveform(static_cast<WaveformType>(params.osc[i].wave));
        carriers[i].setDetune(params.osc[i].coarse, params.osc[i].fine);
    }

    // TN: Run params for modulators
    modulator.setWaveform(static_cast<WaveformType>(params.mod.wave));
    modulator.setDetune(0, params.mod.fine);
    modulator.setFrequencyMultiplier(params.mod.ratio);

    // TN: Run params for amp
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

    lfo.setWaveform(static_cast<LfoWaveform>(params.lfo.wave));
    lfo.setRate(LfoSync::effectiveRateHz(params.lfo, params.bpm));

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

float SynthVoice::computeCutoff(float filterEnvValue, float extraOctaves) {
    const float octaves = filterEnvValue * params.filter.envAmount + keytrackOctaves * params.filter.keytrack + extraOctaves;
    const float cutoff = cutoffSmoothed.getNextValue() * std::exp2(octaves);
    return juce::jlimit(SynthConstants::minCutoffHz, SynthConstants::maxCutoffRatio * (float)sampleRate, cutoff);
}
