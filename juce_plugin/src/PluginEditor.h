#pragma once

#include <JuceHeader.h>
#include <RTNeural/RTNeural.h>

class NeuralAmpAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NeuralAmpAudioProcessorEditor (juce::AudioProcessor&);
    ~NeuralAmpAudioProcessorEditor() override;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralAmpAudioProcessorEditor)

private:
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Slider gainKnob;
};