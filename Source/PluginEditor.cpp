/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProjectHaloAudioProcessorEditor::ProjectHaloAudioProcessorEditor (ProjectHaloAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(animatedKnob1);
    createPanelNavArrows();
    addImagesToArray();
        
    // BACKGROUND ======================================================
    
    background = backgroundGenerator(0);
    
    setSize(1000, 526);

}

ProjectHaloAudioProcessorEditor::~ProjectHaloAudioProcessorEditor()
{
}

//==============================================================================
void ProjectHaloAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    g.drawImageAt(background, 0, 0);
    
}

void ProjectHaloAudioProcessorEditor::resized()
{
    animatedKnob1.setBounds(783, 400, 100, 100);
}

void ProjectHaloAudioProcessorEditor::addImagesToArray()
{
    // Define the images:
    auto initBackground = juce::ImageCache::getFromMemory(BinaryData::PH_InitBG_png, BinaryData::PH_InitBG_pngSize);
    
    auto bg1 = juce::ImageCache::getFromMemory(BinaryData::PH_P1On_P2On_P3On_png, BinaryData::PH_P1On_P2On_P3On_pngSize);
    
    auto bg2 = juce::ImageCache::getFromMemory(BinaryData::PH_P1Off_P2On_P3On_png, BinaryData::PH_P1Off_P2On_P3On_pngSize);
    auto bg3 = juce::ImageCache::getFromMemory(BinaryData::PH_P1On_P2Off_P3On_png, BinaryData::PH_P1On_P2Off_P3On_pngSize);
    auto bg4 = juce::ImageCache::getFromMemory(BinaryData::PH_P1On_P2On_P3Off_png, BinaryData::PH_P1On_P2On_P3Off_pngSize);
    
    auto bg5 = juce::ImageCache::getFromMemory(BinaryData::PH_P1Off_P2Off_P3On_png, BinaryData::PH_P1Off_P2Off_P3On_pngSize);
    auto bg6 = juce::ImageCache::getFromMemory(BinaryData::PH_P1Off_P2On_P3Off_png, BinaryData::PH_P1Off_P2On_P3Off_pngSize);
    auto bg7 = juce::ImageCache::getFromMemory(BinaryData::PH_P1On_P2Off_P3Off_png, BinaryData::PH_P1On_P2Off_P3Off_pngSize);
    
    
    // Add images to array
    imagesArray.add(initBackground);
    imagesArray.add(bg1);
    imagesArray.add(bg2);
    imagesArray.add(bg3);
    imagesArray.add(bg4);
    imagesArray.add(bg5);
    imagesArray.add(bg6);
    imagesArray.add(bg7);
    
}

juce::Image ProjectHaloAudioProcessorEditor::backgroundGenerator(int pos)
{
    
    if (!imagesArray.isEmpty()) {
        return imagesArray[pos];
    }
    else
    {
        std::cout << "EMPTY ARRAY";
        return imagesArray[0];
    }
    
    
}

void ProjectHaloAudioProcessorEditor::createPanelNavArrows()
{
    juce::Rectangle<int> panelLeft1(10, 150, 25, 25); // x, y, width, height
    juce::Rectangle<int> panelRight1(304, 150, 25, 25);
    
    juce::Rectangle<int> panelLeft3(670, 150, 25, 25);
    juce::Rectangle<int> panelRight3(965, 150, 25, 25);
    
    juce::Rectangle<int> presetBank1L(10, 299, 25, 25);
    juce::Rectangle<int> presetBank1R(304, 299, 25, 25);
    juce::Rectangle<int> presetBank2L(670, 299, 25, 25);
    juce::Rectangle<int> presetBank2R(965, 299, 25, 25);
    
    juce::Rectangle<int> powerOn1(110, 103, 116, 120);
    juce::Rectangle<int> powerOn2(442, 90, 116, 120);
    juce::Rectangle<int> powerOn3(776, 103, 116, 120);

    
    rectangleArr.add(panelLeft1);
    rectangleArr.add(panelRight1);
    rectangleArr.add(panelLeft3);
    rectangleArr.add(panelRight3);
    
    rectangleArr.add(presetBank1L);
    rectangleArr.add(presetBank1R);
    rectangleArr.add(presetBank2L);
    rectangleArr.add(presetBank2R);
    
    rectangleArr.add(powerOn1);
    rectangleArr.add(powerOn2);
    rectangleArr.add(powerOn3);
}

