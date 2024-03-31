/*
  ==============================================================================

    AnimatedKnob.h
    Created: 19 Mar 2024 11:16:19am
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once
class AnimatedKnob   : public juce::AnimatedAppComponent
{
public:
    AnimatedKnob()
    {
        setFramesPerSecond(60);
    }

    void update() override {}

    void paint(juce::Graphics& g) override
    {
        // Fill background of comp.
        g.fillAll(juce::Colour(160, 160, 160));

        // Calculate the radius of the circle
        float radius = juce::jmin(getWidth(), getHeight()) * 0.5f;

        // Calculate the center of the circle
        juce::Point<float> center(getWidth() * 0.5f, getHeight() * 0.5f);

        
        juce::Path circlePath;
        circlePath.addEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f);

        // Apply rounding to the corners by drawing a rounded rectangle
        float cornerSize = 50.0f;
        juce::Path roundedRectPath;
        roundedRectPath.addRoundedRectangle(getLocalBounds().toFloat(), cornerSize);
        g.fillPath(roundedRectPath);

    
        circlePath = circlePath.createPathWithRoundedCorners(cornerSize);
        g.reduceClipRegion(circlePath);

        // Draw the spinning path within the circular clipping region
        drawSpinningPath(g, center, radius, 3);
    }

        

    void resized() override {}

private:
    void drawSpinningPath(juce::Graphics& g, const juce::Point<float>& center, float radius, int depth)
    {
        if (depth <= 0) // Base case to stop recursion
            return;

        
        auto numberOfDots = 10;
        juce::Path spinePath;

        for (auto i = 0; i < numberOfDots; ++i)
        {
            juce::Point<float> p(center.x + radius * std::sin((float)getFrameCounter() * 0.04f + (float)i * 0.12f),
                                  center.y + radius * std::cos((float)getFrameCounter() * 0.04f + (float)i * 0.12f));

            if (i == 0)
                spinePath.startNewSubPath(p);
            else
                spinePath.lineTo(p);
        }

        g.setColour(juce::Colours::white);
        g.strokePath(spinePath, juce::PathStrokeType(9.0f));

        // Calculate the radius for the next circle
        float smallerRadius = radius * 0.75f;

        // Draw the next spinning path with smaller radius
        drawSpinningPath(g, center, smallerRadius, depth - 1);
    }

};
