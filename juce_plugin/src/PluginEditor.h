#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class NeuralAmpAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NeuralAmpAudioProcessorEditor (NeuralAmpAudioProcessor&);
    ~NeuralAmpAudioProcessorEditor() override;

    // Public slider component so we can attach listeners if needed, 
    // or access it easily.
    juce::Slider gainKnob;

private:
    void paint (juce::Graphics&) override;
    void resized() override;

    // Reference to the audio processor to access parameters
    NeuralAmpAudioProcessor& audioProcessor; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralAmpAudioProcessorEditor)
};