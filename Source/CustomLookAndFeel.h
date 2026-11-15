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
    const juce::Colour background = juce::Colour::fromRGB(35, 35, 35);
    const juce::Colour background2 = juce::Colour::fromRGB(177, 178, 181);
    const juce::Colour surface = juce::Colour::fromRGB(95, 95, 95);
    const juce::Colour surface2 = juce::Colours::slategrey;
    const juce::Colour accent = juce::Colour::fromRGB(219, 214, 214);
    const juce::Colour textMain = juce::Colour::fromRGB(219, 214, 214);
    const juce::Colour textSecondary = juce::Colour::fromRGB(35, 35, 35);

    static inline const juce::String GothicFontName = "Special Gothic Expanded One Regular";

    namespace FontSize {
        constexpr float small = 12.0f;
        constexpr float medium = 14.0f;
        constexpr float large = 18.0f;
        constexpr float heading = 28.0f;
    }

    inline juce::FontOptions font(float size) {
        return juce::FontOptions(GothicFontName, size, juce::Font::bold);
    }
}


class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
		gothicTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::SpecialGothicExpandedOneRegular_ttf, BinaryData::SpecialGothicExpandedOneRegular_ttfSize);

        // parse the knob once instead of on every repaint
        juce::String svgString(BinaryData::knobOptimized_svg, BinaryData::knobOptimized_svgSize);
        knobDrawable = juce::Drawable::createFromSVGString(svgString);

        // default control colours, so individual components don't have to set them
        setColour(juce::Slider::textBoxOutlineColourId, Style::textSecondary);
        setColour(juce::Slider::textBoxTextColourId, Style::textSecondary);
        setColour(juce::ComboBox::backgroundColourId, Style::accent);
        setColour(juce::ComboBox::textColourId, Style::textSecondary);
        setColour(juce::ComboBox::outlineColourId, Style::textSecondary);
        setColour(juce::ComboBox::arrowColourId, Style::textSecondary);
        setColour(juce::ToggleButton::textColourId, Style::textSecondary);
        setColour(juce::ToggleButton::tickColourId, Style::textSecondary);
        setColour(juce::ToggleButton::tickDisabledColourId, Style::surface);
    }

	juce::Typeface::Ptr getTypefaceForFont(const juce::Font& font) override
	{
        // header
		if (font.getTypefaceName() == Style::GothicFontName && gothicTypeface != nullptr)
		{
			return gothicTypeface;
		}

        // fallback
		return LookAndFeel_V4::getTypefaceForFont(font);
	}

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override
    {
        if (knobDrawable != nullptr)
        {
            juce::Rectangle<float> target((float)x, (float)y, (float)width, (float)height);
            float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

            auto transform = juce::AffineTransform::rotation(angle, target.getCentreX(), target.getCentreY());

            knobDrawable->setDrawableTransformToFit(target, juce::RectanglePlacement::centred);
            knobDrawable->draw(g, 1.0f, transform);
        }
    }

	int getTabButtonBestWidth(juce::TabBarButton&, int) override
	{
        return 50;
	}

	void drawTabButton(juce::TabBarButton& button, juce::Graphics& g, bool, bool) override
	{
		auto bounds = button.getLocalBounds().toFloat();
		auto tabColour = button.getTabBackgroundColour();
		g.setColour(tabColour);
		g.fillRect(bounds);
		if (button.isFrontTab())
		{
			g.setColour(tabColour.contrasting(0.3f));
			g.drawRect(bounds, 1.0f);
		}
        g.setFont(Style::font(Style::FontSize::small));
		g.setColour(Style::background);
        g.drawText(button.getButtonText(), bounds, juce::Justification::centred);
	}
private:
    juce::Typeface::Ptr gothicTypeface;
    std::unique_ptr<juce::Drawable> knobDrawable;
};