#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralAmpAudioProcessorEditor::NeuralAmpAudioProcessorEditor (NeuralAmpAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);

    addAndMakeVisible(gainKnob);
    gainKnob.setSliderStyle(juce::Slider::Rotary);
    gainKnob.setRange(0.0, 2.0);
    gainKnob.setValue(audioProcessor.gain.load(), juce::sendNotification);

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
    gainKnob.setBounds(100, 80, 200, 150);
}

void NeuralAmpAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   
    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    g.drawFittedText ("Neural Amp Plugin", 0, 20, getWidth(), 40, juce::Justification::centred, 1);
}