#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "StringComponent.h"
#include "KnobStyle.h"

//==============================================================================
class ZenkiGuitarModelAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    explicit ZenkiGuitarModelAudioProcessorEditor(ZenkiGuitarModelAudioProcessor&);
    ~ZenkiGuitarModelAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    // Colori delle corde (verde scalare, basso → alto)
    static juce::Colour stringColour(int index)
    {
        const juce::Colour colours[6] = {
            juce::Colour(0xFF004A15),  // E2: Verde molto scuro e profondo
            juce::Colour(0xFF007520),  // A2
            juce::Colour(0xFF00A830),  // D3
            juce::Colour(0xFF00D23A),  // G3
            juce::Colour(0xFF00FA40),  // B3: Verde puro
            juce::Colour(0xFF28FF5A)   // E4: Verde luminoso, ma assolutamente non bianco
        };
        return colours[index % 6];
    }

private:

    /// Titoli delle sezioni
    static constexpr int numSezioni = 7;
	juce::Label titoloSezione[numSezioni];

    // Manopole
    KnobStyle stilePomello;
    // 0: Drive, 1: gain
    static constexpr int numManopole = 13;
    juce::Slider manopolaEffetto[numManopole];
    juce::Label titoloManopolaEffetto[numManopole];

	// Attachment per collegamenti con APVTS (Componente UI <-> Parametro)
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> timeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hardnessAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> revMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> revSizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;

	// Callback del Timer (Per interazione Audio Thread -> UI Thread per la MIDI)
	void timerCallback() override;
    /* Permette di controllare periodicamente 
    se l'Audio Thread ha flaggato una corda come suonata.*/

    // Mouse
    void mouseDown(const juce::MouseEvent& e) override { handleMouseEvent(e); }
    void mouseDrag(const juce::MouseEvent& e) override { handleMouseEvent(e); }
    void mouseUp(const juce::MouseEvent& e) override { oldPosFret = -1; oldMidiNote = -1; }
    void handleMouseEvent(const juce::MouseEvent& e);

    // Paint helpers
    void SetTitle(juce::Graphics&);
    void SetLineaSeparatrice(juce::Graphics&);
    void SetStrings(juce::Graphics&);
    void SetSeparationFret(juce::Graphics&);

    // Tuning helpers
    // Aggiorna la label di tuning per la corda i (mostra nome nota + delta semitoni)
    void updateTuningLabel(int stringIndex);

    // Aggiorna tutte le label di tuning
    void updateAllTuningLabels();

	// Sezioni della UI
    juce::Rectangle<int> areaOscilloscopio;
    juce::Rectangle<int> areaMaster;
    juce::Rectangle<int> areaParametriFisici;
    juce::Rectangle<int> areaDelay;
    juce::Rectangle<int> areaDistortion;
    juce::Rectangle<int> areaReverb;
    juce::Rectangle<int> areaPhaser;
    juce::Rectangle<int> areaCordeSotto;

    // Dati
    ZenkiGuitarModelAudioProcessor& audioProcessor;

    juce::OwnedArray<StringComponent> stringComponents;

    // Controlli tuning: per ogni corda un pulsante "-", una label, un pulsante "+"
    juce::OwnedArray<juce::TextButton> tuningDownButtons;  // [−]
    juce::OwnedArray<juce::TextButton> tuningUpButtons;    // [+]
    juce::OwnedArray<juce::Label>      tuningLabels;       // "E2 (+0)"

    // Pulsante reset accordatura
    juce::TextButton resetTuningButton;

    // Label nota suonata corrente
    juce::Label notaSuonataLabel;

    // Costanti layout
    const int numFret = 12;
    const int numCorde = 6;

    // Larghezza della colonna tuning a sinistra delle corde
    // Layout: [−](22) [Label nota(50)] [+](22)
    static constexpr int tuningPanelWidth = 110;

    // Stato mouse (evita retriggering sulla stessa posizione)
    int oldPosFret = -1;
    int oldMidiNote = -1;

    // Sezione oscilloscopio
	// Il parametro 2 indica i due canali stereo (sinistro e destro)
    juce::AudioVisualiserComponent oscilloscopio{ 2 };

    // Rettangoli per disegnare i meter
	juce::Rectangle<int> meterLeftArea;
	juce::Rectangle<int> meterRightArea;

    // Variabili per memorizzare il valore scalato da disegnare
    float levelLeftScaled = 0.0f;
	float levelRightScaled = 0.0f;

	// Pulsanti ON/OFF per effetti
    juce::TextButton btnDelayOn{ "ON" };
    juce::TextButton btnDistOn{ "ON" };
    juce::TextButton btnRevOn{ "ON" };
    juce::TextButton btnPhaserOn{ "ON" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> atcDelayOn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> atcDistOn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> atcRevOn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> atcPhaserOn;

    // Sezione dei menu a tendina per i preset
    juce::ComboBox presetMenu;
    
    // Funzione per applicare i valori
    void applicaPreset(int presetId);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenkiGuitarModelAudioProcessorEditor)
};