/*
  ==============================================================================

    FmPage.cpp
    Created: 24 Oct 2026 12:21:50pm
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FmPage.h"
#include "Parameters.h"

using namespace Style::Layout;

//==============================================================================
FmPage::FmPage(juce::AudioProcessorValueTreeState& apvts)
    : wave(apvts, ParamIDs::fm + ParamIDs::wave, "Wave"),
      ratio(apvts, ParamIDs::fmRatio, "Ratio"),
      fine(apvts, ParamIDs::fm + ParamIDs::fine, "Fine"),
      index(apvts, ParamIDs::fmIndex, "Index"),
      toOsc1(apvts, ParamIDs::fmToOsc1, "Osc 1"),
      toOsc2(apvts, ParamIDs::fmToOsc2, "Osc 2"),
      fmEnvelope(apvts, ParamIDs::fm, "FM ENVELOPE")
{
    for (auto* c : std::initializer_list<juce::Component*> { &modulatorSection, &routingSection, &fmEnvelope })
        addAndMakeVisible(c);

    for (auto* c : std::initializer_list<juce::Component*> { &wave, &ratio, &fine, &index })
        modulatorSection.addAndMakeVisible(c);

    routingSection.addAndMakeVisible(toOsc1);
    routingSection.addAndMakeVisible(toOsc2);
}

void FmPage::paint (juce::Graphics& g)
{
    g.fillAll(Style::page);
}

void FmPage::resized()
{
    auto bounds = getLocalBounds().reduced(spacing);

    auto topRow = takeTopRow(bounds);
    routingSection.setBounds(topRow.removeFromRight(sectionWidth(2 * toggleColumnWidth + spacing)));
    topRow.removeFromRight(spacing);
    modulatorSection.setBounds(topRow);

    auto modContent = modulatorSection.getContentBounds();
    layoutSelectorRow(takeSelectorRow(modContent), { &wave });
    layoutColumns(modContent, { &ratio, &fine, &index }, knobColumnWidth);

    // the toggles sit in the selector row, level with the wave box next door
    auto routingContent = routingSection.getContentBounds();
    layoutSelectorRow(takeSelectorRow(routingContent), { &toOsc1, &toOsc2 });

    fmEnvelope.setBounds(bounds);
}
