# Neural-Amp: AI-Powered Guitar VST Plugin 🎸🧠

Neural-Amp is a complete pipeline for creating a digital guitar amplifier/pedal simulator using Deep Learning. It captures the complex, non-linear harmonic characteristics of analog guitar hardware and recreates them in a real-time VST3/AU audio plugin.

## Project Overview 🚀

This project is divided into two main phases:
1.  **Deep Learning (Python):** Training a Recurrent Neural Network (LSTM) to learn the mapping between a clean Direct Injection (DI) guitar signal and a heavily distorted target signal.
2.  **Real-Time Plugin (C++):** Exporting the trained model weights and running them inside a JUCE-based audio plugin with zero latency using the RTNeural framework.

## Architecture & Tech Stack 🛠️

### Phase 1: Model Training (`/python_training`)
* **Libraries:** `PyTorch`, `NumPy`, `Librosa`, `Soundfile`, `Pedalboard`
* **Process:** Audio preprocessing (silence removal, chunking into 4096 samples).
    * Generating the "wet" target audio using DSP effects.
    * Training an LSTM + Linear layer architecture to minimize Mean Squared Error.
    * Exporting the trained parameters (`state_dict`) to a `.json` file for C++ integration.

### Phase 2: Audio Plugin (`/juce_plugin`) - *In Progress*
* **Frameworks:** `C++`, `JUCE`, `RTNeural`
* **Process:**
    * Building a VST3/AU plugin interface.
    * Loading the `.json` weights into an RTNeural inference engine.
    * Processing real-time audio buffers through the neural network block frame-by-frame.

## How to Run the Python Pipeline ⚙️

1.  Install dependencies: `pip install torch numpy librosa soundfile pedalboard`
2.  Place your clean guitar track (e.g., `13_ElecGtr03DI.wav`) in the root directory.
3.  Run the pipeline: `python main.py`
4.  The script will output the processed audio (`dataset_dry.wav`, `dataset_wet.wav`), the predicted audio, and the crucial `model_weights.json` file.
