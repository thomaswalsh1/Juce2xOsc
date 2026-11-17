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
    // palette, every colour has exactly one job
    const juce::Colour window = juce::Colour::fromRGB(35, 35, 35);        // editor background and header
    const juce::Colour page = juce::Colour::fromRGB(177, 178, 181);       // tab pages, the area behind the sections
    const juce::Colour panel = juce::Colour::fromRGB(219, 214, 214);      // section boxes and unselected tabs
    const juce::Colour field = juce::Colour::fromRGB(238, 235, 235);      // value boxes, combo boxes, fader caps
    const juce::Colour ink = juce::Colour::fromRGB(35, 35, 35);           // text, outlines and "on/selected" fills on light surfaces
    const juce::Colour inkMuted = juce::Colour::fromRGB(95, 95, 95);      // secondary marks: field outlines, fader ticks, inactive tab text
    const juce::Colour textOnDark = juce::Colour::fromRGB(219, 214, 214); // text on the window colour

    static inline const juce::String GothicFontName = "Special Gothic Expanded One Regular";

    namespace FontSize {
        constexpr float small = 12.0f;     // control labels, tabs
        constexpr float medium = 14.0f;    // section titles
        constexpr float heading = 24.0f;   // plugin name
        constexpr float value = 13.0f;     // numbers and choices in value boxes / combo boxes
    }

    // labels and titles use the house font
    inline juce::FontOptions font(float size) {
        return juce::FontOptions(GothicFontName, size, juce::Font::bold);
    }

    // values use a plain font, the expanded house font is too wide for numbers like 20000.0
    inline juce::FontOptions valueFont() {
        return juce::FontOptions(FontSize::value);
    }

    // every size and gap in the ui comes from here, all spacing is one value so gaps always match
    namespace Layout {
        constexpr int spacing = 8;             // page edges, between sections, inside sections
        constexpr int headerHeight = 44;
        constexpr int tabBarHeight = 24;
        constexpr int tabWidth = 72;
        constexpr int titleHeight = 18;        // section titles
        constexpr int labelHeight = 16;        // control labels
        constexpr int selectorRowHeight = 40;  // row of combo boxes / toggles at the top of a section
        constexpr int comboWidth = 96;
        constexpr int comboHeight = 22;
        constexpr int toggleColumnWidth = 48;
        constexpr int toggleSize = 18;
        constexpr int knobSize = 52;
        constexpr int knobColumnWidth = 72;    // widest a knob column gets, so knobs line up across sections
        constexpr int faderColumnWidth = 48;
        constexpr int faderCapWidth = 28;
        constexpr int faderCapHeight = 14;
        constexpr int textBoxWidth = 52;
        constexpr int textBoxHeight = 18;
        constexpr float cornerRadius = 4.0f;
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

        // default control colours, so individual components never set their own
        setColour(juce::Label::textColourId, Style::ink);

        setColour(juce::Slider::textBoxTextColourId, Style::ink);
        setColour(juce::Slider::textBoxBackgroundColourId, Style::field);
        setColour(juce::Slider::textBoxOutlineColourId, Style::inkMuted);
        setColour(juce::Slider::textBoxHighlightColourId, Style::inkMuted.withAlpha(0.3f));

        setColour(juce::ComboBox::backgroundColourId, Style::field);
        setColour(juce::ComboBox::textColourId, Style::ink);
        setColour(juce::ComboBox::outlineColourId, Style::inkMuted);
        setColour(juce::ComboBox::arrowColourId, Style::ink);

        setColour(juce::PopupMenu::backgroundColourId, Style::field);
        setColour(juce::PopupMenu::textColourId, Style::ink);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, Style::ink);
        setColour(juce::PopupMenu::highlightedTextColourId, Style::textOnDark);

        setColour(juce::TextEditor::textColourId, Style::ink);
        setColour(juce::TextEditor::backgroundColourId, Style::field);
        setColour(juce::TextEditor::highlightColourId, Style::inkMuted.withAlpha(0.3f));
        setColour(juce::CaretComponent::caretColourId, Style::ink);
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

    //==============================================================================
    // knobs

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

    //==============================================================================
    // vertical faders: a slot with scale ticks either side and a flat cap with a centre line

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (style != juce::Slider::LinearVertical)
        {
            LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
            return;
        }

        using namespace Style::Layout;
        const float centreX = (float)x + (float)width * 0.5f;
        const float top = maxSliderPos;       // vertical sliders have their maximum at the top
        const float bottom = minSliderPos;

        // scale: 11 ticks, the ends and middle are longer
        g.setColour(Style::inkMuted);
        for (int i = 0; i <= 10; ++i)
        {
            const float tickY = top + (bottom - top) * (float)i / 10.0f;
            const float length = (i % 5 == 0) ? 6.0f : 3.0f;
            const float inner = (float)faderCapWidth * 0.5f + 2.0f;
            g.drawLine(centreX - inner - length, tickY, centreX - inner, tickY, 1.0f);
            g.drawLine(centreX + inner, tickY, centreX + inner + length, tickY, 1.0f);
        }

        // slot the cap runs in
        g.setColour(Style::ink);
        g.fillRoundedRectangle(juce::Rectangle<float>(centreX - 2.0f, top, 4.0f, bottom - top), 2.0f);

        // cap
        auto cap = juce::Rectangle<float>((float)faderCapWidth, (float)faderCapHeight).withCentre({ centreX, sliderPos });
        g.setColour(Style::field);
        g.fillRoundedRectangle(cap, 2.0f);
        g.setColour(Style::ink);
        g.drawRoundedRectangle(cap, 2.0f, 1.0f);
        g.drawLine(cap.getX() + 3.0f, cap.getCentreY(), cap.getRight() - 3.0f, cap.getCentreY(), 1.5f);
    }

    // keeps the fader cap inside the slider bounds at both ends of its travel
    int getSliderThumbRadius(juce::Slider& slider) override
    {
        if (slider.getSliderStyle() == juce::Slider::LinearVertical)
            return Style::Layout::faderCapHeight / 2 + 1;
        return LookAndFeel_V4::getSliderThumbRadius(slider);
    }

    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        auto* label = LookAndFeel_V4::createSliderTextBox(slider);
        label->setFont(Style::valueFont());
        return label;
    }

    //==============================================================================
    // combo boxes and their popup menus

    void drawComboBox(juce::Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox& box) override
    {
        auto bounds = juce::Rectangle<float>((float)width, (float)height).reduced(0.5f);
        g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
        g.fillRoundedRectangle(bounds, Style::Layout::cornerRadius);
        g.setColour(box.findColour(juce::ComboBox::outlineColourId));
        g.drawRoundedRectangle(bounds, Style::Layout::cornerRadius, 1.0f);

        // small down arrow on the right
        const float arrowX = (float)width - 12.0f;
        const float arrowY = (float)height * 0.5f;
        juce::Path arrow;
        arrow.addTriangle(arrowX - 4.0f, arrowY - 2.0f, arrowX + 4.0f, arrowY - 2.0f, arrowX, arrowY + 3.0f);
        g.setColour(box.findColour(juce::ComboBox::arrowColourId).withMultipliedAlpha(box.isEnabled() ? 1.0f : 0.4f));
        g.fillPath(arrow);
    }

    juce::Font getComboBoxFont(juce::ComboBox&) override
    {
        return Style::valueFont();
    }

    juce::Font getPopupMenuFont() override
    {
        return Style::valueFont();
    }

    //==============================================================================
    // toggles are just a box, the label above them comes from LabeledToggle

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool, bool) override
    {
        const float size = (float)Style::Layout::toggleSize;
        auto box = button.getLocalBounds().toFloat().withSizeKeepingCentre(size, size).reduced(0.5f);
        const float alpha = button.isEnabled() ? 1.0f : 0.4f;

        g.setColour(Style::field.withMultipliedAlpha(alpha));
        g.fillRoundedRectangle(box, Style::Layout::cornerRadius);
        g.setColour(Style::inkMuted.withMultipliedAlpha(alpha));
        g.drawRoundedRectangle(box, Style::Layout::cornerRadius, 1.0f);

        if (button.getToggleState())
        {
            g.setColour(Style::ink.withMultipliedAlpha(alpha));
            g.fillRoundedRectangle(box.reduced(4.0f), 2.0f);
        }
    }

    //==============================================================================
    // tabs: the selected tab is filled with ink, like a toggle that's on

	int getTabButtonBestWidth(juce::TabBarButton&, int) override
	{
        return Style::Layout::tabWidth;
	}

    int getTabButtonOverlap(int) override { return 0; }
    int getTabButtonSpaceAroundImage() override { return 0; }

    void drawTabbedButtonBarBackground(juce::TabbedButtonBar&, juce::Graphics&) override {}
    void drawTabAreaBehindFrontButton(juce::TabbedButtonBar&, juce::Graphics&, int, int) override {}

	void drawTabButton(juce::TabBarButton& button, juce::Graphics& g, bool, bool) override
	{
        // 1px either side so neighbouring tabs read as separate buttons
		auto bounds = button.getLocalBounds().toFloat().reduced(1.0f, 0.0f);
        const bool selected = button.isFrontTab();

		g.setColour(selected ? Style::ink : Style::panel);
		g.fillRoundedRectangle(bounds.withHeight(bounds.getHeight() + Style::Layout::cornerRadius), Style::Layout::cornerRadius);

        g.setFont(Style::font(Style::FontSize::small));
		g.setColour(selected ? Style::textOnDark : Style::inkMuted);
        g.drawText(button.getButtonText(), bounds, juce::Justification::centred);
	}

private:
    juce::Typeface::Ptr gothicTypeface;
    std::unique_ptr<juce::Drawable> knobDrawable;
};
