#include <sstream>
#include "PluginEditor.h"


// Constructor
NeuralAmpAudioProcessorEditor::NeuralAmpAudioProcessorEditor (NeuralAmpAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);

    // Initialize the gain knob
    addAndMakeVisible(gainKnob);
    // Set the properties of the gain knob
    gainKnob.setSliderStyle(juce::Slider::Rotary);
    // Set the range and default value for the gain knob
    gainKnob.setRange(0.0, 2.0);
    // Set the default value of the gain knob to 1.0 (no gain)
    gainKnob.setValue(1.0); 

    // Set the callback for when the gain knob value changes
    gainKnob.onValueChange = [this]()
    {
        // Update the gain value in the audio processor when the knob value changes
        audioProcessor.gain.store(gainKnob.getValue());
    };
};


// Destructor
NeuralAmpAudioProcessorEditor::~NeuralAmpAudioProcessorEditor()
{
}


// This method is called to resize the components in the editor when the editor itself is resized. 
// It can be used to set the bounds of the components based on the new size of the editor.
void NeuralAmpAudioProcessorEditor::resized()
{
    // Set the bounds of the gain knob
    gainKnob.setBounds(100, 50, 200, 200);
}

// This method is called to paint the editor. It can be used to draw custom graphics on the editor.
void NeuralAmpAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill the background with a solid color
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Set the color and font for the text
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    // Draw some text in the editor
    g.drawFittedText ("Neural Amp Plugin", getLocalBounds(), juce::Justification::centred, 1);
}