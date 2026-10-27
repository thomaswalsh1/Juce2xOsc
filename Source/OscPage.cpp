/*
  ==============================================================================

    OscPage.cpp
    Created: 24 Oct 2026 12:21:40pm
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscPage.h"

OscComponent::OscComponent(juce::String name)
{
    title.setText(name, juce::dontSendNotification);
    title.setJustificationType(juce::Justification::left);
    addAndMakeVisible(title);

    adsrLabel.setText("ADSR", juce::dontSendNotification);
    adsrLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(adsrLabel);

    // Helper lambda to initialize labels cleanly
    auto setupLabel = [this](juce::Label& label, const juce::String& text) {
        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setFont(juce::FontOptions(12.0f));
        addAndMakeVisible(label);
        };

    // Setup oscillator labels
    setupLabel(waveTypeLabel, "Wave");
    setupLabel(coarseTuneLabel, "Coarse");
    setupLabel(fineTuneLabel, "Fine");

    // Setup ADSR labels
    setupLabel(attackLabel, "Attack");
    setupLabel(decayLabel, "Decay");
    setupLabel(sustainLabel, "Sustain");
    setupLabel(releaseLabel, "Release");

    // First controls (Oscillator settings)
    waveType.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    waveType.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    addAndMakeVisible(waveType);

    coarseTune.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    coarseTune.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    addAndMakeVisible(coarseTune);

    fineTune.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    fineTune.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    addAndMakeVisible(fineTune);

    // Second controls (ADSR settings)
    attack.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attack.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    addAndMakeVisible(attack);

    decay.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decay.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    addAndMakeVisible(decay);

    sustain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustain.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    addAndMakeVisible(sustain);

    release.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    release.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    addAndMakeVisible(release);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint(juce::Graphics& g)
{
}

void OscComponent::resized()
{
    auto bounds = getLocalBounds().reduced(5);

    // Top header section (Osc name & ADSR header)
    auto headingSection = bounds.removeFromTop(20);
    int headingWidth = headingSection.getWidth();
    title.setBounds(headingSection.removeFromLeft(headingWidth / 2));
    adsrLabel.setBounds(headingSection);

    // Split remaining space into Left (Osc controls) and Right (ADSR controls)
    int firstControlWidth = bounds.getWidth() / 2;
    auto firstControls = bounds.removeFromLeft(firstControlWidth);
    auto adsrControls = bounds;

    // Layout helper to distribute a group of label+slider pairs evenly in a row
    auto layoutControlGroup = [](juce::Rectangle<int> area, std::initializer_list<std::pair<juce::Label*, juce::Slider*>> controls) {
        int numControls = (int)controls.size();
        if (numControls == 0) return;

        int itemWidth = area.getWidth() / numControls;

        for (auto& pair : controls) {
            auto itemArea = area.removeFromLeft(itemWidth).reduced(2, 0);

            // Allocate top 18 pixels for the label
            auto labelArea = itemArea.removeFromTop(18);
            pair.first->setBounds(labelArea);

            // Remaining space goes to the slider/knob
            pair.second->setBounds(itemArea);
        }
        };

    // Position left group (Wave, Coarse, Fine)
    layoutControlGroup(firstControls, {
        {&waveTypeLabel, &waveType},
        {&coarseTuneLabel, &coarseTune},
        {&fineTuneLabel, &fineTune}
        });

    // Position right group (Attack, Decay, Sustain, Release)
    layoutControlGroup(adsrControls, {
        {&attackLabel, &attack},
        {&decayLabel, &decay},
        {&sustainLabel, &sustain},
        {&releaseLabel, &release}
        });
}

//==============================================================================
OscPage::OscPage()
{
    setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);

    enableAdsr2.setButtonText("Adsr 2");
    addAndMakeVisible(enableAdsr2);

    mixControl.setSliderStyle(juce::Slider::LinearHorizontal);
    addAndMakeVisible(mixControl);
}

OscPage::~OscPage()
{
}

void OscPage::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // outline

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f));
}

void OscPage::resized()
{
    auto bounds = getLocalBounds().reduced(10);

    juce::FlexBox main;
    main.flexDirection = juce::FlexBox::Direction::column;
    main.justifyContent = juce::FlexBox::JustifyContent::center;
    main.alignItems = juce::FlexBox::AlignItems::center;

    const int fullWidth = bounds.getWidth();

    juce::FlexBox bottomControls;
    bottomControls.flexDirection = juce::FlexBox::Direction::row;
    bottomControls.justifyContent = juce::FlexBox::JustifyContent::center;
    bottomControls.alignItems = juce::FlexBox::AlignItems::center;
    bottomControls.items.add(juce::FlexItem(enableAdsr2).withFlex(1.0f).withHeight(30));
    bottomControls.items.add(juce::FlexItem(mixControl).withFlex(1.0f).withHeight(30));

    main.items.add(juce::FlexItem(osc1).withFlex(2.0f).withWidth(fullWidth));
    main.items.add(juce::FlexItem(osc2).withFlex(2.0f).withWidth(fullWidth));
    main.items.add(juce::FlexItem(bottomControls).withFlex(1.0f).withWidth(fullWidth));

    main.performLayout(bounds);
}