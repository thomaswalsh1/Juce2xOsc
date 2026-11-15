/*
  ==============================================================================

    Oscillator.h
    Created: 24 Oct 2026 11:14:46am
    Author:  Thomas

    Phase accumulator oscillator with glide, detune and pitch bend.
    Takes a phase offset per sample so it can be phase modulated (DX7-style
    fm). Saw and square are band limited with PolyBLEP. Used for both the
    carriers and the fm modulator.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Oscillator {
public:
    void prepare(double newSampleRate);
    void reset() { phase = 0.0f; }

    void setWaveform(WaveformType type) { waveform = type; }
    void setDetune(int coarse, float fine);
    void setBend(float bendSemitones);
    // scales the note frequency, the modulator uses this for the fm ratio
    void setFrequencyMultiplier(float multiplier) { frequencyMultiplier.setTargetValue(multiplier); }

    void startNote(float fromHz, float toHz, float glideSeconds);

    // phaseOffset is in cycles (1.0 = one full period)
    float processSample(float phaseOffset = 0.0f) {
        // TN: freq = midi * offset/detune * mod
        const float frequency = noteFrequency.getNextValue() * offsetRatio.getNextValue() * frequencyMultiplier.getNextValue();
        // TN: increment is how much this moves forward by 
        const float increment = juce::jlimit(0.0f, 0.5f, frequency / (float)sampleRate);

        // TN: add the offset we need
        float p = phase + phaseOffset;
        // TN: return to 0..1 range
        p -= std::floor(p);

        float out = 0.0f;
        switch (waveform)
        {
        case WaveformType::Sine:
            out = std::sin(juce::MathConstants<float>::twoPi * p);
            break;

        case WaveformType::Saw:
            out = 2.0f * p - 1.0f - polyBlep(p, increment);
            break;

        case WaveformType::Square:
        {
            float halfShifted = p + 0.5f;
            halfShifted -= std::floor(halfShifted);
            out = (p < 0.5f ? 1.0f : -1.0f) + polyBlep(p, increment) - polyBlep(halfShifted, increment);
            break;
        }
        }

        phase += increment;
        if (phase >= 1.0f)
            phase -= 1.0f;

        return out;
    }

private:
    WaveformType waveform = WaveformType::Sine;
    int coarse = 0;
    float fine = 0.0f;
    float bend = 0.0f;
    float phase = 0.0f; // TN: measured in cycles

    double sampleRate = 44100.0; // TN: how many snapshots in a second
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> noteFrequency { 440.0f };
    juce::SmoothedValue<float> offsetRatio { 1.0f };
    juce::SmoothedValue<float> frequencyMultiplier { 1.0f };

    void updateOffsetRatio(bool force);

    // smooths out the discontinuity at t = 0, dt is the phase increment
    static float polyBlep(float t, float dt) {
        if (t < dt) {
            t /= dt;
            return t + t - t * t - 1.0f;
        }
        if (t > 1.0f - dt) {
            t = (t - 1.0f) / dt;
            return t * t + t + t + 1.0f;
        }
        return 0.0f;
    }
};
