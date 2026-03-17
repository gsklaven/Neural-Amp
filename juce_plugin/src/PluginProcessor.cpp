#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <nlohmann/json.hpp>

NeuralAmpAudioProcessor::NeuralAmpAudioProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
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
void NeuralAmpAudioProcessor::releaseResources() {}
bool NeuralAmpAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* NeuralAmpAudioProcessor::createEditor()
{
    return new NeuralAmpAudioProcessorEditor (*this);
}

void NeuralAmpAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    stream.writeFloat(gain.load());
}

void NeuralAmpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    float savedGain = stream.readFloat();
    gain.store(savedGain);

    if (auto* editor = dynamic_cast<NeuralAmpAudioProcessorEditor*>(getActiveEditor()))
        editor->gainKnob.setValue(savedGain, juce::dontSendNotification);
}

void NeuralAmpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
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
        auto jsonString = jsonFile.loadFileAsString().toStdString();
        nlohmann::json modelJson = nlohmann::json::parse(jsonString);

        model0 = RTNeural::json_parser::parseJson<float>(modelJson);
        model1 = RTNeural::json_parser::parseJson<float>(modelJson);

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
    if (!modelLoaded)
    {
        buffer.clear();
        return;
    }

    juce::ScopedNoDenormals noDenormals;
    const float currentGain = gain.load();

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            float outputSample = (channel == 0)
                ? model0->forward(&inputSample)
                : model1->forward(&inputSample);

            channelData[sample] = outputSample * currentGain;
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeuralAmpAudioProcessor();
}