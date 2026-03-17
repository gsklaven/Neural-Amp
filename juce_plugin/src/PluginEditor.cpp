#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralAmpAudioProcessorEditor::NeuralAmpAudioProcessorEditor (NeuralAmpAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set the window size
    setSize (400, 300);

    // Configure the gain slider
    addAndMakeVisible(gainKnob);
    gainKnob.setSliderStyle(juce::Slider::Rotary);
    gainKnob.setRange(0.0, 2.0); // Simple gain range from 0 (silence) to 2.0 (+6dB)
    gainKnob.setValue(audioProcessor.gain.load(), juce::sendNotification);

    // Update the processor parameter when the slider moves
    gainKnob.onValueChange = [this]()
    {
        audioProcessor.gain.store((float)gainKnob.getValue());
    };
}

NeuralAmpAudioProcessorEditor::~NeuralAmpAudioProcessorEditor()
{
}

void NeuralAmpAudioProcessorEditor::resized()
{
    // Position the slider in the center-ish area
    gainKnob.setBounds(100, 80, 200, 150);
}

void NeuralAmpAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   
    
    // Draw title text
    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    g.drawFittedText ("Neural Amp Plugin", 0, 20, getWidth(), 40, juce::Justification::centred, 1);
}