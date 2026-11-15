/*
  ==============================================================================

    Envelope.h
    Author:  Thomas

    ADSR that only recalculates when its parameters actually change.
    Used for the amp, filter and fm envelopes.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Envelope {
public:
    void prepare(double sampleRate) {
        adsr.setSampleRate(sampleRate);
        adsr.setParameters(params);
    }

    void setParameters(const EnvelopeParameters& p) {
        if (p.attack == params.attack && p.decay == params.decay
            && p.sustain == params.sustain && p.release == params.release) {
            return;
        }
        params = { p.attack, p.decay, p.sustain, p.release };
        adsr.setParameters(params);
    }

    void noteOn() { adsr.noteOn(); }
    void noteOff() { adsr.noteOff(); }
    void reset() { adsr.reset(); }
    bool isActive() const { return adsr.isActive(); }
    float getNextSample() { return adsr.getNextSample(); }

private:
    juce::ADSR adsr;
    juce::ADSR::Parameters params { 0.005f, 0.1f, 0.3f, 0.015f };
};
