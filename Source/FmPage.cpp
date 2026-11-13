/*
  ==============================================================================

    FmPage.cpp
    Created: 24 Oct 2026 12:21:50pm
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FmPage.h"
#include "CustomLookAndFeel.h"

//==============================================================================
FmPage::FmPage(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(oscFM);
    setupSmallerLabel(enableOnOsc1Label, "OSC 1");
    addAndMakeVisible(enableOnOsc1Label);
    addAndMakeVisible(enableOnOsc1);
    setupSmallerLabel(enableOnOsc2Label, "OSC 2");
    addAndMakeVisible(enableOnOsc2Label);
    addAndMakeVisible(enableOnOsc2);

    setupSmallerLabel(freqFMLabel, "Freq");
    addAndMakeVisible(freqFMLabel);
    freqFM.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
    freqFM.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
    freqFM.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    freqFM.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(freqFM);

    setupSmallerLabel(depthFMLabel, "Depth");
    addAndMakeVisible(depthFMLabel);
    depthFM.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
    depthFM.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
    depthFM.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    depthFM.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(depthFM);

    // attachments
    enableOnOsc1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "FM_OSC1", enableOnOsc1);
    enableOnOsc2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "FM_OSC2", enableOnOsc2);
    depthFMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "FM_DEPTH", depthFM);
    freqFMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "FM_RATIO", freqFM);
}

FmPage::~FmPage()
{
}

void FmPage::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(Style::background2);
    g.setFont(juce::FontOptions(Style::GothicFontName, 18.0f, juce::Font::bold));
}

void FmPage::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto bounds = getLocalBounds();
    const float width = bounds.getWidth();
    juce::FlexBox main;
    main.flexDirection = juce::FlexBox::Direction::column;
    main.alignContent = juce::FlexBox::AlignContent::center;
    main.justifyContent = juce::FlexBox::JustifyContent::center;
    main.items.add(juce::FlexItem(oscFM).withFlex(1.0f).withWidth(width));


    juce::FlexBox controls;
    controls.flexDirection = juce::FlexBox::Direction::row;
    controls.alignContent = juce::FlexBox::AlignContent::center;
    controls.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    juce::FlexBox enableOsc1;
    juce::FlexBox enableOsc2;
    juce::FlexBox freq;
    juce::FlexBox depth;

    enableOsc1.flexDirection = juce::FlexBox::Direction::column;
    enableOsc1.alignContent = juce::FlexBox::AlignContent::center;
    enableOsc1.justifyContent = juce::FlexBox::JustifyContent::center;

    enableOsc2.flexDirection = juce::FlexBox::Direction::column;
    enableOsc2.alignContent = juce::FlexBox::AlignContent::center;
    enableOsc2.justifyContent = juce::FlexBox::JustifyContent::center;

    freq.flexDirection = juce::FlexBox::Direction::column;
    freq.alignContent = juce::FlexBox::AlignContent::center;
    freq.justifyContent = juce::FlexBox::JustifyContent::center;

    depth.flexDirection = juce::FlexBox::Direction::column;
    depth.alignContent = juce::FlexBox::AlignContent::center;
    depth.justifyContent = juce::FlexBox::JustifyContent::center;

    enableOsc1.items.add(juce::FlexItem(enableOnOsc1Label).withHeight(20));
    enableOsc1.items.add(juce::FlexItem(enableOnOsc1).withFlex(1.0f));

    enableOsc2.items.add(juce::FlexItem(enableOnOsc2Label).withHeight(20));
    enableOsc2.items.add(juce::FlexItem(enableOnOsc2).withFlex(1.0f));

    freq.items.add(juce::FlexItem(freqFMLabel).withHeight(20));
    freq.items.add(juce::FlexItem(freqFM).withFlex(1.0f));

    depth.items.add(juce::FlexItem(depthFMLabel).withHeight(20));
    depth.items.add(juce::FlexItem(depthFM).withFlex(1.0f));

    controls.items.add(juce::FlexItem(enableOsc1).withFlex(1.0f));
    controls.items.add(juce::FlexItem(enableOsc2).withFlex(1.0f));
    controls.items.add(juce::FlexItem(freq).withFlex(1.0f));
    controls.items.add(juce::FlexItem(depth).withFlex(1.0f));

    main.items.add(juce::FlexItem(controls).withFlex(1.0f).withWidth(width));

    main.performLayout(bounds);
}
