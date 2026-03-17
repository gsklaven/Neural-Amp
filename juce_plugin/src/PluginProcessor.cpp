#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <nlohmann/json.hpp>

// Constructor
NeuralAmpAudioProcessor::NeuralAmpAudioProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    // Load the model weights from the binary data
    std::string jsonString(BinaryData::model_weights_json, BinaryData::model_weights_jsonSize);
    auto parsedJson = nlohmann::json::parse(jsonString);

    // Read the stream and parse it to the model
    model0.parseJson(parsedJson);

    // Same for the second model
    model1.parseJson(parsedJson);
}

// Destructor
NeuralAmpAudioProcessor::~NeuralAmpAudioProcessor()
{
}

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
void NeuralAmpAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void NeuralAmpAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}
void NeuralAmpAudioProcessor::releaseResources() {}

bool NeuralAmpAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* NeuralAmpAudioProcessor::createEditor()
{
    return new NeuralAmpAudioProcessorEditor (*this);
}

// This method is called before the audio processing starts. It can be used to initialize any resources or settings needed for processing.
void NeuralAmpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    model0.reset();
    model1.reset();
}

// This method is called to process the audio buffer.
void NeuralAmpAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        // Take a pointer to the audio data for the current channel
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Read the input sample
            float inputSample = channelData[sample];
            float outputSample = 0.0f;

            // Process the sample through the model
            if (channel == 0) // Left channel
            {
                outputSample = model0.forward(&inputSample);
            }
            else // Right channel
            {
                outputSample = model1.forward(&inputSample);
            }

            // Write the output sample back to the buffer
            channelData[sample] = outputSample;
            
            // Apply the gain to the output sample
            channelData[sample] *= gain.load();
        }
    }
}


// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeuralAmpAudioProcessor();
}