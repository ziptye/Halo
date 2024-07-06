/*
  ==============================================================================

    HaloDelayButtons.h
    Created: 19 Apr 2024 2:03:41pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class HaloDelayButtons : public juce::TextButton
{
public:
    HaloDelayButtons(const juce::String text, int x, int y, int w, int h)
    {
        setClickingTogglesState(true);
        setButtonText(text);
        setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
        setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
        setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
        setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
        setBounds(x, y, w, h);
        buttons.add(this);
        onClick = [this] { handleButtonClick(); };
    }
    ~HaloDelayButtons() override {
        buttons.removeFirstMatchingValue(this);
    }
    
private:
    static juce::Array<HaloDelayButtons*> buttons;
    
    void handleButtonClick()
    {
        bool isCurrentlyOn = getToggleState();
        
        if (isCurrentlyOn)
        {
            for (auto* button : buttons)
            {
                if (button != this)
                {
                    button->setToggleState(false, juce::NotificationType::dontSendNotification);
                    button->setClickingTogglesState(true);
                }
            }
            setClickingTogglesState(false);  // Allows button be to deselected
        }
        else
        {
            for (auto* button : buttons)
            {
                button->setClickingTogglesState(true);
            }
        }
        
    }
};
