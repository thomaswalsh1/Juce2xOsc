/*
  ==============================================================================

    FmPage.cpp
    Created: 24 Oct 2026 12:21:50pm
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FmPage.h"

//==============================================================================
FmPage::FmPage(juce::AudioProcessorValueTreeState& apvts)
    : modulatorPanel(apvts, "MOD", ParamIDs::fm + ParamIDs::wave, ParamIDs::fmRatio, "Ratio", ParamIDs::fm + ParamIDs::fine),
      index(apvts, ParamIDs::fmIndex, "Index"),
      toOsc1(apvts, ParamIDs::fmToOsc1, "OSC 1"),
      toOsc2(apvts, ParamIDs::fmToOsc2, "OSC 2"),
      fmEnvelope(apvts, ParamIDs::fm, "FM Env")
{
    for (auto* c : std::initializer_list<juce::Component*> { &modulatorPanel, &index, &toOsc1, &toOsc2, &fmEnvelope })
        addAndMakeVisible(c);
}

void FmPage::paint (juce::Graphics& g)
{
    g.fillAll(Style::background2);
}

void FmPage::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    const int gap = 10;

    auto topRow = bounds.removeFromTop((bounds.getHeight() - gap) / 2);
    bounds.removeFromTop(gap);

    modulatorPanel.setBounds(topRow.removeFromLeft((topRow.getWidth() - gap) / 2));
    topRow.removeFromLeft(gap);
    layoutRow(topRow, { &index, &toOsc1, &toOsc2 });

    fmEnvelope.setBounds(bounds);
}
