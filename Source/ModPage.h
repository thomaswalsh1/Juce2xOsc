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
// top row: the lfo. bottom row: velocity routing
class ModPage  : public juce::Component
{
public:
    ModPage(juce::AudioProcessorValueTreeState& apvts);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Section lfoSection { "LFO" };
    LabeledComboBox lfoWave;
    LabeledToggle lfoSync, lfoFreeRun;
    LabeledSlider lfoRate, lfoToPitch, lfoToCutoff, lfoToFmIndex, lfoModWheel;
    // shares the rate knob's column, only one of the two is visible
    LabeledComboBox lfoSyncRate;
    // swaps the rate knob and the sync rate box whenever sync changes, including from the host
    juce::ParameterAttachment lfoSyncListener;

    Section velocitySection { "VELOCITY" };
    LabeledSlider velToAmp, velToCutoff, velToFmIndex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModPage)
};
