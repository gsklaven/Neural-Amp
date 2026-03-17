# 🎸 Neural Amp

An AI-powered Guitar Amplifier plugin (VST3 / Standalone) built with C++, the JUCE Framework, and RTNeural. This project clones the sound of a real guitar amplifier using Deep Learning models trained in Python.

![Neural Amp UI](assets/ui_screenshot.png) 

## ✨ Features
* **AI Amp Cloning:** Uses Neural Networks (LSTM/Dense) to recreate the non-linear distortion of real guitar amplifiers.
* **Low Latency:** Optimized for real-time audio processing using the `RTNeural` inferencing engine.
* **Formats:** Available as a Standalone application and a VST3 plugin for DAWs (Reaper, Cubase, Ableton, etc.).
* **Train Your Own:** Includes Python scripts to train your own models using direct-in (DI) and re-amped guitar signals.


## 🛠️ How to Build from Source

### Prerequisites
* CMake (v3.22 or higher)
* Visual Studio 2022 / 2019 (with Desktop Development with C++)
* Git

### Build Instructions
Clone the repository and build using CMake:

```bash
git clone [https://github.com/YOUR_USERNAME/Neural-Amp.git](https://github.com/YOUR_USERNAME/Neural-Amp.git)
cd Neural-Amp
cd juce_plugin

# Generate the Visual Studio project files
cmake -B build -G "Visual Studio 17 2022" -A x64

# Build the plugin
cmake --build build --config Release


```

*The compiled binaries (`.exe` and `.vst3`) will be located in `juce_plugin/build/NeuralAmp_artefacts/Release/`.*

---

## 🧠 How to Run the Python Pipeline (Train your own Amp)

1.  Install dependencies: 
    ```bash
    pip install torch numpy librosa soundfile pedalboard
    ```
2.  Place your clean guitar track (e.g., `13_ElecGtr03DI.wav`) in the root directory.
3.  Navigate to the Python folder and run the pipeline: 
    ```bash
    cd python_training
    python main.py
    ```
4.  The script will output the processed audio (`dataset_dry.wav`, `dataset_wet.wav`), the predicted audio, and the crucial `model_weights.json` file.
5.  Re-build the C++ project to embed your newly trained amp!