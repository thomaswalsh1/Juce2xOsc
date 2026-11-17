/*
  ==============================================================================

    OscPage.cpp
    Created: 24 Oct 2026 12:21:40pm
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscPage.h"

OscillatorPanel::OscillatorPanel(juce::AudioProcessorValueTreeState& apvts, const juce::String& title, const juce::String& waveID,
                                 const juce::String& tuneID, const juce::String& tuneText, const juce::String& fineID)
    : wave(apvts, waveID, "Wave"),
      tune(apvts, tuneID, tuneText),
      fine(apvts, fineID, "Fine")
{
    setupLabel(titleLabel, title, Style::FontSize::large, juce::Justification::left);
    setupLabel(tuneLabel, "Tune", Style::FontSize::large, juce::Justification::left);

    for (auto* c : std::initializer_list<juce::Component*> { &titleLabel, &tuneLabel, &wave, &tune, &fine })
        addAndMakeVisible(c);
}

void OscillatorPanel::paint(juce::Graphics& g)
{
    g.fillAll(Style::background2);

    g.setColour(Style::accent);
    g.fillRect(tuneArea);
}

void OscillatorPanel::resized()
{
    auto bounds = getLocalBounds();
    const int gap = 10;

    // wave selector and title
    auto leftMost = bounds.removeFromLeft(80);
    titleLabel.setBounds(leftMost.removeFromBottom(30));
    wave.setBounds(leftMost.removeFromTop(50));

    bounds.removeFromLeft(gap);

    // tuning section
    tuneArea = bounds;
    auto tuneBounds = tuneArea;
    tuneLabel.setBounds(tuneBounds.removeFromTop(20).withTrimmedLeft(5));
    layoutRow(tuneBounds.reduced(5), { &tune, &fine });
}

//==============================================================================
OscPage::OscPage(juce::AudioProcessorValueTreeState& apvts)
    : osc1(apvts, "OSC1", ParamIDs::osc1 + ParamIDs::wave, ParamIDs::osc1 + ParamIDs::coarse, "Coarse", ParamIDs::osc1 + ParamIDs::fine),
      osc2(apvts, "OSC2", ParamIDs::osc2 + ParamIDs::wave, ParamIDs::osc2 + ParamIDs::coarse, "Coarse", ParamIDs::osc2 + ParamIDs::fine),
      ampEnvelope(apvts, ParamIDs::amp, "Amp"),
      voiceMode(apvts, ParamIDs::voiceMode, "Voice"),
      mix(apvts, ParamIDs::oscMix, "Mix", juce::Slider::LinearHorizontal),
      glide(apvts, ParamIDs::glideTime, "Glide", juce::Slider::LinearHorizontal),
      masterGain(apvts, ParamIDs::masterGain, "Master", juce::Slider::LinearHorizontal)
{
    for (auto* c : std::initializer_list<juce::Component*> { &osc1, &osc2, &ampEnvelope, &voiceMode, &mix, &glide, &masterGain })
        addAndMakeVisible(c);
}

void OscPage::paint(juce::Graphics& g)
{
    g.fillAll(Style::background2);
}

void OscPage::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    const int gap = 10;
    const int rowHeight = (bounds.getHeight() - 2 * gap) / 5;

    auto oscRow = bounds.removeFromTop(rowHeight * 2);
    bounds.removeFromTop(gap);
    osc1.setBounds(oscRow.removeFromLeft((oscRow.getWidth() - gap) / 2));
    oscRow.removeFromLeft(gap);
    osc2.setBounds(oscRow);

    ampEnvelope.setBounds(bounds.removeFromTop(rowHeight * 2));
    bounds.removeFromTop(gap);

    layoutRow(bounds, { &voiceMode, &mix, &glide, &masterGain });
}
