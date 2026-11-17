/*
  ==============================================================================

    OscPage.h
    Created: 24 Oct 2026 12:21:40pm
    Author:  Thomas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "Parameters.h"
#include "UiHelpers.h"

// waveform and tuning for one carrier oscillator
class OscillatorPanel : public Section {
public:
    OscillatorPanel(juce::AudioProcessorValueTreeState& apvts, const juce::String& title, const juce::String& idPrefix);

    void resized() override;

private:
    LabeledComboBox wave;
    LabeledSlider coarse, fine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorPanel)
};

//==============================================================================
// top row: both oscillators. bottom row: amp envelope and the global voice / mix / output controls
class OscPage : public juce::Component
{
public:
    OscPage(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    OscillatorPanel osc1;
    OscillatorPanel osc2;
    EnvelopePanel ampEnvelope;

    Section globalSection { "GLOBAL" };
    LabeledComboBox voiceMode;
    LabeledSlider mix;
    LabeledSlider glide;
    LabeledSlider masterGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscPage)
};
