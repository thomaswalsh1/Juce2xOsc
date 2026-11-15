/*
  ==============================================================================

    PageTabs.h
    Author:  Thomas

    The two tabbed areas of the editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "OscPage.h"
#include "FmPage.h"
#include "FilterPage.h"

// oscillators and the fm modulator
class OscillatorTabs : public juce::TabbedComponent
{
public:
    OscillatorTabs(juce::AudioProcessorValueTreeState& apvts)
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop), oscPage(apvts), fmPage(apvts)
    {
        setTabBarDepth(20);
        addTab("Osc", Style::accent, &oscPage, false);
        addTab("FM", Style::accent, &fmPage, false);
        setCurrentTabIndex(0, false);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(Style::background2);
    }

private:
    OscPage oscPage;
    FmPage fmPage;
};

// filter and other effects
class EffectsTabs : public juce::TabbedComponent
{
public:
    EffectsTabs(juce::AudioProcessorValueTreeState& apvts)
        : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop), filterPage(apvts)
    {
        setTabBarDepth(20);
        addTab("Filter", Style::accent, &filterPage, false);
        setCurrentTabIndex(0, false);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(Style::background2);
    }

private:
    FilterPage filterPage;
};
