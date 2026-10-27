/*
  ==============================================================================

    FilterPage.h
    Created: 24 Oct 2026 12:23:24pm
    Author:  Thomas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterPage  : public juce::Component
{
public:
    FilterPage();
    ~FilterPage() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterPage)
};
