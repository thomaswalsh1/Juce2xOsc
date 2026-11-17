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

using namespace Style::Layout;

//==============================================================================
FilterPage::FilterPage(juce::AudioProcessorValueTreeState& apvts)
    : type(apvts, ParamIDs::filterType, "Type"),
      cutoff(apvts, ParamIDs::filterCutoff, "Cutoff"),
      resonance(apvts, ParamIDs::filterResonance, "Res"),
      envAmount(apvts, ParamIDs::filterEnvAmount, "Env Amt"),
      keytrack(apvts, ParamIDs::filterKeytrack, "Key"),
      filterEnvelope(apvts, ParamIDs::filterEnv, "FILTER ENVELOPE")
{
    addAndMakeVisible(filterSection);
    addAndMakeVisible(filterEnvelope);

    for (auto* c : std::initializer_list<juce::Component*> { &type, &cutoff, &resonance, &envAmount, &keytrack })
        filterSection.addAndMakeVisible(c);
}

void FilterPage::paint (juce::Graphics& g)
{
    g.fillAll(Style::page);
}

void FilterPage::resized()
{
    auto bounds = getLocalBounds().reduced(spacing);

    filterSection.setBounds(takeTopRow(bounds));
    auto content = filterSection.getContentBounds();
    layoutSelectorRow(takeSelectorRow(content), { &type });
    layoutColumns(content, { &cutoff, &resonance, &envAmount, &keytrack }, knobColumnWidth);

    filterEnvelope.setBounds(bounds);
}
