#include <RTNeural/RTNeural.h>

class NeuralAmpAudioProcessor  : public juce::AudioProcessor
{
public:
    // ... διάφορες default συναρτήσεις του JUCE ...

private:
    // Εδώ φτιάχνουμε το "αντίγραφο" του PyTorch μοντέλου μας στη C++
    RTNeural::ModelT<float, 1, 1,
        RTNeural::LSTMLayerT<float, 1, 32>,
        RTNeural::DenseT<float, 32, 1>> model;
};