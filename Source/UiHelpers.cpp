/*
  ==============================================================================

    UiHelpers.cpp
    Author:  Thomas

  ==============================================================================
*/

#include "UiHelpers.h"
#include "Parameters.h"

static constexpr int labelHeight = 20;

void setupLabel(juce::Label& label, const juce::String& text, float fontSize, juce::Justification justification)
{
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(justification);
    label.setColour(juce::Label::textColourId, Style::textSecondary);
    label.setFont(Style::font(fontSize));
}

void layoutRow(juce::Rectangle<int> bounds, std::initializer_list<juce::Component*> components)
{
    juce::FlexBox row;
    row.flexDirection = juce::FlexBox::Direction::row;
    for (auto* c : components)
        row.items.add(juce::FlexItem(*c).withFlex(1.0f));
    row.performLayout(bounds);
}

//==============================================================================
LabeledSlider::LabeledSlider(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID, const juce::String& text,
                             juce::Slider::SliderStyle style)
    : attachment(apvts, paramID, slider)
{
    setupLabel(label, text, Style::FontSize::small);
    addAndMakeVisible(label);

    slider.setSliderStyle(style);
    if (style == juce::Slider::LinearHorizontal)
        slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    else
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(slider);
}

void LabeledSlider::resized()
{
    auto bounds = getLocalBounds();
    label.setBounds(bounds.removeFromTop(labelHeight));
    slider.setBounds(bounds);
}

//==============================================================================
LabeledComboBox::LabeledComboBox(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID, const juce::String& text)
{
    setupLabel(label, text, Style::FontSize::small);
    addAndMakeVisible(label);

    // items have to exist before the attachment is created
    if (auto* choice = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(paramID)))
        box.addItemList(choice->choices, 1);
    else
        jassertfalse; // LabeledComboBox needs a choice parameter

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, paramID, box);
    addAndMakeVisible(box);
}

void LabeledComboBox::resized()
{
    auto bounds = getLocalBounds();
    label.setBounds(bounds.removeFromTop(labelHeight));
    box.setBounds(bounds.withSizeKeepingCentre(bounds.getWidth(), juce::jmin(24, bounds.getHeight())));
}

//==============================================================================
LabeledToggle::LabeledToggle(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID, const juce::String& text)
    : attachment(apvts, paramID, button)
{
    setupLabel(label, text, Style::FontSize::small);
    addAndMakeVisible(label);
    addAndMakeVisible(button);
}

void LabeledToggle::resized()
{
    auto bounds = getLocalBounds();
    label.setBounds(bounds.removeFromTop(labelHeight));
    // keep the tick box centred under the label
    button.setBounds(bounds.withSizeKeepingCentre(24, 24));
}

//==============================================================================
EnvelopePanel::EnvelopePanel(juce::AudioProcessorValueTreeState& apvts, const juce::String& idPrefix, const juce::String& title)
    : attack(apvts, idPrefix + ParamIDs::attack, "Attack"),
      decay(apvts, idPrefix + ParamIDs::decay, "Decay"),
      sustain(apvts, idPrefix + ParamIDs::sustain, "Sustain"),
      release(apvts, idPrefix + ParamIDs::release, "Release")
{
    setupLabel(titleLabel, title, Style::FontSize::large, juce::Justification::left);
    for (auto* c : std::initializer_list<juce::Component*> { &titleLabel, &attack, &decay, &sustain, &release })
        addAndMakeVisible(c);
}

void EnvelopePanel::paint(juce::Graphics& g)
{
    g.fillAll(Style::accent);
}

void EnvelopePanel::resized()
{
    auto bounds = getLocalBounds();
    titleLabel.setBounds(bounds.removeFromTop(labelHeight).withTrimmedLeft(5));
    layoutRow(bounds.reduced(5), { &attack, &decay, &sustain, &release });
}
