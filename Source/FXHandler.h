/*
  ==============================================================================

    FXHandler.h
    Created: 3 Aug 2024 12:08:38pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once

class FXHandler : public juce::Timer
{
public:
    enum class Mode
    {
        Distortion,
        CozyMode,
        Shifter,
        SickOMode
    };

    FXHandler(ProjectHaloAudioProcessor& processor) : audioProcessor(processor) {}

    void startFXChange(bool increase, Mode mode)
    {
        isIncreasing = increase;
        currentMode = mode;
        startTimer(85);
    }

    void stopFXChange()
    {
        stopTimer();
    }

    void timerCallback() override
    {
        switch (currentMode)
        {
        case Mode::Distortion:
            if (isIncreasing)
            {
                if (distortionAmt < 100 && audioProcessor.getDistortionState())
                {
                    distortionAmt += 2;
                }
            }
            else
            {
                if (distortionAmt > 0 && audioProcessor.getDistortionState())
                {
                    distortionAmt -= 2;
                }
            }
            break;

        case Mode::CozyMode:
            if (isIncreasing)
            {
                if (cozyModeAmt < 100 && audioProcessor.getCozyModeState())
                {
                    cozyModeAmt += 2;
                }
            }
            else
            {
                if (cozyModeAmt > 0 && audioProcessor.getCozyModeState())
                {
                    cozyModeAmt -= 2;
                }
            }
            break;

        case Mode::Shifter:
            if (isIncreasing)
            {
                if (shifterAmt < 100 && audioProcessor.getShifterState())
                {
                    shifterAmt += 2;
                }
            }
            else
            {
                if (shifterAmt > 0 && audioProcessor.getShifterState())
                {
                    shifterAmt -= 2;
                }
            }
            break;

        case Mode::SickOMode:
            if (isIncreasing)
            {
                if (sickoModeAmt < 100 && audioProcessor.getSickOModeState())
                {
                    sickoModeAmt += 2;
                }
            }
            else
            {
                if (sickoModeAmt > 0 && audioProcessor.getSickOModeState())
                {
                    sickoModeAmt -= 2;
                }
            }
            break;
        }
    }

    int getDistortion() { return distortionAmt; }
    int getShifter() { return shifterAmt; }
    int getCozyMode() { return cozyModeAmt; }
    int getSickoMode() { return sickoModeAmt; }

private:
    ProjectHaloAudioProcessor& audioProcessor;
    bool isIncreasing;
    Mode currentMode;
    int distortionAmt = 0;
    int shifterAmt = 0;
    int cozyModeAmt = 0;
    int sickoModeAmt = 0;
};
