/*
  ==============================================================================

    SynthEngine.cpp
    Author:  Thomas

  ==============================================================================
*/

#include "SynthEngine.h"
#include "SynthVoice.h"

SynthEngine::SynthEngine() {
    // enough for every midi note, so the audio thread never allocates
    heldNotes.ensureStorageAllocated(128);
}

void SynthEngine::setVoiceMode(VoiceMode newMode) {
    if (newMode == voiceMode)
        return;

    voiceMode = newMode;
    allNotesOff(0, true);
}

void SynthEngine::noteOn(int midiChannel, int midiNoteNumber, float velocity) {
    const bool wasHeld = !heldNotes.isEmpty();

    // legato only glides between overlapping keys, a detached key starts fresh
    glideSourceHz = (voiceMode == VoiceMode::Legato && !wasHeld) ? 0.0f : lastNoteHz;
    lastNoteHz = (float)juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

    if (voiceMode == VoiceMode::Poly) {
        juce::Synthesiser::noteOn(midiChannel, midiNoteNumber, velocity);
        return;
    }

    removeHeldNote(midiNoteNumber);
    heldNotes.add({ midiNoteNumber, velocity });

    auto* voice = dynamic_cast<SynthVoice*>(getVoice(0));
    if (voice == nullptr)
        return;

    if (!voice->isVoiceActive()) {
        startVoice(voice, getSound(0).get(), midiChannel, midiNoteNumber, velocity);
    }
    else {
        // mono retriggers and glides every time, legato only glides when keys overlap
        const bool isMono = voiceMode == VoiceMode::Mono;
        voice->changeNote(midiNoteNumber, velocity, isMono || !wasHeld, isMono || wasHeld);
    }
    monoNote = midiNoteNumber;
}

void SynthEngine::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff) {
    if (voiceMode == VoiceMode::Poly) {
        juce::Synthesiser::noteOff(midiChannel, midiNoteNumber, velocity, allowTailOff);
        return;
    }

    removeHeldNote(midiNoteNumber);

    auto* voice = dynamic_cast<SynthVoice*>(getVoice(0));
    if (voice == nullptr)
        return;

    if (heldNotes.isEmpty()) {
        if (voice->isVoiceActive())
            stopVoice(voice, velocity, allowTailOff);
        monoNote = -1;
    }
    else if (midiNoteNumber == monoNote) {
        // the sounding key was released, fall back to the most recent key still held
        const auto previous = heldNotes.getLast();
        voice->changeNote(previous.note, previous.velocity, voiceMode == VoiceMode::Mono, true);
        monoNote = previous.note;
        lastNoteHz = (float)juce::MidiMessage::getMidiNoteInHertz(previous.note);
    }
}

void SynthEngine::allNotesOff(int midiChannel, bool allowTailOff) {
    heldNotes.clearQuick();
    monoNote = -1;
    juce::Synthesiser::allNotesOff(midiChannel, allowTailOff);
}

void SynthEngine::removeHeldNote(int note) {
    heldNotes.removeIf([note](const HeldNote& held) { return held.note == note; });
}
