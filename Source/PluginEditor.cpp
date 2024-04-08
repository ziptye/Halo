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
    
    addAndMakeVisible(mainDryWetSlider);
    mainDryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mainDryWetSlider.setRange(0.0, 100.0);
    mainDryWetSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    mainDryWetSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    mainDryWetSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    
    addAndMakeVisible(sixtyFourthNote);
    sixtyFourthNote.setButtonText("1/64");
    sixtyFourthNote.setClickingTogglesState(true);
    sixtyFourthNote.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    sixtyFourthNote.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    sixtyFourthNote.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    sixtyFourthNote.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    
    addAndMakeVisible(thirtySecondNote);
    thirtySecondNote.setButtonText("1/32");
    thirtySecondNote.setClickingTogglesState(true);
    thirtySecondNote.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    thirtySecondNote.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    thirtySecondNote.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    thirtySecondNote.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    
    addAndMakeVisible(sixteenthNote);
    sixteenthNote.setButtonText("1/16");
    sixteenthNote.setClickingTogglesState(true);
    sixteenthNote.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    sixteenthNote.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    sixteenthNote.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    sixteenthNote.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    
    addAndMakeVisible(eighthNote);
    eighthNote.setButtonText("1/8");
    eighthNote.setClickingTogglesState(true);
    eighthNote.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    eighthNote.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    eighthNote.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    eighthNote.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    
    addAndMakeVisible(quarterNote);
    quarterNote.setButtonText("1/4");
    quarterNote.setClickingTogglesState(true);
    quarterNote.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    quarterNote.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    quarterNote.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    quarterNote.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    
    addAndMakeVisible(halfNote);
    halfNote.setButtonText("1/2");
    halfNote.setClickingTogglesState(true);
    halfNote.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    halfNote.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    halfNote.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    halfNote.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    
    addAndMakeVisible(wholeNote);
    wholeNote.setButtonText("1/1");
    wholeNote.setClickingTogglesState(true);
    wholeNote.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    wholeNote.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    wholeNote.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    wholeNote.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
    
    createPanelNavArrows();
    addImagesToArray();
    
    // Reverb Settings:
    presentBank1Settings.add("Default");
    presentBank1Settings.add("Long Hall");
    presentBank1Settings.add("Short Hall");
    presentBank1Settings.add("Room 1");
    presentBank1Settings.add("Room 2");
    
    // Delay Settings:
    presentBank2Settings.add("Default");
    presentBank2Settings.add("Ping-Pong");
    presentBank2Settings.add("BPM Sync");
    presentBank2Settings.add("MS");
        
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
    
    // Preset Bank 1 Label
    g.setFont(juce::Font("Copperplate", 14.0f, juce::Font::bold));
    g.setColour(juce::Colours::white);
    g.drawText(presentBankSettingsGenerator(0, currentIndexPresetBank1), 125, 294, 85, 35, juce::Justification::centred);
    
    // Preset Bank 2 Label
    g.setFont(juce::Font("Copperplate", 14.0f, juce::Font::bold));
    g.setColour(juce::Colours::white);
    g.drawText(presentBankSettingsGenerator(1, currentIndexPresetBank2), 792, 294, 85, 35, juce::Justification::centred);
    
    // Dist. Amount
    g.setFont(juce::Font("Copperplate", 14.0f, 0));
    g.setColour(juce::Colours::white);
    auto s = std::to_string(distortionAmt);
    g.drawText(s, 340, 49, 35, 20, juce::Justification::centred);
    
    // Cozy Mode Amount
    g.setFont(juce::Font("Copperplate", 14.0f, 0));
    g.setColour(juce::Colours::white);
    auto s2 = std::to_string(cozyModeAmt);
    g.drawText(s2, 340, 139, 35, 20, juce::Justification::centred);
    
    // Shifter Amount
    g.setFont(juce::Font("Copperplate", 14.0f, 0));
    g.setColour(juce::Colours::white);
    auto s3 = std::to_string(shifterAmt);
    g.drawText(s3, 504, 49, 35, 20, juce::Justification::centred);
    
    // Sick-O Mode Amount
    g.setFont(juce::Font("Copperplate", 14.0f, 0));
    g.setColour(juce::Colours::white);
    auto s4 = std::to_string(sickoModeAmt);
    g.drawText(s4, 504, 139, 35, 20, juce::Justification::centred);
    
}

juce::String ProjectHaloAudioProcessorEditor::presentBankSettingsGenerator(int num, int pos){

    if (num == 0){
        int index = juce::jmin(pos, presentBank1Settings.size() - 1);
        return presentBank1Settings[index];
    }
    else if (num == 1) {
        int index = juce::jmin(pos, presentBank2Settings.size() - 1);
        return presentBank2Settings[index];
    }
    
    return "";
    
}

