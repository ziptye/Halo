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
        auto* distortionAmt = audioProcessor.apvts.getRawParameterValue("distortionAmt");
        auto* cozyModeAmt = audioProcessor.apvts.getRawParameterValue("cozyModeAmt");
        auto* shifterAmt = audioProcessor.apvts.getRawParameterValue("shifterAmt");
        auto* sickoModeAmt = audioProcessor.apvts.getRawParameterValue("sickoModeAmt");
        
        auto updateParameter = [](std::atomic<float>* param, bool isIncreasing) 
        {
            float value = param->load();
            if (isIncreasing)
            {
                if (value < 100.0f)
                    value = std::min(100.0f, value + 2.0f);
            }
            else
            {
                if (value > 0.0f)
                    value = std::max(0.0f, value - 2.0f);
            }
            param->store(value);
        };

        switch (currentMode)
            {
            case Mode::Distortion:
                if (audioProcessor.getDistortionState())
                    updateParameter(distortionAmt, isIncreasing);
                break;

            case Mode::CozyMode:
                if (audioProcessor.getCozyModeState())
                    updateParameter(cozyModeAmt, isIncreasing);
                break;

            case Mode::Shifter:
                if (audioProcessor.getShifterState())
                    updateParameter(shifterAmt, isIncreasing);
                break;

            case Mode::SickOMode:
                if (audioProcessor.getSickOModeState())
                    updateParameter(sickoModeAmt, isIncreasing);
                break;
            }
    }
private:
    ProjectHaloAudioProcessor& audioProcessor;
    bool isIncreasing;
    Mode currentMode;
};
