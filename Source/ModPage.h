/*
  ==============================================================================

    ModPage.h
    Author:  Thomas

    LFO and velocity routing.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "UiHelpers.h"

//==============================================================================
class ModPage  : public juce::Component
{
public:
    ModPage(juce::AudioProcessorValueTreeState& apvts);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Label lfoTitle;
    LabeledComboBox lfoWave;
    LabeledSlider lfoRate, lfoToPitch, lfoToCutoff, lfoToFmIndex, lfoModWheel;

    juce::Label velocityTitle;
    LabeledSlider velToAmp, velToCutoff, velToFmIndex;

    juce::Rectangle<int> lfoArea;
    juce::Rectangle<int> velocityArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModPage)
};
