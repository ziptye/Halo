/*
  ==============================================================================

    AnimatedParticles.h
    Created: 15 May 2024 12:34:17pm
    Author:  Zachary Pennington

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

class AnimatedParticles
{
public:
    AnimatedParticles(float startX, float startY, const juce::Rectangle<float>& bounds)
        : position(startX, startY),
          velocity(juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f,
                   juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f),
          size(juce::Random::getSystemRandom().nextFloat() * 5.0f + 5.0f),
          color(juce::Colour::fromHSV(
                 juce::Random::getSystemRandom().nextFloat(),
                 0.8f,
                 0.8f,
                 1.0f)),
          boundingRect(bounds)
    {}

    void update()
    {
        position += velocity;

        // Defines the "bounce off the wall" bounds of rectangle
        if (position.x < boundingRect.getX() || position.x > boundingRect.getRight() - size)
            velocity.x = -velocity.x;
        if (position.y < boundingRect.getY() || position.y > boundingRect.getBottom() - size)
            velocity.y = -velocity.y;
    }

    void draw(juce::Graphics& g)
    {
        g.setColour(color);
        g.fillEllipse(position.x, position.y, size, size);
    }

private:
    juce::Point<float> position;
    juce::Point<float> velocity;
    float size;
    juce::Colour color;
    juce::Rectangle<float> boundingRect;
};
