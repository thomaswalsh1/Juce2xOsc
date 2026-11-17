/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace Style::Layout;

//==============================================================================
_2xOscSynthAudioProcessorEditor::_2xOscSynthAudioProcessorEditor (_2xOscSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), oscillatorTabs(p.apvts), effectsTabs(p.apvts)
{
    // every child inherits this look and feel
    setLookAndFeel(&customLookAndFeel);

    // header
    header.setText("2xOsc", juce::dontSendNotification);
    header.setFont(Style::font(Style::FontSize::heading));
    header.setColour(juce::Label::textColourId, Style::textOnDark);
    header.setJustificationType(juce::Justification::centredLeft);
    header.setBorderSize({});
    addAndMakeVisible(header);

    // author
    author.setText("By Tom", juce::dontSendNotification);
    author.setFont(Style::font(Style::FontSize::small));
    author.setColour(juce::Label::textColourId, Style::textOnDark.withAlpha(0.6f));
    author.setJustificationType(juce::Justification::centredLeft);
    author.setBorderSize({});
    addAndMakeVisible(author);

    // tabs
    addAndMakeVisible(oscillatorTabs);
    addAndMakeVisible(effectsTabs);

    // the layout is sized for this, every knob column fits at full width
    setResizable(true, true);
    setResizeLimits(900, 520, 1920, 1080);
    setSize (900, 520);
}

_2xOscSynthAudioProcessorEditor::~_2xOscSynthAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void _2xOscSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Style::window);

    g.setColour(Style::page);
    g.fillRect(body);
}

void _2xOscSynthAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    auto headingSection = bounds.removeFromTop(headerHeight).reduced(spacing * 2, 0);
    const int headerWidth = juce::roundToInt(juce::GlyphArrangement::getStringWidth(header.getFont(), header.getText()));
    header.setBounds(headingSection.removeFromLeft(headerWidth + spacing));
    headingSection.removeFromLeft(spacing);
    author.setBounds(headingSection);

    body = bounds;

    // two tab areas side by side, same spacing around and between them as everywhere else
    auto tabsArea = body.reduced(spacing);
    oscillatorTabs.setBounds(tabsArea.removeFromLeft((tabsArea.getWidth() - spacing) / 2));
    tabsArea.removeFromLeft(spacing);
    effectsTabs.setBounds(tabsArea);
}
