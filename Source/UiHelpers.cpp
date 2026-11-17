/*
  ==============================================================================

    UiHelpers.cpp
    Author:  Thomas

  ==============================================================================
*/

#include "UiHelpers.h"
#include "Parameters.h"

using namespace Style::Layout;

void setupLabel(juce::Label& label, const juce::String& text, float fontSize, juce::Justification justification)
{
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(justification);
    label.setColour(juce::Label::textColourId, Style::ink);
    label.setFont(Style::font(fontSize));
    label.setBorderSize({});
    label.setInterceptsMouseClicks(false, false);
}

juce::Rectangle<int> takeTopRow(juce::Rectangle<int>& area)
{
    auto top = area.removeFromTop((area.getHeight() - spacing) / 2);
    area.removeFromTop(spacing);
    return top;
}

juce::Rectangle<int> takeSelectorRow(juce::Rectangle<int>& content)
{
    auto row = content.removeFromTop(selectorRowHeight);
    content.removeFromTop(spacing);
    return row;
}

void layoutSelectorRow(juce::Rectangle<int> area, std::initializer_list<juce::Component*> components)
{
    for (auto* c : components)
    {
        const int width = dynamic_cast<LabeledToggle*>(c) != nullptr ? toggleColumnWidth : comboWidth;
        c->setBounds(area.removeFromLeft(width));
        area.removeFromLeft(spacing);
    }
}

void layoutColumns(juce::Rectangle<int> area, std::initializer_list<juce::Component*> components, int maxColumnWidth)
{
    if (components.size() == 0)
        return;

    const int columnWidth = juce::jmin(maxColumnWidth, area.getWidth() / (int)components.size());
    for (auto* c : components)
        c->setBounds(area.removeFromLeft(columnWidth));
}

int sectionWidth(int contentWidth)
{
    return contentWidth + 2 * spacing;
}

//==============================================================================
Section::Section(const juce::String& sectionTitle) : title(sectionTitle)
{
}

juce::Rectangle<int> Section::getContentBounds() const
{
    auto content = getLocalBounds().reduced(spacing);
    content.removeFromTop(titleHeight);
    return content;
}

void Section::paint(juce::Graphics& g)
{
    g.setColour(Style::panel);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);

    g.setColour(Style::ink);
    g.setFont(Style::font(Style::FontSize::medium));
    g.drawText(title, getLocalBounds().reduced(spacing).removeFromTop(titleHeight), juce::Justification::centredLeft);
}

//==============================================================================
LabeledSlider::LabeledSlider(juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID, const juce::String& text,
                             juce::Slider::SliderStyle style)
    : attachment(apvts, paramID, slider)
{
    setupLabel(label, text, Style::FontSize::small);
    addAndMakeVisible(label);

    slider.setSliderStyle(style);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    addAndMakeVisible(slider);
}

void LabeledSlider::resized()
{
    auto bounds = getLocalBounds();
    // value boxes never wider than the column
    const int boxWidth = juce::jmin(textBoxWidth, bounds.getWidth());
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, boxWidth, textBoxHeight);

    if (slider.getSliderStyle() == juce::Slider::LinearVertical)
    {
        // faders use the full height, so a taller section gives a longer throw
        label.setBounds(bounds.removeFromTop(labelHeight));
        slider.setBounds(bounds);
        return;
    }

    // knobs are always the same size, the label/knob/value block is centred in whatever space is left
    const int blockHeight = labelHeight + knobSize + textBoxHeight;
    auto block = bounds.withSizeKeepingCentre(bounds.getWidth(), juce::jmin(blockHeight, bounds.getHeight()));
    label.setBounds(block.removeFromTop(labelHeight));
    slider.setBounds(block);
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
    auto block = getLocalBounds().withSizeKeepingCentre(getWidth(), labelHeight + comboHeight);
    label.setBounds(block.removeFromTop(labelHeight));
    box.setBounds(block);
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
    // same block height as a combo box, so toggles and combos in a selector row line up
    auto block = getLocalBounds().withSizeKeepingCentre(getWidth(), labelHeight + comboHeight);
    label.setBounds(block.removeFromTop(labelHeight));
    button.setBounds(block);
}

//==============================================================================
EnvelopePanel::EnvelopePanel(juce::AudioProcessorValueTreeState& apvts, const juce::String& idPrefix, const juce::String& title)
    : Section(title),
      attack(apvts, idPrefix + ParamIDs::attack, "Attack"),
      decay(apvts, idPrefix + ParamIDs::decay, "Decay"),
      sustain(apvts, idPrefix + ParamIDs::sustain, "Sustain"),
      release(apvts, idPrefix + ParamIDs::release, "Release")
{
    for (auto* c : std::initializer_list<juce::Component*> { &attack, &decay, &sustain, &release })
        addAndMakeVisible(c);
}

void EnvelopePanel::resized()
{
    layoutColumns(getContentBounds(), { &attack, &decay, &sustain, &release }, knobColumnWidth);
}
