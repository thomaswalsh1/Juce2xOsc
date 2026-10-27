/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_2xOscSynthAudioProcessorEditor::_2xOscSynthAudioProcessorEditor (_2xOscSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // look and feel
    setLookAndFeel(&customLookAndFeel);


    // header
    header.setText("2xOsc", juce::dontSendNotification);
    header.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(header);

    // tabs
    addAndMakeVisible(tabs1);
    addAndMakeVisible(tabs2);

    // call to audioProcessor for last saved heights
    setResizable(true, true);
    setResizeLimits(800, 400, 1920, 1080);
    setSize (800, 400);
}

_2xOscSynthAudioProcessorEditor::~_2xOscSynthAudioProcessorEditor()
{
}

//==============================================================================
void _2xOscSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Style::background);

    g.setColour (Style::textMain);
    g.setFont (juce::FontOptions (15.0f));
}

void _2xOscSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    auto heading = bounds.removeFromTop(30);
    header.setBounds(heading);
    int leftWidth = bounds.getWidth() / 2;
    tabs1.setBounds(bounds.removeFromLeft(leftWidth));
    tabs2.setBounds(bounds);
}


