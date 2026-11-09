/*
  ==============================================================================

    OscData.h
    Created: 24 Oct 2026 11:14:46am
    Author:  Thomas

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct OscParameters {
    int wave = 0;
	int coarse = 0;
	float fine = 0.0f;
};

enum WaveformType
{
    Sine,Saw,Square
};

class Oscillator {
public:
    Oscillator();
    void setDetune(int coarse, float fine);
    void setFrequency(float hz, bool force = false);
    void setWaveform(WaveformType type);
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    template <typename ProcessContext>
    void process(const ProcessContext& context) {
        osc.process(context);
    }
private:
    juce::dsp::Oscillator<float> osc;

	WaveformType osc_waveform = WaveformType::Sine;
	float osc_base = 440.0f;
    int osc_coarse = 0;
	float osc_fine = 0.0f;
	bool initialized = false;

    // needs to run during processing
    void updateFrequency(bool force);
};