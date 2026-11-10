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
        osc[i].noteOn();
    }
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    if (allowTailOff) {
        for (int i = 0; i < NUM_OSCS; i++) {
            osc[i].noteOff();
        }
    }
    else {
        clearCurrentNote();
        level = 0.0f;
        for (int i = 0; i < NUM_OSCS; i++) {
            osc[i].reset();
        }
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (!isVoiceActive())
        return;

	// check if we have to stop the voice
	bool isOscActive = false;
	for (int i = 0; i < NUM_OSCS; i++) {
		if (osc[i].isActive()) {
			isOscActive = true;
			break;
		}
	}
    if (!isOscActive) {
		clearCurrentNote();
		level = 0.0f;
		for (int i = 0; i < NUM_OSCS; i++) {
			osc[i].reset();
		}
		return;
    }


    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    // put that buffer into a block
    juce::dsp::AudioBlock<float> block{ synthBuffer };
	juce::AudioBuffer<float> tempBuffer(outputBuffer.getNumChannels(), numSamples);
	juce::dsp::AudioBlock<float> tempBlock{tempBuffer};

    for (int i = 0; i < NUM_OSCS; i++) {
        tempBuffer.clear();
		// process the oscillator into the temp block
        osc[i].process(juce::dsp::ProcessContextNonReplacing<float> (tempBlock, tempBlock));
        // add to the main one
		block.add(tempBlock);
    }

    synthBuffer.applyGain(level * 0.3f);

    for (int i = 0; i < outputBuffer.getNumChannels(); ++i) {
        outputBuffer.addFrom(i, startSample, synthBuffer, i, 0, numSamples);
    }

	bool stillGoing = false;
	for (int i = 0; i < NUM_OSCS; i++) {
		if (osc[i].isActive()) {
			stillGoing = true;
			break;
		}
	}
	if (!stillGoing) {
		clearCurrentNote();
		level = 0.0f;
		for (int i = 0; i < NUM_OSCS; i++) {
			osc[i].reset();
		}
	}
}

void SynthVoice::setOscParameters(int oscIndex, const OscParameters& params) {
    if (oscIndex < 0 || oscIndex >= NUM_OSCS) {
        DBG("Invalid oscillator index: " << oscIndex);
        return;
    }

	osc[oscIndex].setWaveform(static_cast<WaveformType>(params.wave));
	osc[oscIndex].setDetune(params.coarse, params.fine);
	osc[oscIndex].setAdsr(params.attack, params.decay, params.sustain, params.release);
}