/*
  ==============================================================================

    ModPage.cpp
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ModPage.h"
#include "Parameters.h"

using namespace Style::Layout;

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
    addAndMakeVisible(lfoSection);
    addAndMakeVisible(velocitySection);

    for (auto* c : std::initializer_list<juce::Component*> { &lfoWave, &lfoSync, &lfoFreeRun, &lfoRate, &lfoSyncRate,
                                                             &lfoToPitch, &lfoToCutoff, &lfoToFmIndex, &lfoModWheel })
        lfoSection.addAndMakeVisible(c);

    for (auto* c : std::initializer_list<juce::Component*> { &velToAmp, &velToCutoff, &velToFmIndex })
        velocitySection.addAndMakeVisible(c);

    // hides whichever rate control doesn't apply yet
    lfoSyncListener.sendInitialUpdate();
}

void ModPage::paint (juce::Graphics& g)
{
    g.fillAll(Style::page);
}

void ModPage::resized()
{
    auto bounds = getLocalBounds().reduced(spacing);

    lfoSection.setBounds(takeTopRow(bounds));
    auto lfoContent = lfoSection.getContentBounds();
    layoutSelectorRow(takeSelectorRow(lfoContent), { &lfoWave, &lfoSync, &lfoFreeRun });
    layoutColumns(lfoContent, { &lfoRate, &lfoToPitch, &lfoToCutoff, &lfoToFmIndex, &lfoModWheel }, knobColumnWidth);
    lfoSyncRate.setBounds(lfoRate.getBounds());

    velocitySection.setBounds(bounds);
    layoutColumns(velocitySection.getContentBounds(), { &velToAmp, &velToCutoff, &velToFmIndex }, knobColumnWidth);
}
