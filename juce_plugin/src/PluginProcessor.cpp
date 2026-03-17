#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <nlohmann/json.hpp>

NeuralAmpAudioProcessor::NeuralAmpAudioProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    // Initialize the AudioProcessor with stereo input and output buses
}

NeuralAmpAudioProcessor::~NeuralAmpAudioProcessor() {}

const juce::String NeuralAmpAudioProcessor::getName() const { return "NeuralAmp"; }
bool NeuralAmpAudioProcessor::acceptsMidi() const { return false; }
bool NeuralAmpAudioProcessor::producesMidi() const { return false; }
bool NeuralAmpAudioProcessor::isMidiEffect() const { return false; }
double NeuralAmpAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int NeuralAmpAudioProcessor::getNumPrograms() { return 1; }
int NeuralAmpAudioProcessor::getCurrentProgram() { return 0; }
void NeuralAmpAudioProcessor::setCurrentProgram (int index) {}
const juce::String NeuralAmpAudioProcessor::getProgramName (int index) { return {}; }
void NeuralAmpAudioProcessor::changeProgramName (int index, const juce::String& newName) {}

//==============================================================================
void NeuralAmpAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool NeuralAmpAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* NeuralAmpAudioProcessor::createEditor()
{
    return new NeuralAmpAudioProcessorEditor (*this);
}

//==============================================================================
void NeuralAmpAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Store parameters in the memory block.
    // Here we're using a simple binary stream to save the gain value.
    juce::MemoryOutputStream stream(destData, true);
    stream.writeFloat(gain.load());
}

void NeuralAmpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore parameters from the memory block.
    // Make sure to read them back in the exact same order they were written.
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    float savedGain = stream.readFloat();
    gain.store(savedGain);

    // Update the editor UI if it's currently active on the screen
    if (auto* editor = dynamic_cast<NeuralAmpAudioProcessorEditor*>(getActiveEditor()))
        editor->gainKnob.setValue(savedGain, juce::dontSendNotification);
}

void NeuralAmpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Locate the model weights file in the same directory as the plugin binary
    auto pluginDir = juce::File::getSpecialLocation(
        juce::File::currentApplicationFile).getParentDirectory();
    auto jsonFile = pluginDir.getChildFile("model_weights.json");

    if (!jsonFile.existsAsFile())
    {
        DBG("ERROR: model_weights.json not found at: " + jsonFile.getFullPathName());
        modelLoaded = false;
        return;
    }

    try
    {
        // Load and parse the JSON model weights
        auto jsonString = jsonFile.loadFileAsString().toStdString();
        nlohmann::json modelJson = nlohmann::json::parse(jsonString);

        // Parse the RTNeural models from the JSON data
        // We create two instances, one for each channel (stereo)
        model0 = RTNeural::json_parser::parseJson<float>(modelJson);
        model1 = RTNeural::json_parser::parseJson<float>(modelJson);

        // Reset the models to their initial state
        model0->reset();
        model1->reset();

        modelLoaded = true;
        DBG("Model loaded successfully.");
    }
    catch (const std::exception& e)
    {
        DBG("ERROR loading model: " + juce::String(e.what()));
        modelLoaded = false;
    }
}

void NeuralAmpAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages)
{
    // If the model hasn't loaded successfully, silence the buffer to avoid noise
    if (!modelLoaded)
    {
        buffer.clear();
        return;
    }

    juce::ScopedNoDenormals noDenormals;
    const float currentGain = gain.load();

    // Iterate through each channel
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        // Process each sample in the buffer
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            
            // Apply the neural network model. Use model0 for the left channel
            // and model1 for the right channel (or any other channels)
            float outputSample = (channel == 0)
                ? model0->forward(&inputSample)
                : model1->forward(&inputSample);

            // Apply gain and write to output
            channelData[sample] = outputSample * currentGain;
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeuralAmpAudioProcessor();
}