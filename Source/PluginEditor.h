/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OscPage.h"
#include "FmPage.h"
#include "FilterPage.h"
#include "CustomLookAndFeel.h";


// Tabs1 = Oscillators and FM Oscillator
class Tabs1 : public juce::TabbedComponent
{
public:
    Tabs1() : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop) {
        
        setTabBarDepth(20);

        addTab("Osc", Style::accent, &oscPage, false);
        addTab("FM", Style::accent, &fmPage, false);
        
        setCurrentTabIndex(0, false);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(Style::background2);
    }

    ~Tabs1() override = default;
private:
    OscPage oscPage;
    FmPage fmPage;
};

// Tabs2 = Filter and other
class Tabs2 : public juce::TabbedComponent
{
public:
    Tabs2() : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop) {

        addTab("Filter", Style::accent, &filterPage, false);
        setTabBarDepth(20);
        setCurrentTabIndex(0, false);
    }

	void paint(juce::Graphics& g) override {
		g.fillAll(Style::background2);
	}

    ~Tabs2() override = default;
private:
    FilterPage filterPage;
};


//==============================================================================
/**
*/
class _2xOscSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _2xOscSynthAudioProcessorEditor (_2xOscSynthAudioProcessor&);
    ~_2xOscSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _2xOscSynthAudioProcessor& audioProcessor;
    
    // look and feel
    CustomLookAndFeel customLookAndFeel;

    // tabs
    Tabs1 tabs1;
    Tabs2 tabs2;

    // header
    juce::Label header;
    juce::Label author;

    // secondary background
	juce::Rectangle<int> secondaryBackground;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_2xOscSynthAudioProcessorEditor)
};
