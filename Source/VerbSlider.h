/*
  ==============================================================================

    VerbSlider.h
    Created: 18 Apr 2024 2:54:05pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once

class VerbSlider : public juce::Slider
{
public:
    VerbSlider(juce::Colour thumbColor, juce::Colour fillColor, int x, int y, int w, int h)
    {
        setColour(juce::Slider::ColourIds::thumbColourId, thumbColor);
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, fillColor);
        setTextBoxStyle(NoTextBox, true, 0, 0);
        setBounds(x, y, w, h);
        setRange(0.0, 100.0); // TODO: need to also capture this in the arguments to this constructor. See linear.
        
        setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    }
    ~VerbSlider() override {}
private:
    
};
