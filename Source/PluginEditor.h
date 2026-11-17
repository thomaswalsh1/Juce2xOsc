/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "PageTabs.h"

//==============================================================================
/**
*/
class _2xOscSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _2xOscSynthAudioProcessorEditor (_2xOscSynthAudioProcessor&);
    ~_2xOscSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _2xOscSynthAudioProcessor& audioProcessor;

    // look and feel
    CustomLookAndFeel customLookAndFeel;

    // tabs
    OscillatorTabs oscillatorTabs;
    EffectsTabs effectsTabs;

    // header
    juce::Label header;
    juce::Label author;

    // everything under the header, filled with the page colour
    juce::Rectangle<int> body;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_2xOscSynthAudioProcessorEditor)
};
