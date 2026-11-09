/*
  ==============================================================================

    OscData.cpp
    Created: 24 Oct 2026 11:14:46am
    Author:  Thomas

  ==============================================================================
*/

#include "OscData.h"

Oscillator::Oscillator() {
    setWaveform(Sine);
}

void Oscillator::setFrequency(float hz, bool force) {
    osc.setFrequency(hz, force);
}

void Oscillator::setWaveform(WaveformType type) {
    switch (type)
    {
    case WaveformType::Sine:
        osc.initialise([](float x) { return std::sin(x); }, 128);
        break;

    case WaveformType::Saw:
        osc.initialise([](float x) { return x / juce::MathConstants<float>::pi; }, 128);
        break;

    case WaveformType::Square:
        osc.initialise([](float x) { return x < 0 ? -1.0f : 1.0f; }, 128);
        break;
    }
}

void Oscillator::prepare(const juce::dsp::ProcessSpec& spec) {
    osc.prepare(spec);
}

void Oscillator::reset() {
    osc.reset();
}


