/*
  ==============================================================================

    FilterPage.h
    Created: 24 Oct 2026 12:23:24pm
    Author:  Thomas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "UiHelpers.h"

//==============================================================================
// top row: filter type and cutoff controls. bottom row: the filter envelope
class FilterPage  : public juce::Component
{
public:
    FilterPage(juce::AudioProcessorValueTreeState& apvts);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Section filterSection { "FILTER" };
    LabeledComboBox type;
    LabeledSlider cutoff, resonance, envAmount, keytrack;

    EnvelopePanel filterEnvelope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterPage)
};
