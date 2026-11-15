/*
  ==============================================================================

    FmPage.h
    Created: 24 Oct 2026 12:21:50pm
    Author:  Thomas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OscPage.h"

//==============================================================================
// fm modulator settings, its envelope, and which carriers it modulates
class FmPage  : public juce::Component
{
public:
    FmPage(juce::AudioProcessorValueTreeState& apvts);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    OscillatorPanel modulatorPanel;

    LabeledSlider index;
    LabeledToggle toOsc1;
    LabeledToggle toOsc2;

    EnvelopePanel fmEnvelope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmPage)
};
