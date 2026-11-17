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
#include "ModPage.h"

// tab bar + page, styled the same for both tab areas
class StyledTabs : public juce::TabbedComponent
{
public:
    StyledTabs() : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
    {
        setTabBarDepth(Style::Layout::tabBarHeight);
        setOutline(0);
    }

    void addPage(const juce::String& name, juce::Component& page)
    {
        // the colour is unused, CustomLookAndFeel::drawTabButton picks the tab colours
        addTab(name, Style::panel, &page, false);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(Style::page);
    }
};

// oscillators and the fm modulator
class OscillatorTabs : public StyledTabs
{
public:
    OscillatorTabs(juce::AudioProcessorValueTreeState& apvts) : oscPage(apvts), fmPage(apvts)
    {
        addPage("OSC", oscPage);
        addPage("FM", fmPage);
        setCurrentTabIndex(0, false);
    }

private:
    OscPage oscPage;
    FmPage fmPage;
};

// filter and modulation
class EffectsTabs : public StyledTabs
{
public:
    EffectsTabs(juce::AudioProcessorValueTreeState& apvts) : filterPage(apvts), modPage(apvts)
    {
        addPage("FILTER", filterPage);
        addPage("MOD", modPage);
        setCurrentTabIndex(0, false);
    }

private:
    FilterPage filterPage;
    ModPage modPage;
};
