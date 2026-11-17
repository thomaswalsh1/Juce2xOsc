/*
  ==============================================================================

    FmPage.h
    Created: 24 Oct 2026 12:21:50pm
    Author:  Thomas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "UiHelpers.h"

//==============================================================================
// top row: the fm modulator and which carriers it modulates. bottom row: its envelope
class FmPage  : public juce::Component
{
public:
    FmPage(juce::AudioProcessorValueTreeState& apvts);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Section modulatorSection { "MODULATOR" };
    LabeledComboBox wave;
    LabeledSlider ratio, fine, index;

    Section routingSection { "ROUTING" };
    LabeledToggle toOsc1;
    LabeledToggle toOsc2;

    EnvelopePanel fmEnvelope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmPage)
};
