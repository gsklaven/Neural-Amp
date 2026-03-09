#pragma once

#include <JuceHeader.h>
#include <RTNeural/RTNeural.h>

class NeuralAmpAudioProcessor : public juce::AudioProcessor
{
public:
    NeuralAmpAudioProcessor();
    ~NeuralAmpAudioProcessor() override;

    // JUCE gives the sound for processing
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
private:
    RTNeural::ModelT<float, 1, 1,
        RTNeural::LSTMLayerT<float, 1, 32>,
        RTNeural::DenseT<float, 32, 1>> model;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralAmpAudioProcessor)
};