void ProjectHaloAudioProcessorEditor::mouseDown(const juce::MouseEvent &event)
{
    juce::Point<int> clickPosition = event.getPosition();
    
    for (const auto& rect : rectangleArr)
    {
        // Checks if the current mouse click position matches any (x,y) coordinates in rectangleArr
        if (rect.contains(clickPosition))
        {
            // Extract x, y of the rectangle
            int x = rect.getX();
            int y = rect.getY();
            
            switch (x)
            {
                case 10:
                    if (y == 150)
                    {
                        std::cout << "LEFT PANEL 1";
                    }
                    else if (y == 299)
                    {
                        std::cout << "PRESENT BANK 1L";
                    }
                    break;
                case 304:
                    if (y == 150)
                    {
                        std::cout << "RIGHT PANEL 1";
                    }
                    else if (y == 299)
                    {
                        std::cout << "PRESENT BANK 1R";
                    }
                    break;
                case 670:
                    if (y == 150)
                    {
                        std::cout << "LEFT PANEL 3";
                    }
                    else if (y == 299)
                    {
                        std::cout << "PRESET BANK 2L";
                    }
                    break;
                case 965:
                    if (y == 150)
                    {
                        std::cout << "RIGHT PANEL 3";
                    }
                    else if (y == 299)
                    {
                        std::cout << "PRESET BANK 2R";
                    }
                    break;
                case 110:
                    if (y == 103)
                    {
                        if (!panel1State && !panel2State && !panel3State)
                        {
                            background = backgroundGenerator(7);
                            panel1State = true;
                            repaint();
                        }
                        else if (!panel1State && panel2State && panel3State)
                        {
                            background = backgroundGenerator(1);
                            panel1State = true;
                            repaint();
                        }
                        else if (!panel1State && !panel2State && panel3State)
                        {
                            background = backgroundGenerator(3);
                            panel1State = true;
                            repaint();
                        }
                        else if (!panel1State && panel2State && !panel3State)
                        {
                            background = backgroundGenerator(4);
                            panel1State = true;
                            repaint();
                        }
                        else if (panel1State && panel2State && panel3State)
                        {
                            background = backgroundGenerator(2);
                            panel1State = false;
                            repaint();
                        }
                        else if (panel1State && !panel2State && !panel3State)
                        {
                            background = backgroundGenerator(0);
                            panel1State = false;
                            repaint();
                        }
                        else if (panel1State && !panel2State && panel3State)
                        {
                            background = backgroundGenerator(5);
                            panel1State = false;
                            repaint();
                        }
                        else if (panel1State && panel2State && !panel3State)
                        {
                            background = backgroundGenerator(6);
                            panel1State = false;
                            repaint();
                        }
                        
                    }
                    break;
                    
                case 442:
                    if (y == 90)
                    {
                        if (!panel1State && !panel2State && !panel3State)
                        {
                            background = backgroundGenerator(6);
                            panel2State = true;
                            repaint();
                        }
                        else if (panel1State && !panel2State && !panel3State)
                        {
                            background = backgroundGenerator(4);
                            panel2State = true;
                            repaint();
                        }
                        else if (panel1State && !panel2State && panel3State)
                        {
                            background = backgroundGenerator(1);
                            panel2State = true;
                            repaint();
                        }
                        else if (!panel1State && !panel2State && panel3State)
                        {
                            background = backgroundGenerator(2);
                            panel2State = true;
                            repaint();
                        }
                        else if (!panel1State && panel2State && !panel3State)
                        {
                            background = backgroundGenerator(0);
                            panel2State = false;
                            repaint();
                        }
                        else if (!panel1State && panel2State && panel3State)
                        {
                            background = backgroundGenerator(5);
                            panel2State = false;
                            repaint();
                        }
                        else if (panel1State && panel2State && !panel3State)
                        {
                            background = backgroundGenerator(7);
                            panel2State = false;
                            repaint();
                        }
                        else if (panel1State && panel2State && panel3State)
                        {
                            background = backgroundGenerator(3);
                            panel2State = false;
                            repaint();
                        }
                        
                    }
                    break;
                    
                case 776:
                    if (y == 103)
                    {
                        if (!panel1State && !panel2State && !panel3State)
                        {
                            background = backgroundGenerator(5);
                            panel3State = true;
                            repaint();
                        }
                        else if (panel1State && !panel2State && !panel3State)
                        {
                            background = backgroundGenerator(3);
                            panel3State = true;
                            repaint();
                        }
                        else if (panel1State && panel2State && !panel3State)
                        {
                            background = backgroundGenerator(1);
                            panel3State = true;
                            repaint();
                        }
                        else if (!panel1State && panel2State && !panel3State)
                        {
                            background = backgroundGenerator(2);
                            panel3State = true;
                            repaint();
                        }
                        else if (panel1State && panel2State && panel3State)
                        {
                            background = backgroundGenerator(4);
                            panel3State = false;
                            repaint();
                        }
                        else if (!panel1State && !panel2State && panel3State)
                        {
                            background = backgroundGenerator(0);
                            panel3State = false;
                            repaint();
                        }
                        else if (panel1State && !panel2State && panel3State)
                        {
                            background = backgroundGenerator(7);
                            panel3State = false;
                            repaint();
                        }
                        else if (!panel1State && panel2State && panel3State)
                        {
                            background = backgroundGenerator(6);
                            panel3State = false;
                            repaint();
                        }
                        
                    }
                    break;
                
                default:
                {
                    break;
                }
            }
            break; // Exit the loop
        }
    }
}
