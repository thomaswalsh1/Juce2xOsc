/*
  ==============================================================================

    OscData.cpp
    Created: 24 Oct 2026 11:14:46am
    Author:  Thomas

  ==============================================================================
*/

#include "OscData.h"

Oscillator::Oscillator() {
	adsr.setSampleRate(44100.0f);
    setWaveform(Sine);
}

void Oscillator::setDetune(int coarse, float fine) {
	if (coarse == osc_coarse && fine == osc_fine) {
		return;
	}
	osc_coarse = coarse;
	osc_fine = fine;

	updateFrequency(false);
}

void Oscillator::setAdsr(float attack, float decay, float sustain, float release) {
    osc_attack = attack;
	osc_decay = decay;
	osc_sustain = sustain;
	osc_release = release;
	
	// Update ADSR parameters
	adsr.setParameters(juce::ADSR::Parameters {
		osc_attack,
		osc_decay,
		osc_sustain,
		osc_release
	});
}

void Oscillator::setFrequency(float hz, bool force) {
    // sets the frequency BY THE NOTE
	osc_base = hz;
    updateFrequency(force);
}

void Oscillator::updateFrequency(bool force) {
	const float semitones = osc_coarse + osc_fine / 100.0f;
    osc.setFrequency(osc_base * std::pow(2.0f, semitones / 12.0f), force);
}

void Oscillator::setWaveform(WaveformType type) {
	if (initialized && type == osc_waveform) {
		return; // No change needed
	}

	osc_waveform = type;

    switch (type)
    {
    case WaveformType::Sine:
        osc.initialise([](float x) { return std::sin(x); }, 128);
        break;

    case WaveformType::Saw:
        osc.initialise([](float x) { return x / juce::MathConstants<float>::pi; }, 128);
        break;

    case WaveformType::Square:
        osc.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 128);
        break;
    }
	initialized = true;
}

void Oscillator::prepare(const juce::dsp::ProcessSpec& spec) {
    osc.prepare(spec);

	// adsr setup
    adsr.setSampleRate(spec.sampleRate);
	setAdsr(osc_attack, osc_decay, osc_sustain, osc_release);
}

void Oscillator::reset() {
    osc.reset();
    adsr.reset();
}


