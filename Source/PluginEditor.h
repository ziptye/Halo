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
#include "AnimatedParticles.h"

//==============================================================================
/**
*/
class ProjectHaloAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    ProjectHaloAudioProcessorEditor (ProjectHaloAudioProcessor&);
    ~ProjectHaloAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void createClickableAreas();
    void addImagesToArray();
    juce::Image backgroundGenerator(int pos);
    juce::String presentBankSettingsGenerator(int num, int pos);
    
    void handleCompClick(const juce::Rectangle<int>& rect);
    void handlePanelLeft(int x, int y);
    void handlePanelRight(int x, int y);
    void renderReverbComps(int page, int direction);
    void renderDelayComps(int page, int direction);
    void hideReverbComps(int page);
    void hideDelayComps(int page);
    void handleReverbPowerToggle(int y);
    void handleDelayToggle(int y);
    void handleFXPowerToggles1(int y);
    void handleFXPowerToggles2(int y);
    void handleFXAmounts1(int y);
    void handleFXAmounts2(int y);
    void generateReverbParticles();
    void generateDelayParticles();
    
    void handleTempoChange(int x, int y);
    
    void drawLabel(juce::Graphics& g, const juce::String& text, int x, int y);
    void drawText(juce::Graphics& g, juce::Colour color, float fontSize, const juce::String& text, int x, int y);
    
    void drawLEDLights(juce::Graphics& g, juce::Colour color, float x, float y, float w, float h, float cornerSize);
    
    std::vector<Component*>getDelayComps(int curPage);
    std::vector<Component*>getReverbComps(int pageNum);

private:
    
    ProjectHaloAudioProcessor& audioProcessor;
    
    juce::Image background;
    
    AnimatedKnob animatedKnob1;
    
    HaloSliders mainDryWetSlider {juce::Colours::white, juce::Colours::transparentWhite, 393, 299, 213, 227, 0.0, 100.0};
    
    HaloSliders delayFeedback {juce::Colours::royalblue, 670, 200, 80, 80, 0.0, 100.0};
    HaloSliders delayLPF {juce::Colours::goldenrod, 792, 200, 80, 80, 0.0, 100.0};
    HaloSliders delayHPF {juce::Colours::limegreen, 910, 200, 80, 80, 0.0, 100.0};
    
    HaloSliders reverbRoomSize {juce::Colours::limegreen, 10, 200, 80, 80, 0.0, 100.0}; // P1
    HaloSliders reverbPreDelay {juce::Colours::skyblue, 127, 200, 80, 80, 0.0, 100.0}; // P1
    HaloSliders reverbDamping {juce::Colours::yellow, 248, 200, 80, 80, 0.0, 100.0}; // P1
    HaloSliders reverbWidth {juce::Colours::darkblue, 10, 200, 80, 80, 0.0, 100.0}; // P2
    HaloSliders reverbHPF {juce::Colours::lightblue, 127, 200, 80, 80, 0.0, 100.0}; // P2
    HaloSliders reverbLPF {juce::Colours::limegreen, 248, 200, 80, 80, 0.0, 100.0}; // P2
    
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
    
    int bpmVal = 120; // Default BPM
    
    void timerCallback() override;
    
    std::vector<std::unique_ptr<AnimatedParticles>> particles;
    juce::Rectangle<float>particleBoundsReverb;
    
    std::vector<std::unique_ptr<AnimatedParticles>> particlesDelay;
    juce::Rectangle<float>particleBoundsDelay;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectHaloAudioProcessorEditor)
};
