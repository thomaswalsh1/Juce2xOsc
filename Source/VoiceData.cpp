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

static float wheelToSemitones(int wheelValue) {
    return (wheelValue - 8192) / 8192.0f * 2.0f;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
	DBG("Note Started: " << midiNoteNumber);

    currentNote = midiNoteNumber;
	level = velocity;

    // handle gliding
	const float noteTarget = (float)juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	const bool glideActive = glide.glideTime > 0.001f && glide.sourceFrequency > 0.0f;
	const float prev = glideActive ? glide.sourceFrequency : noteTarget;
	const float newTime = glideActive ? glide.glideTime : 0.0f;

	glide.nextFrequency = noteTarget;

	const float bend = wheelToSemitones(currentPitchWheelPosition);
    for (int i = 0; i < NUM_OSCS; i++) {
		osc[i].setBend(bend);
        osc[i].startNote(prev, noteTarget, newTime);
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
	const float bend = wheelToSemitones(newPitchWheelValue);
	for (int i = 0; i < NUM_OSCS; i++) {
		osc[i].setBend(bend);
	}
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

void SynthVoice::updateFMParameters(FMParameters& fmParams, OscParameters& fmOsc) {
    const bool enabled[NUM_OSCS] = { fmParams.osc1, fmParams.osc2 };
    for (int i = 0; i < NUM_OSCS; ++i) {
        // depth of 0 turns fm off for that osc
        osc[i].setFMDepth(enabled[i] ? fmParams.depth : 0.0f);
        osc[i].setFMRatio(fmParams.ratio);
        osc[i].setFMData(fmOsc);
    }
}