/*
  ==============================================================================

    SynthEngine.h
    Author:  Thomas

    Synthesiser that remembers the previously played note, so new voices can
    glide from it. Voice modes (mono/legato) will live here later.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthEngine : public juce::Synthesiser {
public:
    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override {
        glideSourceHz = lastNoteHz;
        lastNoteHz = (float)juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        juce::Synthesiser::noteOn(midiChannel, midiNoteNumber, velocity);
    }

    // frequency of the note played before the one currently starting, 0 if none yet
    float getGlideSourceHz() const { return glideSourceHz; }

private:
    float lastNoteHz = 0.0f;
    float glideSourceHz = 0.0f;
};
