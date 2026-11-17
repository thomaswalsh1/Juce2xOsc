/*
  ==============================================================================

    ModPage.cpp
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ModPage.h"
#include "Parameters.h"

//==============================================================================
ModPage::ModPage(juce::AudioProcessorValueTreeState& apvts)
    : lfoWave(apvts, ParamIDs::lfoWave, "Wave"),
      lfoRate(apvts, ParamIDs::lfoRate, "Rate"),
      lfoToPitch(apvts, ParamIDs::lfoToPitch, "Pitch"),
      lfoToCutoff(apvts, ParamIDs::lfoToCutoff, "Cutoff"),
      lfoToFmIndex(apvts, ParamIDs::lfoToFmIndex, "FM Idx"),
      lfoModWheel(apvts, ParamIDs::lfoModWheel, "Wheel"),
      velToAmp(apvts, ParamIDs::velToAmp, "Amp"),
      velToCutoff(apvts, ParamIDs::velToCutoff, "Cutoff"),
      velToFmIndex(apvts, ParamIDs::velToFmIndex, "FM Idx")
{
    setupLabel(lfoTitle, "LFO", Style::FontSize::large, juce::Justification::left);
    setupLabel(velocityTitle, "Velocity", Style::FontSize::large, juce::Justification::left);

    for (auto* c : std::initializer_list<juce::Component*> { &lfoTitle, &lfoWave, &lfoRate, &lfoToPitch, &lfoToCutoff,
                                                             &lfoToFmIndex, &lfoModWheel, &velocityTitle,
                                                             &velToAmp, &velToCutoff, &velToFmIndex })
        addAndMakeVisible(c);
}

void ModPage::paint (juce::Graphics& g)
{
    g.fillAll(Style::background2);

    g.setColour(Style::accent);
    g.fillRect(lfoArea);
    g.fillRect(velocityArea);
}

void ModPage::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    const int gap = 10;

    lfoArea = bounds.removeFromTop((bounds.getHeight() - gap) / 2);
    bounds.removeFromTop(gap);
    velocityArea = bounds;

    auto lfoBounds = lfoArea;
    lfoTitle.setBounds(lfoBounds.removeFromTop(20).withTrimmedLeft(5));
    lfoBounds.reduce(5, 5);
    lfoWave.setBounds(lfoBounds.removeFromLeft(80).withSizeKeepingCentre(80, 50));
    lfoBounds.removeFromLeft(5);
    const int knobWidth = lfoBounds.getWidth() / 5;
    layoutRow(lfoBounds, { &lfoRate, &lfoToPitch, &lfoToCutoff, &lfoToFmIndex, &lfoModWheel });

    auto velocityBounds = velocityArea;
    velocityTitle.setBounds(velocityBounds.removeFromTop(20).withTrimmedLeft(5));
    // same knob width as the lfo row, so the knobs are the same size in both sections
    layoutRow(velocityBounds.reduced(5).removeFromLeft(knobWidth * 3), { &velToAmp, &velToCutoff, &velToFmIndex });
}
