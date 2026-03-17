cmake_minimum_required(VERSION 3.22.0)

project(NeuralAmp)

include(FetchContent)

FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE
    GIT_TAG 8.0.12
)
FetchContent_MakeAvailable(juce)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    RTNeural
    GIT_REPOSITORY https://github.com/jatinchowdhury18/RTNeural.git
    GIT_TAG main
)
FetchContent_GetProperties(RTNeural)
if(NOT rtneural_POPULATED)
    FetchContent_Populate(RTNeural)
    add_subdirectory(${rtneural_SOURCE_DIR} ${rtneural_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
    
FetchContent_MakeAvailable(RTNeural)

juce_add_plugin(NeuralAmp
    VERSION 1.0.0
    COMPANY_NAME "Giorgos"
    FORMATS VST3 AU Standalone
    PLUGIN_MANUFACTURER_CODE "Gior" 
    PLUGIN_CODE "Namp"  
    VST3_AUTO_REPLACE_VST2 TRUE
)

juce_generate_juce_header(NeuralAmp)

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

target_link_libraries(NeuralAmp PRIVATE 
    juce::juce_core 
    juce::juce_audio_processors 
    juce::juce_gui_basics      
    juce::juce_audio_utils     
    RTNeural 
    NeuralAmpData
    nlohmann_json::nlohmann_json
)

target_compile_definitions(NeuralAmp PUBLIC JUCE_VST3_CAN_REPLACE_VST2=0)