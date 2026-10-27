/*
  ==============================================================================

    FmPage.h
    Created: 24 Oct 2026 12:21:50pm
    Author:  Thomas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FmPage  : public juce::Component
{
public:
    FmPage();
    ~FmPage() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmPage)
};
