/*
  ==============================================================================

    UiHelpers.h
    Author:  Thomas

    Small reusable UI pieces shared by the pages.

    Layout rules every page follows:
      - pages are filled with Style::page and padded by Layout::spacing
      - controls always live inside a Section, sections are separated by Layout::spacing
      - a section may start with a selector row (combo boxes / toggles), then a row of
        knob or fader columns, all left aligned so columns line up between sections

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

void setupLabel(juce::Label& label, const juce::String& text, float fontSize,
                juce::Justification justification = juce::Justification::centred);

// every page is two rows of sections: returns the top row and leaves the bottom row in area
juce::Rectangle<int> takeTopRow(juce::Rectangle<int>& area);

// splits the selector row (and the gap under it) off the top of a section's content
juce::Rectangle<int> takeSelectorRow(juce::Rectangle<int>& content);

// combo boxes and toggles left to right, each at its standard width
void layoutSelectorRow(juce::Rectangle<int> area, std::initializer_list<juce::Component*> components);

// equal columns left to right, each at most maxColumnWidth wide
void layoutColumns(juce::Rectangle<int> area, std::initializer_list<juce::Component*> components, int maxColumnWidth);

// total width of a section whose content is contentWidth wide
int sectionWidth(int contentWidth);

//==============================================================================
// titled box that every group of controls sits in. Controls are added as children of the
// section and laid out inside getContentBounds()
class Section : public juce::Component {
public:
    explicit Section(const juce::String& title);

    juce::Rectangle<int> getContentBounds() const;

    void paint(juce::Graphics&) override;

private:
    juce::String title;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Section)
};

//==============================================================================
// label on top, then a rotary knob or vertical fader with its value box underneath,
// attached to an apvts parameter
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

// label on top, toggle box below, attached to an apvts parameter
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

//==============================================================================
// section with ADSR knobs, for any envelope using the ParamIDs envelope suffixes
class EnvelopePanel : public Section {
public:
    EnvelopePanel(juce::AudioProcessorValueTreeState& apvts, const juce::String& idPrefix, const juce::String& title);

    void resized() override;

private:
    LabeledSlider attack, decay, sustain, release;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopePanel)
};
