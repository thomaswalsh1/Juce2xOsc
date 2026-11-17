/*
  ==============================================================================

    OscPage.cpp
    Created: 24 Oct 2026 12:21:40pm
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscPage.h"

using namespace Style::Layout;

OscillatorPanel::OscillatorPanel(juce::AudioProcessorValueTreeState& apvts, const juce::String& title, const juce::String& idPrefix)
    : Section(title),
      wave(apvts, idPrefix + ParamIDs::wave, "Wave"),
      coarse(apvts, idPrefix + ParamIDs::coarse, "Coarse"),
      fine(apvts, idPrefix + ParamIDs::fine, "Fine")
{
    for (auto* c : std::initializer_list<juce::Component*> { &wave, &coarse, &fine })
        addAndMakeVisible(c);
}

void OscillatorPanel::resized()
{
    auto content = getContentBounds();
    layoutSelectorRow(takeSelectorRow(content), { &wave });
    layoutColumns(content, { &coarse, &fine }, knobColumnWidth);
}

//==============================================================================
OscPage::OscPage(juce::AudioProcessorValueTreeState& apvts)
    : osc1(apvts, "OSC 1", ParamIDs::osc1),
      osc2(apvts, "OSC 2", ParamIDs::osc2),
      ampEnvelope(apvts, ParamIDs::amp, "AMP ENVELOPE"),
      voiceMode(apvts, ParamIDs::voiceMode, "Voice"),
      mix(apvts, ParamIDs::oscMix, "Mix", juce::Slider::LinearVertical),
      glide(apvts, ParamIDs::glideTime, "Glide", juce::Slider::LinearVertical),
      masterGain(apvts, ParamIDs::masterGain, "Master", juce::Slider::LinearVertical)
{
    for (auto* c : std::initializer_list<juce::Component*> { &osc1, &osc2, &ampEnvelope, &globalSection })
        addAndMakeVisible(c);

    for (auto* c : std::initializer_list<juce::Component*> { &voiceMode, &mix, &glide, &masterGain })
        globalSection.addAndMakeVisible(c);
}

void OscPage::paint(juce::Graphics& g)
{
    g.fillAll(Style::page);
}

void OscPage::resized()
{
    auto bounds = getLocalBounds().reduced(spacing);

    auto topRow = takeTopRow(bounds);
    osc1.setBounds(topRow.removeFromLeft((topRow.getWidth() - spacing) / 2));
    topRow.removeFromLeft(spacing);
    osc2.setBounds(topRow);

    // global gets exactly enough room for its three faders, the envelope takes the rest
    globalSection.setBounds(bounds.removeFromRight(sectionWidth(3 * faderColumnWidth)));
    bounds.removeFromRight(spacing);
    ampEnvelope.setBounds(bounds);

    auto globalContent = globalSection.getContentBounds();
    layoutSelectorRow(takeSelectorRow(globalContent), { &voiceMode });
    layoutColumns(globalContent, { &mix, &glide, &masterGain }, faderColumnWidth);
}
