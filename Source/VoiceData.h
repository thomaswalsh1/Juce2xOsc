/*
  ==============================================================================

    VoiceData.h
    Created: 8 Nov 2026 1:51:46pm
    Author:  Thomas

  ==============================================================================
*/

#pragma once
#include "OscData.h"
#include <JuceHeader.h>

static constexpr int NUM_OSCS = 2;

class SynthSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override {
        return true;
    }
    bool appliesToChannel(int midiChannel) override {
        return true;
    }
};

struct GlideParameters {
    float glideTime = 0.0f;
    float sourceFrequency = 0.0f;
    float nextFrequency = 0.0f;
};

struct FMParameters {
    bool osc1 = false;
    bool osc2 = false;
    float ratio = 1.0f;
    float depth = 0.0f;
};

class SynthVoice : public juce::SynthesiserVoice {
public:
	explicit SynthVoice(GlideParameters& glideParams) : glide(glideParams) {}
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
	void setOscParameters(int oscIndex, const OscParameters& params);
    void setAdsrSync(bool sync);
    void updateMix(float newMix);
	void updateGlide(float newGlide);
    void updateFMParameters(FMParameters& fmParams, OscParameters& fmOsc);
private:
    Oscillator osc[NUM_OSCS];
    GlideParameters& glide;
    
    int currentNote = 0;
	float currentFrequency = 0.0f;
    float level = 0.0f;
	bool adsrSync = false;

    juce::SmoothedValue<float> mixSmoothed;
};
