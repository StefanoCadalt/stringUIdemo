/*
  ==============================================================================

    KnobStyle.h
    Created: 12 May 2026 12:41:45pm
    Author:  enric

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class KnobStyle  : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider&) override
    {
        auto radius = juce::jmin(width, height) / 2.0f - 10.0f;
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;

        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Colore Ciano Ghiaccio
        juce::Colour cianoGhiaccio = juce::Colour(0xFF00D4FF);

        // Sfondo interno trasparente/scuro (buco)
        g.setColour(juce::Colour(0xFF08080A));
        g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

        // Bordo dell'anello: usa un alpha basso per non appesantire (es. 0.3f)
        g.setColour(cianoGhiaccio.withAlpha(0.3f));
        g.drawEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 2.5f);

        // Linea indicatore (Il "LED" che segna il valore)
        juce::Path p;
        auto pointerLength = radius * 0.85f;
        auto pointerThickness = 3.0f;

        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);

        // Disegno l'indicatore con il ciano pieno e luminoso
        g.setColour(cianoGhiaccio);
        g.fillPath(p, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    }



    void drawButtonBackground(juce::Graphics& g,
        juce::Button& button,
        const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        float cornerSize = 1.0f; // Angoli quasi squadrati, molto tecnici

        if (button.getToggleState())
        {
            // STATO ON: Sfondo leggermente illuminato, bordo Ciano Ghiaccio acceso
            g.setColour(juce::Colour(0xFF00D4FF).withAlpha(0.15f));
            g.fillRoundedRectangle(bounds, cornerSize);

            g.setColour(juce::Colour(0xFF00D4FF));
            g.drawRoundedRectangle(bounds, cornerSize, 1.5f);
        }
        else
        {
            // STATO OFF: Sfondo nero totale, bordo verde molto tenue (come i pannelli)
            g.setColour(juce::Colour(0xFF08080A));
            g.fillRoundedRectangle(bounds, cornerSize);

            g.setColour(juce::Colour(0xFF20D065).withAlpha(0.2f));
            g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
        }
    }
};
