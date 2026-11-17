/*
  ==============================================================================

    Lfo.h
    Author:  Thomas

    Low frequency oscillator, -1..+1. Phase accumulator in cycles like
    Oscillator, but without PolyBLEP since it runs far below audio rate.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Lfo {
public:
    void prepare(double newSampleRate) { sampleRate = newSampleRate; }

    void setWaveform(LfoWaveform type) { waveform = type; }
    void setRate(float hz) { increment = hz / (float)sampleRate; }

    float getIncrement() const { return increment; }

    // restart the cycle, used on note on so every note's lfo starts the same way
    void reset() {
        phase = 0.0f;
        heldValue = nextRandom();
    }

    // jump to a phase from outside (free running / bar locked). Small corrections every block,
    // so a big backwards jump means the cycle wrapped and S&H should take a new step
    void setPhase(float newPhase) {
        // TN: If we are closer to the nexg phase
        if (newPhase < phase - 0.5f)
            heldValue = nextRandom();
        phase = newPhase;
    }

    float processSample() {
        float out = 0.0f;
        switch (waveform)
        {
        case LfoWaveform::Sine:          out = std::sin(juce::MathConstants<float>::twoPi * phase); break;
        case LfoWaveform::Triangle:      out = 4.0f * std::abs(phase - 0.5f) - 1.0f; break;
        case LfoWaveform::Saw:           out = 2.0f * phase - 1.0f; break;
        case LfoWaveform::Square:        out = phase < 0.5f ? 1.0f : -1.0f; break;
        case LfoWaveform::SampleAndHold: out = heldValue; break;
        }

        phase += increment;
        // TN: If phase is now greater than 1
        if (phase >= 1.0f) {
            phase -= 1.0f;
            heldValue = nextRandom(); // new step every cycle
        }

        return out;
    }

private:
    LfoWaveform waveform = LfoWaveform::Sine;
    double sampleRate = 44100.0;
    float phase = 0.0f;
    // TN: increment is how far into the cycle it is during the current Sample
    float increment = 0.0f;
    float heldValue = 0.0f;
    juce::Random random;

    float nextRandom() { return random.nextFloat() * 2.0f - 1.0f; }
};
