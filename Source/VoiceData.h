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


class SynthVoice : public juce::SynthesiserVoice {
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
	void setOscParameters(int oscIndex, const OscParameters& params);
    void setAdsrSync(bool sync);
private:
    Oscillator osc[NUM_OSCS];

    juce::AudioBuffer<float> synthBuffer;
    
    int currentNote = 0;
	float currentFrequency = 0.0f;
    float level = 0.0f;

	bool adsrSync = false;

};
