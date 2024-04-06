/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AnimatedKnob.h"

//==============================================================================
/**
*/
class ProjectHaloAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ProjectHaloAudioProcessorEditor (ProjectHaloAudioProcessor&);
    ~ProjectHaloAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    
    void createPanelNavArrows();
    void addImagesToArray();
    juce::Image backgroundGenerator(int pos);

private:
    
    ProjectHaloAudioProcessor& audioProcessor;
    
    juce::Image background;
    
    AnimatedKnob animatedKnob1;
    
    juce::Array<juce::Rectangle<int>> rectangleArr;
    
    juce::Array<juce::Image> imagesArray;
    
    bool reverbState = false;
    bool delayState = false;
    
    bool distortionState = false;
    bool shifterState = false;
    bool cozyModeState = false;
    bool sickoModeState = false;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectHaloAudioProcessorEditor)
};
