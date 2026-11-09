/*
  ==============================================================================

    VoiceData.cpp
    Created: 8 Nov 2026 1:51:46pm
    Author:  Thomas

  ==============================================================================
*/

#include "VoiceData.h"

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
	DBG("Preparing SynthVoice to play");
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = (juce::uint32)outputChannels;

    for (int i = 0; i < NUM_OSCS; i++) {
        osc[i].prepare(spec);
    }
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return true;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
	DBG("Note Started: " << midiNoteNumber);

    currentNote = midiNoteNumber;
	level = velocity;
	currentFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    for (int i = 0; i < NUM_OSCS; i++) {
        osc[i].setFrequency(currentFrequency, true);
        osc[i].reset();
    }
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (!isVoiceActive())
        return;

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    // put that buffer into a block
    juce::dsp::AudioBlock<float> block{ synthBuffer };

    for (int i = 0; i < NUM_OSCS; i++) {
        osc[i].process(juce::dsp::ProcessContextReplacing<float> (block));
    }

    synthBuffer.applyGain(level * 0.3f);

    for (int i = 0; i < outputBuffer.getNumChannels(); ++i) {
        outputBuffer.addFrom(i, startSample, synthBuffer, i, 0, numSamples);
    }
}