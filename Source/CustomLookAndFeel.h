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
}


class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
		gothicTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::SpecialGothicExpandedOneRegular_ttf, BinaryData::SpecialGothicExpandedOneRegular_ttfSize);
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

    juce::Font getHeadingFont(float size = 28.0f) {
		return juce::Font(Style::GothicFontName, size, juce::Font::bold);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider& slider) override
    {
        auto* svgData = BinaryData::knobOptimized_svg;
        auto svgDataSize = BinaryData::knobOptimized_svgSize;
        juce::String svgString (svgData, svgDataSize);

        std::unique_ptr<juce::Drawable> drawable = juce::Drawable::createFromSVGString(svgString);

        if (drawable != nullptr)
        {
            juce::Rectangle<float> target(x, y, width, height);
            float angle = rotaryEndAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

            auto transform = juce::AffineTransform::rotation(angle, target.getCentreX(), target.getCentreY());

            drawable->setDrawableTransformToFit(target, juce::RectanglePlacement::centred);
            drawable->draw(g, 1.0f, transform);
        }
    }

	int getTabButtonBestWidth(juce::TabBarButton& button, int tabDepth) override
	{
        return 50;
	}

	void drawTabButton(juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
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
        g.setFont(getHeadingFont(12.0f));  
		g.setColour(Style::background);
        g.drawText(button.getButtonText(), bounds, juce::Justification::centred);
	}
private:
    juce::Typeface::Ptr gothicTypeface;
};