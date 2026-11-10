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
	void setAdsr(float attack, float decay, float sustain, float release);
    void setFrequency(float hz, bool force = false);
	void noteOn() { adsr.noteOn(); }
	void noteOff() { adsr.noteOff(); }
	bool isActive() const { return adsr.isActive(); }
    void setWaveform(WaveformType type);
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();

    template <typename ProcessContext>
    void process(const ProcessContext& context) {
		auto& inputBlock = context.getInputBlock();
        auto &output_block = context.getOutputBlock();
		auto numSumples = output_block.getNumSamples();
		auto numChannels = output_block.getNumChannels();

		for (size_t sample = 0; sample < numSumples; ++sample) {
			auto raw = osc.processSample(0.0f);
            float envelope = adsr.getNextSample();
			float finalSample = raw * envelope;
			for (size_t channel = 0; channel < numChannels; ++channel) {
				output_block.setSample(channel, sample, finalSample);
			}
		}
    }
private:
    juce::dsp::Oscillator<float> osc;
	juce::ADSR adsr;

	WaveformType osc_waveform = WaveformType::Sine;
	float osc_base = 440.0f;
    int osc_coarse = 0;
	float osc_fine = 0.0f;

    float osc_attack = 0.005f;
    float osc_decay = 0.1f;
    float osc_sustain = 0.3f;
    float osc_release = 0.015f;

	bool initialized = false;

    // needs to run during processing
    void updateFrequency(bool force);
};