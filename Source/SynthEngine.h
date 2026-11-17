/*
  ==============================================================================

    SynthEngine.h
    Author:  Thomas

    Synthesiser with voice modes:
      Poly   - normal juce voice allocation, new voices glide from the last note
      Mono   - one voice, every new key retriggers the envelopes and glides
      Legato - one voice, overlapping keys glide without retriggering,
               detached keys retrigger without gliding
    Mono and legato keep a stack of held keys, so releasing a key goes back
    to the previous one still held.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class SynthEngine : public juce::Synthesiser {
public:
    SynthEngine();

    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    void noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff) override;
    void allNotesOff(int midiChannel, bool allowTailOff) override;

    // kept here rather than in the voices, since voices that aren't playing never see controller messages
    void handleController(int midiChannel, int controllerNumber, int controllerValue) override {
        if (controllerNumber == 1)
            modWheel = controllerValue / 127.0f;
        juce::Synthesiser::handleController(midiChannel, controllerNumber, controllerValue);
    }

    // called every block, releases everything when the mode changes so nothing gets stuck
    void setVoiceMode(VoiceMode newMode);

    // frequency of the note played before the one currently starting, 0 if none (or no glide wanted)
    float getGlideSourceHz() const { return glideSourceHz; }
    // 0..1
    float getModWheel() const { return modWheel; }

private:
    struct HeldNote {
        int note;
        float velocity;
    };

    VoiceMode voiceMode = VoiceMode::Poly;
    juce::Array<HeldNote> heldNotes;   // most recent key last
    int monoNote = -1;                 // note the mono voice is currently playing, -1 if none

    float lastNoteHz = 0.0f;
    float glideSourceHz = 0.0f;
    float modWheel = 0.0f;

    void removeHeldNote(int note);
};
