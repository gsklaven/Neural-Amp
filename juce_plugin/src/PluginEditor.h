#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class NeuralAmpAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NeuralAmpAudioProcessorEditor (NeuralAmpAudioProcessor&);
    ~NeuralAmpAudioProcessorEditor() override;

    juce::Slider gainKnob;

private:
    void paint (juce::Graphics&) override;
    void resized() override;

    NeuralAmpAudioProcessor& audioProcessor; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralAmpAudioProcessorEditor)
};