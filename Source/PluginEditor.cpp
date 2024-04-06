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
//    addAndMakeVisible(animatedKnob1);
    createPanelNavArrows();
    addImagesToArray();
        
    // BACKGROUND ======================================================
    
    background = backgroundGenerator(0);
    
    setSize(1000, 525);

}

ProjectHaloAudioProcessorEditor::~ProjectHaloAudioProcessorEditor()
{
}

//==============================================================================
void ProjectHaloAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    g.drawImageAt(background, 0, 0);
    
    // LED LIGHT COORDS.
    
    // LEFT TOP ----
    if (!distortionState){
        g.setColour(juce::Colours::red);
        g.fillRoundedRectangle(467, 75, 8, 8, 4.0f);
    }
    else {
        g.setColour(juce::Colours::lime);
        g.fillRoundedRectangle(467, 30, 8, 8, 4.0f);
    }
    // RIGHT TOP ----
    if (!shifterState){
        g.setColour(juce::Colours::red);
        g.fillRoundedRectangle(628, 75, 8, 8, 4.0f);
    }
    else {
        g.setColour(juce::Colours::lime);
        g.fillRoundedRectangle(628, 30, 8, 8, 4.0f);
    }
    // LEFT BOT. ----
    if (!cozyModeState){
        g.setColour(juce::Colours::red);
        g.fillRoundedRectangle(467, 170, 8, 8, 4.0f);
    }
    else {
        g.setColour(juce::Colours::lime);
        g.fillRoundedRectangle(467, 122, 8, 8, 4.0f);
    }
    // RIGHT BOT. ----
    if (!sickoModeState){
        g.setColour(juce::Colours::red);
        g.fillRoundedRectangle(628, 170, 8, 8, 4.0f);
    }
    else {
        
        g.setColour(juce::Colours::lime);
        g.fillRoundedRectangle(628, 122, 8, 8, 4.0f);
    }
    // DIST COORDS. ----
//    g.setColour(juce::Colours::pink);
//    g.fillRect(393, 30, 50, 50);
    
    // SHIFTER COORDS. ----
//    g.setColour(juce::Colours::pink);
//    g.fillRect(553, 30, 50, 50);
    
    // COZY MODE COORDS. ----
//    g.setColour(juce::Colours::pink);
//    g.fillRect(393, 123, 50, 50);
    
    // SICKO MODE COORDS. ----
//    g.setColour(juce::Colours::pink);
//    g.fillRect(553, 123, 50, 50);
    
}

void ProjectHaloAudioProcessorEditor::resized()
{
//    animatedKnob1.setBounds(783, 400, 100, 100);
}

void ProjectHaloAudioProcessorEditor::addImagesToArray()
{
    // Define the images:
    auto initBackground = juce::ImageCache::getFromMemory(BinaryData::PH_InitBG_png, BinaryData::PH_InitBG_pngSize);
    
    auto bg1 = juce::ImageCache::getFromMemory(BinaryData::PH_RonDoff_png, BinaryData::PH_RonDoff_pngSize);
    auto bg2 = juce::ImageCache::getFromMemory(BinaryData::PH_RoffDon_png, BinaryData::PH_RoffDon_pngSize);
    auto bg3 = juce::ImageCache::getFromMemory(BinaryData::PH_RonDon_png, BinaryData::PH_RonDon_pngSize);
    
    // Add images to array
    imagesArray.add(initBackground);
    imagesArray.add(bg1);
    imagesArray.add(bg2);
    imagesArray.add(bg3);
    
}

juce::Image ProjectHaloAudioProcessorEditor::backgroundGenerator(int pos)
{
    
    return imagesArray[pos];
    
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
    juce::Rectangle<int> powerOn2(776, 103, 116, 120);
    
    juce::Rectangle<int> distortionFx(393, 30, 50, 50);
    juce::Rectangle<int> cozyModeFx(393, 123, 50, 50);
    juce::Rectangle<int> shifterFx(553, 30, 50, 50);
    juce::Rectangle<int> sickoModeFx(553, 123, 50, 50);
    
    
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
    
    rectangleArr.add(distortionFx);
    rectangleArr.add(shifterFx);
    rectangleArr.add(cozyModeFx);
    rectangleArr.add(sickoModeFx);
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
                case 393:
                    if (y == 30)
                    {
                        int i = 0;
                        
                        if (!distortionState && i == 0){
                            i++;
                            distortionState = true;
                            repaint();
                        }
                        else{
                            distortionState = false;
                            i = 0;
                            repaint();
                        }
                    }
                    else if (y == 123){
                        
                        int i = 0;
                        
                        if (!cozyModeState && i == 0){
                            i++;
                            cozyModeState = true;
                            repaint();
                        }
                        else {
                            i = 0;
                            cozyModeState = false;
                            repaint();
                        }
                    }
                    break;
                case 553:
                    if (y == 30)
                    {
                        int i = 0;
                        
                        if (!shifterState && i == 0){
                            i++;
                            shifterState = true;
                            repaint();
                        }
                        else{
                            i = 0;
                            shifterState = false;
                            repaint();
                        }
                    }
                    else if (y == 123){
                        
                        int i = 0;
                        
                        if (!sickoModeState && i == 0){
                            i++;
                            sickoModeState = true;
                            repaint();
                        }
                        else {
                            i = 0;
                            sickoModeState = false;
                            repaint();
                        }
                    }
                    break;
                case 110:
                    if (y == 103)
                    {
                        if (!reverbState && !delayState){
                            background = backgroundGenerator(1);
                            repaint();
                            reverbState = true;
                        }
                        else if (!reverbState && delayState){
                            background = backgroundGenerator(3);
                            repaint();
                            reverbState = true;
                        }
                        else if (reverbState && !delayState){
                            background = backgroundGenerator(0);
                            repaint();
                            reverbState = false;
                        }
                        else if (reverbState && delayState){
                            background = backgroundGenerator(2);
                            repaint();
                            reverbState = false;
                        }
                    }
                    break;
                case 776:
                    if (y == 103)
                    {
                        if (!delayState && !reverbState){
                            background = backgroundGenerator(2);
                            repaint();
                            delayState = true;
                        }
                        else if (!delayState && reverbState){
                            background = backgroundGenerator(3);
                            repaint();
                            delayState = true;
                        }
                        else if (delayState && !reverbState){
                            background = backgroundGenerator(0);
                            repaint();
                            delayState = false;
                        }
                        else if (delayState && reverbState){
                            background = backgroundGenerator(1);
                            repaint();
                            delayState = false;
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
