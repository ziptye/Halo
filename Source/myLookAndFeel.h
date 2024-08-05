/*
  ==============================================================================

    myLookAndFeel.h
    Created: 5 Aug 2024 11:24:35am
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class myLookAndFeelV2 : public juce::LookAndFeel_V4
{
public:
    myLookAndFeelV2();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

private:
    juce::Image img2;
};
