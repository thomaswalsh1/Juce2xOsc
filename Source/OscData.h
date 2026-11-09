/*
  ==============================================================================

    OscData.h
    Created: 24 Oct 2026 11:14:46am
    Author:  Thomas

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum WaveformType
{
    Sine,Saw,Square
};

class Oscillator {
public:
    Oscillator();
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
};