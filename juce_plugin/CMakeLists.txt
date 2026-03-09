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

target_sources(NeuralAmp PRIVATE
    ./src/PluginProcessor.cpp
    ./src/PluginEditor.cpp
    ./src/PluginEditor.h
    ./src/PluginProcessor.h
)

target_compile_features(NeuralAmp PRIVATE cxx_std_20)