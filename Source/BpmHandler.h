/*
  ==============================================================================

    BpmHandler.h
    Created: 3 Aug 2024 6:54:58am
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class BpmHandler : public juce::Timer
{
public:
    BpmHandler(ProjectHaloAudioProcessor& processor) : audioProcessor(processor) {}

    void startBpmChange(bool increase)
    {
        isIncreasing = increase;
        startTimer(100); // Adjust the interval as needed
    }

    void stopBpmChange()
    {
        stopTimer();
    }

    void timerCallback() override
    {
        if (isIncreasing)
        {
            if (audioProcessor.bpmVal < 300)
                audioProcessor.bpmVal += 1;
        }
        else
        {
            if (audioProcessor.bpmVal > 0)
                audioProcessor.bpmVal -= 1;
        }
    }

private:
    ProjectHaloAudioProcessor& audioProcessor;
    bool isIncreasing;
};
