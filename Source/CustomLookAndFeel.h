/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 24 Oct 2026
    Author:  Thomas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Style {
    const juce::Colour background = juce::Colour::fromRGB(0, 0, 0);
    const juce::Colour surface = juce::Colours::lightgrey;
    const juce::Colour surface2 = juce::Colours::slategrey;
    const juce::Colour accent = juce::Colours::darkslategrey;
    const juce::Colour textMain = juce::Colours::white;
    const juce::Colour textSecondary = juce::Colour::fromRGB(180, 180, 180);
}


class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // Set default colors if desired
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(6.0f);

        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto center = bounds.getCentre();
        auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // 1. Draw background track ring
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(center.x, center.y, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);

        g.setColour(juce::Colour(0xff222222));
        g.strokePath(backgroundArc, juce::PathStrokeType(3.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 2. Draw active value arc (accent color)
        juce::Path valueArc;
        valueArc.addCentredArc(center.x, center.y, radius, radius, 0.0f, rotaryStartAngle, toAngle, true);

        g.setColour(juce::Colour(0xff00d2ff)); // Sleek cyan accent
        g.strokePath(valueArc, juce::PathStrokeType(3.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 3. Draw the solid knob body (inner circle)
        auto thumbRadius = radius - 7.0f;
        g.setColour(juce::Colour(0xff181818));
        g.fillEllipse(center.x - thumbRadius, center.y - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f);

        // Knob border outline
        g.setColour(juce::Colour(0xff3a3a3a));
        g.drawEllipse(center.x - thumbRadius, center.y - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f, 1.2f);

        // 4. Draw pointer indicator line
        juce::Path pointer;
        auto pointerLength = thumbRadius * 0.75f;
        auto pointerThickness = 2.0f;
        pointer.addRectangle(-pointerThickness / 2.0f, -thumbRadius, pointerThickness, pointerLength);

        pointer.applyTransform(juce::AffineTransform::rotation(toAngle).translated(center.x, center.y));

        g.setColour(juce::Colours::white);
        g.fillPath(pointer);
    }

    void drawTabButton(juce::TabBarButton& b, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override {
        auto bounds = b.getBounds();

        // background 
        g.setColour(Style::surface2);
        g.fillRect(bounds);
    }
};