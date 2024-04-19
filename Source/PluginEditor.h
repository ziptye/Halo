/*
  ==============================================================================

 PluginEditor.h
 Author:  Zachary Pennington
 Project: Halo
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AnimatedKnob.h"
#include "HaloSliders.h"
#include "HaloDelayButtons.h"

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
    void configTextButtons(juce::TextButton& button, const juce::String& text);
    void configPresetBanks(juce::StringArray &strArray, const juce::String &text);
    void createClickableAreas();
    void addImagesToArray();
    juce::Image backgroundGenerator(int pos);
    juce::String presentBankSettingsGenerator(int num, int pos);
    
    std::vector<Component*>getDelayComps(int curPage);
    std::vector<Component*>getReverbComps(int pageNum);

private:
    
    ProjectHaloAudioProcessor& audioProcessor;
    
    juce::Image background;
    
    AnimatedKnob animatedKnob1;
    
    juce::Slider mainDryWetSlider;
    
    HaloSliders delayFeedback {juce::Colours::white, juce::Colours::royalblue, 670, 200, 80, 80, 0.0, 100.0};
    HaloSliders delayLPF {juce::Colours::white, juce::Colours::goldenrod, 792, 200, 80, 80, 0.0, 100.0};
    HaloSliders delayHPF {juce::Colours::white, juce::Colours::limegreen, 910, 200, 80, 80, 0.0, 100.0};
    
    HaloSliders reverbRoomSize {juce::Colours::white, juce::Colours::limegreen, 10, 200, 80, 80, 0.0, 100.0}; // P1
    HaloSliders reverbPreDelay {juce::Colours::white, juce::Colours::skyblue, 127, 200, 80, 80, 0.0, 100.0}; // P1
    HaloSliders reverbDamping {juce::Colours::white, juce::Colours::yellow, 248, 200, 80, 80, 0.0, 100.0}; // P1
    HaloSliders reverbWidth {juce::Colours::white, juce::Colours::limegreen, 10, 200, 80, 80, 0.0, 100.0}; // P2
    HaloSliders reverbHPF {juce::Colours::white, juce::Colours::limegreen, 127, 200, 80, 80, 0.0, 100.0}; // P2
    HaloSliders reverbLPF {juce::Colours::white, juce::Colours::limegreen, 248, 200, 80, 80, 0.0, 100.0}; // P2
    
    juce::Array<juce::Rectangle<int>> rectangleArr;
    
    juce::Array<juce::Image> imagesArray;
    
    juce::StringArray presentBank1Settings;
    juce::StringArray presentBank2Settings;
    
    HaloDelayButtons sixtyFourthNote {"1/64", 672, 205, 75, 30};
    HaloDelayButtons thirtySecondNote {"1/32", 752, 205, 75, 30};
    HaloDelayButtons sixteenthNote {"1/16", 832, 205, 75, 30};
    HaloDelayButtons eighthNote {"1/8", 912, 205, 75, 30};
    HaloDelayButtons quarterNote {"1/4", 712, 245, 75, 30};
    HaloDelayButtons halfNote {"1/2", 793, 245, 75, 30};
    HaloDelayButtons wholeNote {"1/1", 873, 245, 75, 30};
    
    bool reverbState = false;
    bool delayState = false;
    
    bool distortionState = false;
    bool shifterState = false;
    bool cozyModeState = false;
    bool sickoModeState = false;
    
    int currentIndexPresetBank1 = 0;
    int currentIndexPresetBank2 = 0;
    
    unsigned short currentVerbIndex = 0;
    unsigned short currentDelayIndex = 0;
    
    int distortionAmt = 0;
    int shifterAmt = 0;
    int cozyModeAmt = 0;
    int sickoModeAmt = 0;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectHaloAudioProcessorEditor)
};
