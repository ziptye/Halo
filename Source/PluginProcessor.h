/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Visualizer.h"

//==============================================================================
/**
*/
class ProjectHaloAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ProjectHaloAudioProcessor();
    ~ProjectHaloAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts;
    juce::dsp::StateVariableTPTFilter<float> verbHPF, verbLPF, delayHPF, delayLPF;
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;
    Visualizer& getVisualizer() {return visualizer;}
    
    bool getReverbState() {return reverbState;}
    bool getDelayState() {return delayState;}
    
    void setReverbState(bool rState);
    void setDelayState(bool dState);
    
    bool getDistortionState() {return distortionState;}
    bool getShifterState() {return shifterState;}
    bool getCozyModeState() {return cozyModeState;}
    bool getSickOModeState() {return sickoModeState;}
    
    void setDistortionState(bool distState);
    void setShifterState(bool shiftState);
    void setCozyModeState(bool cozyState);
    void setSickOModeState(bool sickState);
    
private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    void reset() override;
    juce::dsp::DelayLine<float> verbPreDelay;
    Visualizer visualizer;
    
    bool reverbState = false;
    bool delayState = false;
    bool distortionState = false;
    bool shifterState = false;
    bool cozyModeState = false;
    bool sickoModeState = false;
    
    static constexpr int maxDelayTime = 500;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectHaloAudioProcessor)
};
