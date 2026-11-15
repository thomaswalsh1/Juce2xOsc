/*
  ==============================================================================

    UiHelpers.h
    Author:  Thomas

    Small reusable UI pieces shared by the pages.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

void setupLabel(juce::Label& label, const juce::String& text, float fontSize,
                juce::Justification justification = juce::Justification::centred);

// lays the components out side by side, sharing the width equally
void layoutRow(juce::Rectangle<int> bounds, std::initializer_list<juce::Component*> components);

// label on top, slider below (rotary knob by default), attached to an apvts parameter
class LabeledSlider : public juce::Component {
public:
    LabeledSlider(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID, const juce::String& text,
                  juce::Slider::SliderStyle style = juce::Slider::RotaryVerticalDrag);

    void resized() override;

private:
    juce::Label label;
    juce::Slider slider;
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledSlider)
};

// label on top, combo box below, items taken from the choice parameter
class LabeledComboBox : public juce::Component {
public:
    LabeledComboBox(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID, const juce::String& text);

    void resized() override;

private:
    juce::Label label;
    juce::ComboBox box;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledComboBox)
};

// label on top, toggle button below, attached to an apvts parameter
class LabeledToggle : public juce::Component {
public:
    LabeledToggle(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID, const juce::String& text);

    void resized() override;

private:
    juce::Label label;
    juce::ToggleButton button;
    juce::AudioProcessorValueTreeState::ButtonAttachment attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledToggle)
};

// titled box with ADSR knobs, for any envelope using the ParamIDs envelope suffixes
class EnvelopePanel : public juce::Component {
public:
    EnvelopePanel(juce::AudioProcessorValueTreeState& apvts, const juce::String& idPrefix, const juce::String& title);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Label titleLabel;
    LabeledSlider attack, decay, sustain, release;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopePanel)
};
