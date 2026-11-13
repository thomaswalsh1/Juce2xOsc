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
/*
*/
class FmPage  : public juce::Component
{
public:
    FmPage(juce::AudioProcessorValueTreeState& apvts);
    ~FmPage() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& apvts;

    OscComponent oscFM{ "OSC3", apvts };

    juce::Label freqFMLabel;
    juce::Slider freqFM;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqFMAttachment;

    juce::Label depthFMLabel;
    juce::Slider depthFM;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthFMAttachment;

    juce::Label enableOnOsc1Label;
    juce::ToggleButton enableOnOsc1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableOnOsc1Attachment;

    juce::Label enableOnOsc2Label;
    juce::ToggleButton enableOnOsc2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableOnOsc2Attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmPage)
};
