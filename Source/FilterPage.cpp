/*
  ==============================================================================

    FilterPage.cpp
    Created: 24 Oct 2026 12:23:24pm
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterPage.h"
#include "Parameters.h"

//==============================================================================
FilterPage::FilterPage(juce::AudioProcessorValueTreeState& apvts)
    : type(apvts, ParamIDs::filterType, "Type"),
      cutoff(apvts, ParamIDs::filterCutoff, "Cutoff"),
      resonance(apvts, ParamIDs::filterResonance, "Res"),
      envAmount(apvts, ParamIDs::filterEnvAmount, "Env Amt"),
      keytrack(apvts, ParamIDs::filterKeytrack, "Key"),
      filterEnvelope(apvts, ParamIDs::filterEnv, "Filter Env")
{
    setupLabel(titleLabel, "Filter", Style::FontSize::large, juce::Justification::left);

    for (auto* c : std::initializer_list<juce::Component*> { &titleLabel, &type, &cutoff, &resonance, &envAmount, &keytrack, &filterEnvelope })
        addAndMakeVisible(c);
}

void FilterPage::paint (juce::Graphics& g)
{
    g.fillAll(Style::background2);

    g.setColour(Style::accent);
    g.fillRect(filterArea);
}

void FilterPage::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    const int gap = 10;

    filterArea = bounds.removeFromTop((bounds.getHeight() - gap) / 2);
    bounds.removeFromTop(gap);

    auto filterBounds = filterArea;
    titleLabel.setBounds(filterBounds.removeFromTop(20).withTrimmedLeft(5));
    filterBounds.reduce(5, 5);
    type.setBounds(filterBounds.removeFromLeft(90).withSizeKeepingCentre(90, 50));
    filterBounds.removeFromLeft(5);
    layoutRow(filterBounds, { &cutoff, &resonance, &envAmount, &keytrack });

    filterEnvelope.setBounds(bounds);
}
