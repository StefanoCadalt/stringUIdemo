/*
  ==============================================================================

    StringComponent.h
    Created: 30 Apr 2026 11:35:34am
    Author:  enric

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    Corda vibrante orizzontale (Karplus-Strong visivo).
    Tutte le 6 corde hanno la stessa lunghezza grafica (come sulla chitarra).
*/
class StringComponent final : public juce::Component,
    private juce::Timer
{
public:
    //costruttore
    StringComponent(juce::Colour stringColour) : colour(stringColour)
    {
        //permetto al mouse di premere il component
        setInterceptsMouseClicks(false, false);
        //avvio un timer che chiama timerCall n volte al secondo
        startTimerHz(60);
    }

    /// <summary>
    /// Simulo pizzichio corda.
    /// <br>pizzichio al centro = vibrazione massima.</br>
    /// <br>pizzichio vicino ai bordi = vibrazione minima.</br>
    /// </summary>
    void stringPlucked(float pluckPositionRelative)
    {

        //pluckPositionRelative è un valore tra 0 e 1 della posizione nella corda
        //calcolo l'ampiezza della vibrazione
        amplitude = maxAmplitude * std::sin(pluckPositionRelative * juce::MathConstants<float>::pi);
        //imposta la fase nel punto corretto (sinusoide sincronizzata) 
        phase = juce::MathConstants<float>::pi;
    }

    /// <summary>
    /// Imposto il colore della corda.
    /// </summary>
    /// <param name="g"></param>
    void paint(juce::Graphics& g) override
    {
        // 1. Disegno il "bagliore" (Glow)
        // Prendo il colore originale e gli abbasso l'opacità
        g.setColour(colour.withAlpha(0.3f));
        // Disegno la linea molto spessa (6.0f)
        g.strokePath(generateStringPath(), juce::PathStrokeType(6.0f));

        // 2. Disegno il "nucleo" luminoso
        // Colore pieno
        g.setColour(colour);
        // Linea sottile e tagliente (1.5f)
        g.strokePath(generateStringPath(), juce::PathStrokeType(1.5f));
    }

    /// <summary>
    /// Costruisco la forma della corda che può oscillare
    /// </summary>
    /// <returns></returns>
    juce::Path generateStringPath() const
    {
        //larghezza
        float w = (float)getWidth();
        //centro verticale
        float y = (float)getHeight() / 2.0f;

        juce::Path p;
        //parto da sinistra a disegnare
        p.startNewSubPath(0.0f, y);
        //curva quadratica che si muove dall'alto al basso così da dare l'effetto del movimento
        p.quadraticTo(w / 2.0f, y + std::sin(phase) * amplitude, w, y);
        return p;
    }

private:
    /// <summary>
    /// Chiamata n volte al secondo per gestire le animazioni
    /// </summary>
    void timerCallback() override
    {
        //diminuzione nel tempo dell'oscillazione corda
        amplitude *= 0.99f;

        //velocità di oscillazione
        float phaseStep = 400.0f / (float)juce::jmax(1, getWidth());
        phase += phaseStep;

        //loop phase mantenendola tra [0,2piGreco]
        if (phase >= juce::MathConstants<float>::twoPi)
            phase -= juce::MathConstants<float>::twoPi;

        //refresh grafico
        repaint();
    }

    //colore corda
    juce::Colour colour;
    //ampiezza iniziale
    float amplitude = 0.0f;
    //massima ampiezza
    const float maxAmplitude = 12.0f;
    //fase iniziale
    float phase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringComponent)
};