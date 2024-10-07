/*
  ==============================================================================

    Visualizer.h
    Created: 22 Jun 2023 11:52:12am
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once


class Visualizer : public juce::AudioVisualiserComponent
{
public:
    Visualizer() : AudioVisualiserComponent(2)
    {
        setBufferSize(512);
        setSamplesPerBlock(256);
        setColours(juce::Colours::black, juce::Colours::white);
    }
};
