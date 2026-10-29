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


class OscComponent : public juce::Component {
public:
    OscComponent(juce::String name);
    ~OscComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;
private:
    juce::Label title;
    juce::Label adsrLabel;

    // Labels for each knob
    juce::Label waveTypeLabel;

    // other big label
    juce::Label detune;

    juce::Label coarseTuneLabel;
    juce::Label fineTuneLabel;
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;

    // Sliders
    juce::Slider waveType;
    juce::Slider coarseTune;
    juce::Slider fineTune;
    juce::Slider attack;
    juce::Slider decay;
    juce::Slider sustain;
    juce::Slider release;

    // colored areas
    juce::Rectangle<int> detuneArea;
    juce::Rectangle<int> adsrArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent)
};

//==============================================================================
class OscPage : public juce::Component
{
public:
    OscPage();
    ~OscPage() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    CustomLookAndFeel customLookAndFeel;

    OscComponent osc1{ "Osc 1" };
    OscComponent osc2{ "Osc 2" };

    juce::ToggleButton enableAdsr2;
	juce::Label mixLabel;
    juce::Slider mixControl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscPage)
};