/*
  ==============================================================================

    OscData.h
    Created: 24 Oct 2026 11:14:46am
    Author:  Thomas

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct OscParameters {
    int wave = 0;
	int coarse = 0;
	float fine = 0.0f;

    float attack = 0.005f;
	float decay = 0.1f;
	float sustain = 0.3f;
	float release = 0.015f;
};

enum WaveformType
{
    Sine,Saw,Square
};

class Oscillator {
public:
    Oscillator();
    void setDetune(int coarse, float fine);
    void setFMDetune(int coarse, float fine);
    void setAdsr(float attack, float decay, float sustain, float release);
    void setFMAdsr(float attack, float decay, float sustain, float release);
    void setFrequency(float hz, bool force = false);
    void setBend(float bendSemitones);
    void startNote(float prev, float next, float time);
	void noteOff() { adsr.noteOff(); fmAdsr.noteOff(); }
	bool isActive() const { return adsr.isActive(); }
    void setWaveform(WaveformType type);
    void setFMWaveform(WaveformType type);
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setFMDepth(const float depth) { fmDepth = depth; }
    void setFMRatio(const float ratio) { fmRatio = ratio; }
    void setFMData(OscParameters& params);
    void reset();

    float processSample() {
        float base = noteFrequency.getNextValue() * offsetRatio.getNextValue();
        float modFreq = base * fmRatio * fmDetuneRatio;
        fm.setFrequency(modFreq, true);

        // modulator shifts the carrier's frequency, depth is the modulation index
        float deviation = fm.processSample(0.0f) * fmDepth * modFreq * fmAdsr.getNextSample();
        // juce oscillators can't run backwards, so keep the carrier frequency positive
		osc.setFrequency(juce::jmax(0.0f, base + deviation), true);
		return osc.processSample(0.0f) * adsr.getNextSample();
    }
private:
    juce::dsp::Oscillator<float> osc;
    juce::dsp::Oscillator<float> fm;
	juce::ADSR adsr;
    juce::ADSR fmAdsr;

	WaveformType osc_waveform = WaveformType::Sine;
    int osc_coarse = 0;
	float osc_fine = 0.0f;
    float osc_bend = 0.0f;

    float osc_attack = 0.005f;
    float osc_decay = 0.1f;
    float osc_sustain = 0.3f;
    float osc_release = 0.015f;

    WaveformType fm_waveform = WaveformType::Sine;
    int fm_coarse = 0;
    float fm_fine = 0.0f;
    float fm_bend = 0.0f;

    float fm_attack = 0.005f;
    float fm_decay = 0.1f;
    float fm_sustain = 0.3f;
    float fm_release = 0.015f;

    bool initialized = false;
    bool fmInitialized = false;

	double sampleRate = 44100.0;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> noteFrequency{440.0f};
	juce::SmoothedValue<float> offsetRatio{ 1.0f };

    float fmRatio = 1.0f;
    float fmDepth = 0.0f;
    float fmDetuneRatio = 1.0f;

    // needs to run during processing
    void updateFrequency(bool force);
};