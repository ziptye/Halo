/*
  ==============================================================================

 PluginEditor.cpp
 Author:  Zachary Pennington
 Project: Halo

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
    mainDryWetSlider.setColour(juce::Slider::trackColourId, juce::Colours::transparentWhite);
    mainDryWetSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    
    configSlider(delayFeedback, 0.0, 100.0, juce::Colours::white, juce::Colours::royalblue);
    configSlider(delayHPF, 0.0, 100.0, juce::Colours::white, juce::Colours::goldenrod);
    configSlider(delayLPF, 0.0, 100.0, juce::Colours::white, juce::Colours::limegreen);
    
    configTextButtons(sixtyFourthNote, "1/64");
    configTextButtons(thirtySecondNote, "1/32");
    configTextButtons(sixteenthNote, "1/16");
    configTextButtons(eighthNote, "1/8");
    configTextButtons(quarterNote, "1/4");
    configTextButtons(halfNote, "1/2");
    configTextButtons(wholeNote, "1/1");
    
    createClickableAreas();
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
void ProjectHaloAudioProcessorEditor::configSlider(juce::Slider &slider, double minVal, double maxVal, juce::Colour thumbColour, juce::Colour fillColour)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setRange(minVal, maxVal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    slider.setColour(juce::Slider::thumbColourId, thumbColour);
    slider.setColour(juce::Slider::rotarySliderFillColourId, fillColour);
}

void ProjectHaloAudioProcessorEditor::configTextButtons(juce::TextButton &button, const juce::String &text)
{
    button.setButtonText(text);
    button.setClickingTogglesState(true);
    button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::black);
    button.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    button.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
}

void ProjectHaloAudioProcessorEditor::configPresetBanks(juce::StringArray &strArray, const juce::String &text)
{
    configTextButtons(sixtyFourthNote, "1/64");
    configTextButtons(thirtySecondNote, "1/32");
    configTextButtons(sixteenthNote, "1/16");
    configTextButtons(eighthNote, "1/8");
    configTextButtons(quarterNote, "1/4");
    configTextButtons(halfNote, "1/2");
    configTextButtons(wholeNote, "1/1");
}

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
    mainDryWetSlider.setBounds(393, 299, 213, 227);
    sixtyFourthNote.setBounds(672, 205, 75, 30);
    thirtySecondNote.setBounds(752, 205, 75, 30);
    sixteenthNote.setBounds(832, 205, 75, 30);
    eighthNote.setBounds(912, 205, 75, 30);
    quarterNote.setBounds(712, 245, 75, 30);
    halfNote.setBounds(793, 245, 75, 30);
    wholeNote.setBounds(873, 245, 75, 30);
    
    delayFeedback.setBounds(670, 200, 80, 80);
    delayHPF.setBounds(792, 200, 80, 80);
    delayLPF.setBounds(910, 200, 80, 80);
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

void ProjectHaloAudioProcessorEditor::createClickableAreas()
{
    // Adds coords. of clickable bounds to the rectangles vector.
    std::vector<juce::Rectangle<int>> rectangles =
    {
        {10, 150, 25, 25}, // P1L
        {304, 150, 25, 25}, // P1R
        {670, 150, 25, 25}, // P3L
        {965, 150, 25, 25}, // P3R
        {10, 299, 25, 25}, // presetBank1L
        {304, 299, 25, 25}, // presetBank1R
        {670, 299, 25, 25}, // presetBank2L
        {965, 299, 25, 25}, // presetBank2R
        {110, 103, 116, 120}, // Reverb on/off
        {776, 103, 116, 120}, // Delay on/off
        {393, 30, 50, 50}, // Distortion FX
        {393, 123, 50, 50}, // Cozy Mode FX
        {553, 30, 50, 50}, // Shifter FX
        {553, 123, 50, 50}, // Sick-o-Mode FX
        {347, 25, 20, 20}, // Distortion Amt Up
        {347, 75, 20, 20}, // Distortion Amt Down
        {347, 115, 20, 20}, // Cozy Mode Amt Up
        {347, 165, 20, 20}, // Cozy Mode Amt Down
        {512, 25, 20, 20}, // Shifter Amt Up
        {512, 75, 20, 20}, // Shifter Amt Down
        {512, 115, 20, 20}, // Sick-o-Mode Amt Up
        {512, 165, 20, 20}, // Sick-o-Mode Amt Down
    };
    
    for (const auto& rect : rectangles)
    {
        rectangleArr.add(rect);
    }
}

std::vector<juce::Component*>ProjectHaloAudioProcessorEditor::getComps()
{
    return {
        &sixtyFourthNote,
        &thirtySecondNote,
        &sixteenthNote,
        &eighthNote,
        &quarterNote,
        &halfNote,
        &wholeNote,
    };
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
                    if (y == 150) // LP1
                    {
//                        std::cout << "LEFT PANEL 1";
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
                    if (y == 150) // RP1
                    {
//                        std::cout << "RIGHT PANEL 1";
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
                        if (distortionAmt < 100 && distortionState)
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
                    if (y == 150) // LP3
                    {
                        if (delayState)
                        {
                            for(auto* comp : getComps())
                            {
                                addAndMakeVisible(comp); // RENDERS ALL DELAY COMPONENTS
                            }
                            // TODO --> Update the getComps() func to include this code
                            removeChildComponent(&delayFeedback);
                            removeChildComponent(&delayHPF);
                            removeChildComponent(&delayLPF);
                        }
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
                    if (y == 150) // RP3
                    {
                        if (delayState)
                        {
                            for(auto* comp : getComps())
                            {
                                removeChildComponent(comp); // DELETES ALL DELAY COMPONENTS
                            }
                            // TODO --> Update the getComps() func to include this code
                            addAndMakeVisible(delayFeedback);
                            addAndMakeVisible(delayHPF);
                            addAndMakeVisible(delayLPF);
                        }
                    
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
                            
                            for(auto* comp : getComps())
                            {
                                addAndMakeVisible(comp); // RENDERS ALL DELAY COMPONENTS
                            }
                        }
                        else if (!delayState && reverbState){
                            background = backgroundGenerator(3);
                            repaint();
                            delayState = true;
                            
                            for(auto* comp : getComps())
                            {
                                addAndMakeVisible(comp); // RENDERS ALL DELAY COMPONENTS
                            }
                        }
                        else if (delayState && !reverbState){
                            background = backgroundGenerator(0);
                            repaint();
                            delayState = false;
                            
                            for(auto* comp : getComps())
                            {
                                removeChildComponent(comp); // DELETES ALL DELAY COMPONENTS
                            }
                            // TODO --> Update the getComps() func to include this code
                            removeChildComponent(&delayFeedback);
                            removeChildComponent(&delayHPF);
                            removeChildComponent(&delayLPF);
                        }
                        else if (delayState && reverbState){
                            background = backgroundGenerator(1);
                            repaint();
                            delayState = false;
                            
                            for(auto* comp : getComps())
                            {
                                removeChildComponent(comp); // DELETES ALL DELAY COMPONENTS
                            }
                            // TODO --> Update the getComps() func to include this code
                            removeChildComponent(&delayFeedback);
                            removeChildComponent(&delayHPF);
                            removeChildComponent(&delayLPF);
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
