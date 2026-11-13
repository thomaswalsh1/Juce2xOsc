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

void setupSmallerLabel(juce::Label& label, juce::String text);

class OscComponent : public juce::Component {
public:
    OscComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts);
    ~OscComponent() override;
    void setAdsrEnabled(bool enabled);

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> waveTypeAttachment;

    juce::Slider coarseTune;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> coarseTuneAttachment;

    juce::Slider fineTune;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fineTuneAttachment;

    juce::Slider attack;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    juce::Slider decay;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    juce::Slider sustain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    juce::Slider release;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

    // colored areas
    juce::Rectangle<int> detuneArea;
    juce::Rectangle<int> adsrArea;

	juce::AudioProcessorValueTreeState& apvts;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent)
};

//==============================================================================
class OscPage : public juce::Component
{
public:
    OscPage(juce::AudioProcessorValueTreeState& apvts);
    ~OscPage() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    CustomLookAndFeel customLookAndFeel;
    juce::AudioProcessorValueTreeState& apvts;

    OscComponent osc1{ "OSC1", apvts };
    OscComponent osc2{ "OSC2", apvts };

	

    juce::ToggleButton adsrSync;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> adsrSyncAttachment;

	juce::Label mixLabel;
    juce::Slider mixControl;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixControlAttachment;

	juce::Label glideLabel;
	juce::Slider glideControl;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> glideControlAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscPage)
};