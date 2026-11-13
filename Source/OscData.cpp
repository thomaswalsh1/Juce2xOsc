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
	setFMWaveform(Sine);
}

void Oscillator::setDetune(int coarse, float fine) {
	if (coarse == osc_coarse && fine == osc_fine) {
		return;
	}
	osc_coarse = coarse;
	osc_fine = fine;

	updateFrequency(false);
}

void Oscillator::setFMDetune(int coarse, float fine) {
	if (coarse == fm_coarse && fine == fm_fine) {
		return;
	}
	fm_coarse = coarse;
	fm_fine = fine;

	fmDetuneRatio = std::pow(2.0f, (fm_coarse + fm_fine / 100.0f) / 12.0f);
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

void Oscillator::setFMAdsr(float attack, float decay, float sustain, float release) {
	fm_attack = attack;
	fm_decay = decay;
	fm_sustain = sustain;
	fm_release = release;

	// Update ADSR parameters
	fmAdsr.setParameters(juce::ADSR::Parameters{
		fm_attack,
		fm_decay,
		fm_sustain,
		fm_release
		});
}

void Oscillator::startNote(float prev, float next, float time) {
	// we need to call the smoothed frequency values proper methods
	noteFrequency.reset(sampleRate, time);
	noteFrequency.setCurrentAndTargetValue(prev);
	noteFrequency.setTargetValue(next);

	updateFrequency(true);
	adsr.noteOn();
	fmAdsr.noteOn();
}

void Oscillator::setBend(float bendSemitones) {
	if (bendSemitones == osc_bend) {
		return;
	}
	osc_bend = bendSemitones;
	updateFrequency(false);
}

void Oscillator::updateFrequency(bool force) {
	const float semitones = osc_coarse + osc_fine / 100.0f + osc_bend;
	const float ratio = std::pow(2.0f, semitones / 12.0f);

	if (force) {
		offsetRatio.setCurrentAndTargetValue(ratio); // becomes the previous
	}
	else {
		offsetRatio.setTargetValue(ratio); // becomes the next
	}
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

void Oscillator::setFMWaveform(WaveformType type) {
	if (fmInitialized && type == fm_waveform) {
		return; // No change needed
	}

	fm_waveform = type;

	switch (type)
	{
	case WaveformType::Sine:
		fm.initialise([](float x) { return std::sin(x); }, 128);
		break;

	case WaveformType::Saw:
		fm.initialise([](float x) { return x / juce::MathConstants<float>::pi; }, 128);
		break;

	case WaveformType::Square:
		fm.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 128);
		break;
	}
	fmInitialized = true;
}

void Oscillator::prepare(const juce::dsp::ProcessSpec& spec) {
	sampleRate = spec.sampleRate;
    osc.prepare(spec);
	fm.prepare(spec);

	noteFrequency.reset(sampleRate, 0.0);
	offsetRatio.reset(sampleRate, 0.01);

	// adsr setup
    adsr.setSampleRate(spec.sampleRate);
	setAdsr(osc_attack, osc_decay, osc_sustain, osc_release);
	fmAdsr.setSampleRate(spec.sampleRate);
	setFMAdsr(fm_attack, fm_decay, fm_sustain, fm_release);
}

void Oscillator::reset() {
    osc.reset();
	fm.reset();
    adsr.reset();
	fmAdsr.reset();
}

void Oscillator::setFMData(OscParameters& params) {
	setFMWaveform(static_cast<WaveformType>(params.wave));
	setFMDetune(params.coarse, params.fine);
	setFMAdsr(params.attack, params.decay, params.sustain, params.release);
}