void ProjectHaloAudioProcessorEditor::resized()
{
//    animatedKnob1.setBounds(783, 400, 100, 100);
    mainDryWetSlider.setBounds(393, 300, 213, 222);
    sixtyFourthNote.setBounds(670, 200, 60, 30);
    thirtySecondNote.setBounds(740, 200, 60, 30);
    sixteenthNote.setBounds(810, 200, 60, 30);
    eighthNote.setBounds(880, 200, 60, 30);
    quarterNote.setBounds(670, 240, 60, 30);
    halfNote.setBounds(740, 240, 60, 30);
    wholeNote.setBounds(810, 240, 60, 30);
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
    
    int index = juce::jmin(pos, imagesArray.size() - 1);
    return imagesArray[index];
    
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
    
    juce::Rectangle<int> distortionAmtUp(347, 25, 20, 20);
    juce::Rectangle<int> distortionAmtDown(347, 75, 20, 20);
    juce::Rectangle<int> cozyModeAmtUp(347, 115, 20, 20);
    juce::Rectangle<int> cozyModeAmtDown(347, 165, 20, 20);
    juce::Rectangle<int> shifterAmtUp(512, 25, 20, 20);
    juce::Rectangle<int> shifterAmtDown(512, 75, 20, 20);
    juce::Rectangle<int> sickoModeAmtUp(512, 115, 20, 20);
    juce::Rectangle<int> sickoModeAmtDown(512, 165, 20, 20);
    
    
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
    
    rectangleArr.add(distortionAmtUp);
    rectangleArr.add(distortionAmtDown);
    rectangleArr.add(cozyModeAmtUp);
    rectangleArr.add(cozyModeAmtDown);
    rectangleArr.add(shifterAmtUp);
    rectangleArr.add(shifterAmtDown);
    rectangleArr.add(sickoModeAmtUp);
    rectangleArr.add(sickoModeAmtDown);
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
                        if (currentIndexPresetBank1 == 0){
                            currentIndexPresetBank1 = presentBank1Settings.size() - 1;
                        }
                        else {
                            currentIndexPresetBank1--;
                        }
                        repaint();
                    }
                    break;
                case 304:
                    if (y == 150)
                    {
                        std::cout << "RIGHT PANEL 1";
                    }
                    else if (y == 299)
                    {
                        currentIndexPresetBank1 = (currentIndexPresetBank1 + 1) % presentBank1Settings.size();
                        repaint();
                    }
                    break;
                case 347:
                    if (y == 25)
                    {
                        if (distortionAmt < 100 && distortionState) // && distortionState ?????
                        {
                            distortionAmt += 2;
                            repaint();
                        }
                        distortionAmt += 0;
                    }
                    else if (y == 75)
                    {
                        // Checks that distortionAmt is non-negative
                        if (distortionAmt > 0 && distortionState)
                        {
                            distortionAmt -= 2;
                            repaint();
                        }
                        distortionAmt -= 0;
                    }
                    else if (y == 115 && cozyModeState)
                    {
                        if (cozyModeAmt < 100)
                        {
                            cozyModeAmt += 2;
                            repaint();
                        }
                        cozyModeAmt += 0;
                    }
                    else if (y == 165 && cozyModeState)
                    {
                        // Checks that cozyModeAmt is non-negative
                        if (cozyModeAmt > 0)
                        {
                            cozyModeAmt -= 2;
                            repaint();
                        }
                        cozyModeAmt -= 0;
                    }
                    break;
                case 512:
                    if (y == 25 && shifterState)
                    {
                        if (shifterAmt < 100)
                        {
                            shifterAmt += 2;
                            repaint();
                        }
                        shifterAmt += 0;
                    }
                    else if (y == 75 && shifterState)
                    {
                        // Checks that shifterAmt is non-negative
                        if (shifterAmt > 0)
                        {
                            shifterAmt -= 2;
                            repaint();
                        }
                        shifterAmt -= 0;
                    }
                    else if (y == 115 && sickoModeState)
                    {
                        if (sickoModeAmt < 100)
                        {
                            sickoModeAmt += 2;
                            repaint();
                        }
                        sickoModeAmt += 0;
                    }
                    else if (y == 165 && sickoModeState)
                    {
                        // Checks that sickoModeAmt is non-negative
                        if (sickoModeAmt > 0)
                        {
                            sickoModeAmt -= 2;
                            repaint();
                        }
                        sickoModeAmt -= 0;
                    }
                    break;
                case 670:
                    if (y == 150)
                    {
                        std::cout << "LEFT PANEL 3";
                    }
                    else if (y == 299)
                    {
                        if (currentIndexPresetBank2 == 0)
                        {
                            currentIndexPresetBank2 = presentBank2Settings.size() - 1;
                        }
                        else {
                            currentIndexPresetBank2--;
                        }
                        repaint();
                    }
                    break;
                case 965:
                    if (y == 150)
                    {
                        std::cout << "RIGHT PANEL 3";
                    }
                    else if (y == 299)
                    {
                        currentIndexPresetBank2 = (currentIndexPresetBank2 + 1) % presentBank2Settings.size();
                        repaint();
                    }
                    break;
                case 393:
                    if (y == 30)
                    {
                        int i = 0;
                        
                        if (!distortionState && i == 0)
                        {
                            i++;
                            distortionState = true;
                            repaint();
                        }
                        else
                        {
                            distortionState = false;
                            i = 0;
                            repaint();
                        }
                    }
                    else if (y == 123){
                        
                        int i = 0;
                        
                        if (!cozyModeState && i == 0)
                        {
                            i++;
                            cozyModeState = true;
                            repaint();
                        }
                        else 
                        {
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
                        
                        if (!shifterState && i == 0)
                        {
                            i++;
                            shifterState = true;
                            repaint();
                        }
                        else
                        {
                            i = 0;
                            shifterState = false;
                            repaint();
                        }
                    }
                    else if (y == 123){
                        
                        int i = 0;
                        
                        if (!sickoModeState && i == 0)
                        {
                            i++;
                            sickoModeState = true;
                            repaint();
                        }
                        else 
                        {
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
