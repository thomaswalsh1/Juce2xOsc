/*
  ==============================================================================

    SynthVoice.h
    Created: 8 Nov 2026 1:51:46pm
    Author:  Thomas

    Signal path per voice:
      modulator (fm env) -> phase modulates carriers -> mix -> filter (filter env)
      -> amp env -> stereo out

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Envelope.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "SynthEngine.h"

class SynthVoice : public juce::SynthesiserVoice {
public:
    SynthVoice(const SynthParams& sharedParams, const SynthEngine& owner) : params(sharedParams), engine(owner) {}

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:
    static constexpr int numCarriers = SynthConstants::numCarriers;

    // create parameters and then the engine as well
    const SynthParams& params;
    const SynthEngine& engine;

    // carriers
    Oscillator carriers[numCarriers];
    // oscillator responsible for modulation
    Oscillator modulator;

    Envelope ampEnv, filterEnv, fmEnv;
    juce::dsp::StateVariableTPTFilter<float> filter;

    juce::SmoothedValue<float> mixSmoothed;
    juce::SmoothedValue<float> fmIndexSmoothed;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> cutoffSmoothed { 20000.0f };

    double sampleRate = 44100.0;
    float level = 0.0f;
    float keytrackOctaves = 0.0f;   // note distance from middle C, for filter keytracking

    // pulls the latest block's parameters into the oscillators, envelopes and smoothers
    void applyParams();
    void setBendOnAll(float bendSemitones);
    float computeCutoff(float filterEnvValue);
};
