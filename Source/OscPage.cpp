/*
  ==============================================================================

    OscPage.cpp
    Created: 24 Oct 2026 12:21:40pm
    Author:  Thomas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscPage.h"


void setupSmallerLabel(juce::Label& label, juce::String text)
{
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, Style::textSecondary);
    label.setFont(juce::FontOptions(Style::GothicFontName, 12.0f, juce::Font::bold));
}

OscComponent::OscComponent(juce::String name)
{
	juce::LookAndFeel& lnf = getLookAndFeel();

    title.setText(name, juce::dontSendNotification);
	title.setColour(juce::Label::textColourId, Style::textSecondary);
	title.setFont(juce::FontOptions(Style::GothicFontName, 18.0f, juce::Font::bold));
    title.setJustificationType(juce::Justification::left);
    addAndMakeVisible(title);

    // First controls (Oscillator settings)
	waveType.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
	waveType.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
    waveType.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    waveType.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    addAndMakeVisible(waveType);

    waveTypeLabel.setText("Wave", juce::dontSendNotification);
    waveTypeLabel.setJustificationType(juce::Justification::left);
	waveTypeLabel.setColour(juce::Label::textColourId, Style::textSecondary);
	waveTypeLabel.setFont(juce::FontOptions(Style::GothicFontName, 18.0f, juce::Font::bold));
    addAndMakeVisible(waveTypeLabel);

	detune.setText("Detune", juce::dontSendNotification);
	detune.setJustificationType(juce::Justification::left);
	detune.setColour(juce::Label::textColourId, Style::textSecondary);
	detune.setFont(juce::FontOptions(Style::GothicFontName, 18.0f, juce::Font::bold));
    addAndMakeVisible(detune);

	setupSmallerLabel(coarseTuneLabel, "Coarse");
	addAndMakeVisible(coarseTuneLabel);
	coarseTune.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
	coarseTune.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
    coarseTune.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    coarseTune.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(coarseTune);
    
	setupSmallerLabel(fineTuneLabel, "Fine");
	addAndMakeVisible(fineTuneLabel);
	fineTune.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
	fineTune.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
    fineTune.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    fineTune.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(fineTune);

	adsrLabel.setText("ADSR", juce::dontSendNotification);
	adsrLabel.setJustificationType(juce::Justification::left);
	adsrLabel.setColour(juce::Label::textColourId, Style::textSecondary);
	adsrLabel.setFont(juce::FontOptions(Style::GothicFontName, 18.0f, juce::Font::bold));
	addAndMakeVisible(adsrLabel);

    // Second controls (ADSR settings)
	setupSmallerLabel(attackLabel, "Attack");
	addAndMakeVisible(attackLabel);
	attack.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
	attack.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
    attack.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attack.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(attack);

	setupSmallerLabel(decayLabel, "Decay");
	addAndMakeVisible(decayLabel);
	decay.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
	decay.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
    decay.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decay.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(decay);

	setupSmallerLabel(sustainLabel, "Sustain");
	addAndMakeVisible(sustainLabel);
	sustain.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
	sustain.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
    sustain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(sustain);
    
	setupSmallerLabel(releaseLabel, "Release");
	addAndMakeVisible(releaseLabel);
	release.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
	release.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
    release.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    release.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(release);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint(juce::Graphics& g)
{
	// background
	g.fillAll(Style::background2);

    // bottom border
	g.setColour(juce::Colours::black);
    float thickness = 1.0f;
	float w = static_cast<float>(getWidth());
	float h = static_cast<float>(getHeight());

	g.drawLine(0.0f, h - thickness / 2.0f, w, h - thickness / 2.0f, thickness);

    // draw inner boxes
    g.setColour(Style::accent);
	g.fillRect(detuneArea);
	g.fillRect(adsrArea);

}

void OscComponent::resized()
{
	auto bounds = getLocalBounds().reduced(10);
    int gap = 10;

	auto leftMost = bounds.removeFromLeft(50);
	title.setBounds(leftMost.removeFromBottom(30));
	waveTypeLabel.setBounds(leftMost.removeFromTop(20));
    waveType.setBounds(leftMost);

    bounds.removeFromLeft(gap);

    // detuning section
	detuneArea = bounds.removeFromLeft(100);
	auto detuneBounds = detuneArea;
    detune.setBounds(detuneBounds.removeFromTop(20));
	juce::FlexBox detuneControls;
	detuneControls.flexDirection = juce::FlexBox::Direction::row;
	detuneControls.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
	detuneControls.alignContent = juce::FlexBox::AlignContent::center;
    juce::FlexBox coarseTuneArea;
	juce::FlexBox fineTuneArea;
	coarseTuneArea.flexDirection = juce::FlexBox::Direction::column;
	fineTuneArea.flexDirection = juce::FlexBox::Direction::column;
	coarseTuneArea.alignContent = juce::FlexBox::AlignContent::center;
	fineTuneArea.alignContent = juce::FlexBox::AlignContent::center;
	coarseTuneArea.items.add(juce::FlexItem(coarseTuneLabel).withHeight(20));
	coarseTuneArea.items.add(juce::FlexItem(coarseTune).withFlex(1.0f));
	fineTuneArea.items.add(juce::FlexItem(fineTuneLabel).withHeight(20));
	fineTuneArea.items.add(juce::FlexItem(fineTune).withFlex(1.0f));
	detuneControls.items.add(juce::FlexItem(coarseTuneArea).withFlex(1.0f));
	detuneControls.items.add(juce::FlexItem(fineTuneArea).withFlex(1.0f));
	detuneControls.performLayout(detuneBounds.reduced(5));

	bounds.removeFromLeft(gap);

    // adsr section
    adsrArea = bounds;
	auto adsrBounds = adsrArea;
	adsrLabel.setBounds(adsrBounds.removeFromTop(20));
	juce::FlexBox adsrControls;
	adsrControls.flexDirection = juce::FlexBox::Direction::row;
	adsrControls.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
	adsrControls.alignContent = juce::FlexBox::AlignContent::center;
	juce::FlexBox attackBox;
	juce::FlexBox decayBox;
	juce::FlexBox sustainBox;
	juce::FlexBox releaseBox;
	attackBox.flexDirection = juce::FlexBox::Direction::column;
	decayBox.flexDirection = juce::FlexBox::Direction::column;
	sustainBox.flexDirection = juce::FlexBox::Direction::column;
	releaseBox.flexDirection = juce::FlexBox::Direction::column;
	attackBox.items.add(juce::FlexItem(attackLabel).withHeight(20));
    decayBox.items.add(juce::FlexItem(decayLabel).withHeight(20));
	sustainBox.items.add(juce::FlexItem(sustainLabel).withHeight(20));
	releaseBox.items.add(juce::FlexItem(releaseLabel).withHeight(20));
	attackBox.items.add(juce::FlexItem(attack).withFlex(1.0f));
	decayBox.items.add(juce::FlexItem(decay).withFlex(1.0f));
	sustainBox.items.add(juce::FlexItem(sustain).withFlex(1.0f));
	releaseBox.items.add(juce::FlexItem(release).withFlex(1.0f));
	adsrControls.items.add(juce::FlexItem(attackBox).withFlex(1.0f));
	adsrControls.items.add(juce::FlexItem(decayBox).withFlex(1.0f));
	adsrControls.items.add(juce::FlexItem(sustainBox).withFlex(1.0f));
	adsrControls.items.add(juce::FlexItem(releaseBox).withFlex(1.0f));
	adsrControls.performLayout(adsrBounds.reduced(5));

}

//==============================================================================
OscPage::OscPage()
{
    setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);

	mixLabel.setText("Mix", juce::dontSendNotification);
	mixLabel.setJustificationType(juce::Justification::centred);
	mixLabel.setFont(juce::FontOptions(Style::GothicFontName, 14.0f, juce::Font::bold));
	mixLabel.setColour(juce::Label::textColourId, Style::textSecondary);
	addAndMakeVisible(mixLabel);

	enableAdsr2.setButtonText("Adsr 2");
    addAndMakeVisible(enableAdsr2);

    mixControl.setSliderStyle(juce::Slider::LinearHorizontal);
	mixControl.setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
	mixControl.setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
	mixControl.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    addAndMakeVisible(mixControl);
}

OscPage::~OscPage()
{
}

void OscPage::paint(juce::Graphics& g)
{
    g.fillAll(Style::background2);
	g.setFont(juce::FontOptions(Style::GothicFontName, 18.0f, juce::Font::bold));
}

void OscPage::resized()
{
    auto bounds = getLocalBounds();

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
    bottomControls.items.add(juce::FlexItem(mixLabel).withFlex(1.0f).withHeight(30));
    bottomControls.items.add(juce::FlexItem(mixControl).withFlex(1.0f).withHeight(30));

    main.items.add(juce::FlexItem(osc1).withFlex(2.0f).withWidth(fullWidth));
    main.items.add(juce::FlexItem(osc2).withFlex(2.0f).withWidth(fullWidth));
    main.items.add(juce::FlexItem(bottomControls).withFlex(1.0f).withWidth(fullWidth));

    main.performLayout(bounds);
}