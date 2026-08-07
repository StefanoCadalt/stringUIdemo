#pragma once

#include <JuceHeader.h>

//==============================================================================
class StringComponent final : public juce::Component, private juce::Timer
{
public:
    StringComponent(juce::Colour stringColour) : colour(stringColour)
    {
        // Disabilita l'intercettazione del mouse per delegarla all'Editor principale
        setInterceptsMouseClicks(false, false);
        startTimerHz(60);
    }

    // Innesca la vibrazione visiva. L'ampiezza massima si ottiene al centro (0.5)
    void stringPlucked(float pluckPositionRelative)
    {
        amplitude = maxAmplitude * std::sin(pluckPositionRelative * juce::MathConstants<float>::pi);
        phase = juce::MathConstants<float>::pi;
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(colour);
        g.strokePath(generateStringPath(), juce::PathStrokeType(2.5f));
    }

    juce::Path generateStringPath() const
    {
        float w = (float)getWidth();
        float y = (float)getHeight() / 2.0f;

        juce::Path p;
        p.startNewSubPath(0.0f, y);
        p.quadraticTo(w / 2.0f, y + std::sin(phase) * amplitude, w, y);

        return p;
    }

private:
    void timerCallback() override
    {
        // Decadimento dell'oscillazione
        amplitude *= 0.99f;

        float phaseStep = 400.0f / (float)juce::jmax(1, getWidth());
        phase += phaseStep;

        if (phase >= juce::MathConstants<float>::twoPi)
            phase -= juce::MathConstants<float>::twoPi;

        repaint();
    }

#pragma region Variabili di Stato Grafico
    juce::Colour colour;
    float amplitude = 0.0f;
    const float maxAmplitude = 12.0f;
    float phase = 0.0f;
#pragma endregion

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringComponent)
};