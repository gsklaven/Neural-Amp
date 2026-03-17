#include <sstream>
#include "PluginProcessor.h"


// Cunstructor
NeuralAmpAudioProcessor::NeuralAmpAudioProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    // Load the model weights from the binary data
    std::string jsonString(BinaryData::model_weights_json, BinaryData::model_weights_jsonSize);

    // Create a string stream from the JSON string
    std::stringstream jsonStream0(jsonString);

    // Read the stream and parse it to the model
    model0.parseJson(jsonStream0);

    // Same for the second model
    std::stringstream jsonStream1(jsonString);
    model1.parseJson(jsonStream1);
}


// Destructor
NeuralAmpAudioProcessor::~NeuralAmpAudioProcessor()
{
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
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
{
    // Take a pointer to the audio data for the current channel
    auto* channelData = buffer.getWritePointer(channel);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float outputSample;

        // Read the input sample
        float inputSample = channelData[sample];

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