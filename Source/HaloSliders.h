/*
  ==============================================================================

    VerbSlider.h
    Created: 18 Apr 2024 2:54:05pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once

class HaloSliders : public juce::Slider
{
public:
    HaloSliders(juce::Colour fillColor, int x, int y, int w, int h, double minVal, double maxVal)
    {
        setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, fillColor);
        setTextBoxStyle(NoTextBox, true, 0, 0);
        setBounds(x, y, w, h);
        setRange(minVal, maxVal);
        setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    }
    HaloSliders(juce::Colour fillColor,juce::Colour outlineColor, int x, int y, int w, int h, double minVal, double maxVal)
    {
        setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, fillColor);
        setColour(juce::Slider::rotarySliderOutlineColourId, outlineColor);
        setTextBoxStyle(NoTextBox, true, 0, 0);
        setBounds(x, y, w, h);
        setRange(minVal, maxVal);
        setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    }
    ~HaloSliders() override {}
private:
    
};
