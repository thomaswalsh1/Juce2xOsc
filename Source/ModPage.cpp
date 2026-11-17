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
      lfoSync(apvts, ParamIDs::lfoSync, "Sync"),
      lfoFreeRun(apvts, ParamIDs::lfoFreeRun, "Free"),
      lfoRate(apvts, ParamIDs::lfoRate, "Rate"),
      lfoToPitch(apvts, ParamIDs::lfoToPitch, "Pitch"),
      lfoToCutoff(apvts, ParamIDs::lfoToCutoff, "Cutoff"),
      lfoToFmIndex(apvts, ParamIDs::lfoToFmIndex, "FM Idx"),
      lfoModWheel(apvts, ParamIDs::lfoModWheel, "Wheel"),
      lfoSyncRate(apvts, ParamIDs::lfoSyncRate, "Rate"),
      lfoSyncListener(*apvts.getParameter(ParamIDs::lfoSync), [this](float value) {
          const bool synced = value > 0.5f;
          lfoRate.setVisible(!synced);
          lfoSyncRate.setVisible(synced);
      }),
      velToAmp(apvts, ParamIDs::velToAmp, "Amp"),
      velToCutoff(apvts, ParamIDs::velToCutoff, "Cutoff"),
      velToFmIndex(apvts, ParamIDs::velToFmIndex, "FM Idx")
{
    setupLabel(lfoTitle, "LFO", Style::FontSize::large, juce::Justification::left);
    setupLabel(velocityTitle, "Velocity", Style::FontSize::large, juce::Justification::left);

    for (auto* c : std::initializer_list<juce::Component*> { &lfoTitle, &lfoWave, &lfoSync, &lfoFreeRun, &lfoRate, &lfoSyncRate, &lfoToPitch,
                                                             &lfoToCutoff, &lfoToFmIndex, &lfoModWheel, &velocityTitle,
                                                             &velToAmp, &velToCutoff, &velToFmIndex })
        addAndMakeVisible(c);

    // hides whichever rate control doesn't apply yet
    lfoSyncListener.sendInitialUpdate();
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
    // wave on top, sync and free side by side below it
    auto leftColumn = lfoBounds.removeFromLeft(90);
    lfoWave.setBounds(leftColumn.removeFromTop(leftColumn.getHeight() / 2));
    lfoSync.setBounds(leftColumn.removeFromLeft(leftColumn.getWidth() / 2));
    lfoFreeRun.setBounds(leftColumn);
    lfoBounds.removeFromLeft(5);
    const int knobWidth = lfoBounds.getWidth() / 5;
    layoutRow(lfoBounds, { &lfoRate, &lfoToPitch, &lfoToCutoff, &lfoToFmIndex, &lfoModWheel });
    lfoSyncRate.setBounds(lfoRate.getBounds());

    auto velocityBounds = velocityArea;
    velocityTitle.setBounds(velocityBounds.removeFromTop(20).withTrimmedLeft(5));
    // same knob width as the lfo row, so the knobs are the same size in both sections
    layoutRow(velocityBounds.reduced(5).removeFromLeft(knobWidth * 3), { &velToAmp, &velToCutoff, &velToFmIndex });
}
