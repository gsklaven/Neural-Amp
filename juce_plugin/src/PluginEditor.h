#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class NeuralAmpAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NeuralAmpAudioProcessorEditor (NeuralAmpAudioProcessor&);
    ~NeuralAmpAudioProcessorEditor() override;

private:
    void paint (juce::Graphics&) override;
    void resized() override;

    NeuralAmpAudioProcessor& audioProcessor; 
    juce::Slider gainKnob;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralAmpAudioProcessorEditor)
};