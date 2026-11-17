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

    // kept here rather than in the voices, since voices that aren't playing never see controller messages
    void handleController(int midiChannel, int controllerNumber, int controllerValue) override {
        if (controllerNumber == 1)
            modWheel = controllerValue / 127.0f;
        juce::Synthesiser::handleController(midiChannel, controllerNumber, controllerValue);
    }

    // frequency of the note played before the one currently starting, 0 if none yet
    float getGlideSourceHz() const { return glideSourceHz; }
    // 0..1
    float getModWheel() const { return modWheel; }

private:
    float lastNoteHz = 0.0f;
    float glideSourceHz = 0.0f;
    float modWheel = 0.0f;
};
