/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_2xOscSynthAudioProcessorEditor::_2xOscSynthAudioProcessorEditor (_2xOscSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), oscillatorTabs(p.apvts), effectsTabs(p.apvts)
{
    // every child inherits this look and feel
    setLookAndFeel(&customLookAndFeel);

    // header
    header.setText("2xOsc", juce::dontSendNotification);
    header.setFont(Style::font(Style::FontSize::heading));
    header.setColour(juce::Label::textColourId, Style::textMain);
    header.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(header);

    // author
    author.setText("By Tom", juce::dontSendNotification);
    author.setFont(Style::font(Style::FontSize::medium));
    author.setColour(juce::Label::textColourId, Style::surface);
    author.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(author);

    // tabs
    addAndMakeVisible(oscillatorTabs);
    addAndMakeVisible(effectsTabs);

    setResizable(true, true);
    setResizeLimits(800, 400, 1920, 1080);
    setSize (800, 400);
}

_2xOscSynthAudioProcessorEditor::~_2xOscSynthAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void _2xOscSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (Style::background);

    g.setColour(Style::background2);
    g.fillRect(secondaryBackground);
}

void _2xOscSynthAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto headingSection = bounds.removeFromTop(48);
    headingSection.reduce(10, 0);
    header.setBounds(headingSection.removeFromLeft(80));
    author.setBounds(headingSection.removeFromLeft(104));

    secondaryBackground = bounds.removeFromTop(13);

    int leftWidth = bounds.getWidth() / 2;
    oscillatorTabs.setBounds(bounds.removeFromLeft(leftWidth));
    effectsTabs.setBounds(bounds);
}
