#pragma once

#include <JuceHeader.h>
#include <RTNeural/RTNeural.h>

class NeuralAmpAudioProcessor : public juce::AudioProcessor
{
public:
    NeuralAmpAudioProcessor();
    ~NeuralAmpAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    std::atomic<float> gain { 1.0f };
    
private:
    std::unique_ptr<RTNeural::Model<float>> model0;
    std::unique_ptr<RTNeural::Model<float>> model1;

    bool modelLoaded = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralAmpAudioProcessor)
};