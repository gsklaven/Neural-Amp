cmake_minimum_required(VERSION 3.22.0)

project(NeuralAmp)

include(FetchContent)

FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE
    GIT_TAG v8.0.0
    )

FetchContent_MakeAvailable(juce)

FetchContent_Declare(
    RTNeural
    GIT_REPOSITORY https://github.com/jatinchowdhury18/RTNeural.git
    GIT_TAG  main
    )
    
FetchContent_MakeAvailable(RTNeural)

juce_add_plugin(NeuralAmp
    COMPANY_NAME "Giorgos"
    FORMATS VST3 AU Standalone
    PLUGIN_MANUFACTURER_CODE "Gior" 
    PLUGIN_CODE "Namp"              
)

juce_add_binary_data(NeuralAmpData
    SOURCES
        ../python_training/model_weights.json
)

target_sources(NeuralAmp PRIVATE
    ./src/PluginProcessor.cpp
    ./src/PluginEditor.cpp
    ./src/PluginEditor.h
    ./src/PluginProcessor.h
)

target_compile_features(NeuralAmp PRIVATE cxx_std_20)
target_link_libraries(NeuralAmp PRIVATE juce::juce_core juce::juce_audio_processors RTNeural::RTNeural NeuralAmpData)