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

	mixSmoothed.reset(sampleRate, 0.2);

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
    if (!isVoiceActive()) return;

    for (int s = 0; s < numSamples; ++s)
    {
        const float m = mixSmoothed.getNextValue();
        const float sample = (osc[0].processSample() * (1.0f - m)
            + osc[1].processSample() * m) * level * 0.25f;

        for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
            outputBuffer.addSample(ch, startSample + s, sample);
    }

    if (!osc[0].isActive() && !osc[1].isActive())
        clearCurrentNote();
}

void SynthVoice::setOscParameters(int oscIndex, const OscParameters& params) {
    if (oscIndex < 0 || oscIndex >= NUM_OSCS) {
        DBG("Invalid oscillator index: " << oscIndex);
        return;
    }

	osc[oscIndex].setWaveform(static_cast<WaveformType>(params.wave));
	osc[oscIndex].setDetune(params.coarse, params.fine);
	if (adsrSync) {
		for (int i = 0; i < NUM_OSCS; ++i) {
			osc[i].setAdsr(params.attack, params.decay, params.sustain, params.release);
		}
	}
	else {
		osc[oscIndex].setAdsr(params.attack, params.decay, params.sustain, params.release);
	}
}

void SynthVoice::setAdsrSync(bool sync) {
	adsrSync = sync;
}
    
void SynthVoice::updateMix(float newMix)
{
	mixSmoothed.setTargetValue(newMix);
}