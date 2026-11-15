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

// waveform and tuning controls for one oscillator
class OscillatorPanel : public juce::Component {
public:
    // tuneID/tuneText is the first tuning knob: coarse for carriers, ratio for the modulator
    OscillatorPanel(juce::AudioProcessorValueTreeState& apvts, const juce::String& title, const juce::String& waveID,
                    const juce::String& tuneID, const juce::String& tuneText, const juce::String& fineID);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Label titleLabel;
    juce::Label tuneLabel;
    LabeledComboBox wave;
    LabeledSlider tune, fine;

    juce::Rectangle<int> tuneArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorPanel)
};

//==============================================================================
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

    LabeledSlider mix;
    LabeledSlider glide;
    LabeledSlider masterGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscPage)
};
