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
    : AudioProcessorEditor (&p), audioProcessor (p), bpmHandler(p), fxHandler(p)
{
    addAndMakeVisible(mainDryWetSlider);
    
    createClickableAreas();
    addImagesToArray();
    generateReverbParticles();
    generateDelayParticles();
    
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
    
    // SLIDER ATTACHMENTS ==============================================
    reverbRoomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Room Size", reverbRoomSize);
    
    reverbPreDelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Pre Delay", reverbPreDelay);
    
    reverbDampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Damping", reverbDamping);
    
    reverbWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Width", reverbWidth);
    
    reverbHPFAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Reverb HPF", reverbHPF);
    
    reverbLPFAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Reverb LPF", reverbLPF);
    
    delayFeedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Feedback", delayFeedback);
    
    delayHPFAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Delay HPF", delayHPF);
    
    delayLPFAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Delay LPF", delayLPF);
    
    mainDryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Dry Wet", mainDryWetSlider);
    
    // ======= DELAY =======
    delay64 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Delay64", sixtyFourthNote);
    
    delay32 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Delay32", thirtySecondNote);
    
    delay16 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Delay16", sixteenthNote);
    
    delay8 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Delay8", eighthNote);
    
    delay4 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Delay4", quarterNote);
    
    delay2 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Delay2", halfNote);
    
    delay1 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Delay1", wholeNote);
    
    for(auto* comp : getOtherComps())
    {
        addAndMakeVisible(comp); // RENDERS VISUALIZER COMP.
    }
    showActivationWindow();
    
//
//    if (!audioProcessor.getIsActivated())
//    {
//        //        std::cout << "ACTIVATION STATUS: " << audioProcessor.getIsActivated() << std::endl;
//        juce::DialogWindow::LaunchOptions options;
//        options.content.setOwned(new LicenseActivationWindow());
//        options.dialogTitle = "Activate Project Halo";
//        options.dialogBackgroundColour = juce::Colours::black;
//        options.escapeKeyTriggersCloseButton = true;
//        options.useNativeTitleBar = true;
//        options.resizable = false;
//        
//        auto* dialog = options.launchAsync();
//        dialog->centreWithSize(400, 250);
//        dialog->setAlwaysOnTop(true);
//        
//        audioProcessor.setIsActivated(true);
//        
//        // Check if activation was successful
//        if (!audioProcessor.getIsActivated())
//        {
//            std::cout << "ACTIVATION STATUS: " << audioProcessor.getIsActivated() << std::endl;
//        }
//        else
//        {
//            std::cout << "Activation failed, please try again." << std::endl;
//        }
//        
//    }
}
ProjectHaloAudioProcessorEditor::~ProjectHaloAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================

void ProjectHaloAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    g.drawImageAt(background, 0, 0);
    
    // DIST. STATUS ----
    if (!audioProcessor.getDistortionState())
        drawLEDLights(g, juce::Colours::red, 467, 75, 8, 8, 4.0f);
    else
        drawLEDLights(g, juce::Colours::lime, 467, 30, 8, 8, 4.0f);
    
    // SHIFTER STATUS ----
    if (!audioProcessor.getShifterState())
        drawLEDLights(g, juce::Colours::red, 628, 75, 8, 8, 4.0f);
    else
        drawLEDLights(g, juce::Colours::lime, 628, 30, 8, 8, 4.0f);
    
    // COZY MODE STATUS  ----
    if (!audioProcessor.getCozyModeState())
        drawLEDLights(g, juce::Colours::red, 467, 170, 8, 8, 4.0f);
    else
        drawLEDLights(g, juce::Colours::lime, 467, 122, 8, 8, 4.0f);
    
    // SICKOMODE STATUS ----
    if (!audioProcessor.getSickOModeState())
        drawLEDLights(g, juce::Colours::red, 628, 170, 8, 8, 4.0f);
    else 
        drawLEDLights(g, juce::Colours::lime, 628, 122, 8, 8, 4.0f);
    
    // Preset Bank 1 Label
    drawLabel(g, 14.0f, presentBankSettingsGenerator(0, currentIndexPresetBank1), 125, 294);
    
    // Preset Bank 2 Label
    drawLabel(g, 14.0f, presentBankSettingsGenerator(1, currentIndexPresetBank2), 792, 294);
    
    // Dist. Amount
    drawText(g, juce::Colours::white, 14.0f, std::to_string(static_cast<int>(*audioProcessor.distortionAmt)), 340, 49);
    
    // Cozy Mode Amount
    drawText(g, juce::Colours::white, 14.0f, std::to_string(static_cast<int>(*audioProcessor.cozyModeAmt)), 340, 139);
    
    // Shifter Amount
    drawText(g, juce::Colours::white, 14.0f, std::to_string(static_cast<int>(*audioProcessor.shifterAmt)), 504, 49);
    
    // Sick-O Mode Amount
    drawText(g, juce::Colours::white, 14.0f, std::to_string(static_cast<int>(*audioProcessor.sickoModeAmt)), 504, 139);
    
    // Display BPM
    drawText(g, juce::Colours::black, 16.0f, std::to_string(audioProcessor.bpmVal), 233, 406);
    
    // Adds text below the various reverb knobs
    if(audioProcessor.getReverbState() && currentVerbIndex == 0)
    {
        drawLabel(g, 10.0f, "Room Size", 8, 255);
        drawLabel(g, 10.0f, "Pre-Delay", 125, 255);
        drawLabel(g, 10.0f, "Damping", 246, 255);
    }
    else if (audioProcessor.getReverbState() && currentVerbIndex == 1)
    {
        drawLabel(g, 10.0f, "Width", 8, 255);
        drawLabel(g, 10.0f, "HPF", 125, 255);
        drawLabel(g, 10.0f, "LPF", 246, 255);
    }
    
    // Adds text below the various delay knobs
    if (audioProcessor.getDelayState() && currentDelayIndex == 1)
    {
        drawLabel(g, 10.0f, "Feedback", 668, 255);
        drawLabel(g, 10.0f, "HPF", 790, 255);
        drawLabel(g, 10.0f, "LPF", 908, 255);
    }
    
    // Light/Dark Mode Text Toggle
    if (!darkModeState)
    {
        drawLabel(g, 12.0f, "Light Mode", 330, 190);
        drawLEDLights(g, juce::Colours::cyan, 370, 220, 8, 8, 4.0f);
    }
    else
    {
        drawLabel(g, 12.0f, "Dark Mode", 585, 190);
        drawLEDLights(g, juce::Colours::blueviolet, 625, 220, 8, 8, 4.0f);
    }
    
    // Displays LED lights for Tap Tempo
    if (tapTimes.size() == 4 || tapTimes.size() == 0)
    {
        drawLEDLights(g, juce::Colours::limegreen, 150, 430, 8, 8, 4.0f);
    }
    else if (tapTimes.size() < 4)
    {
        drawLEDLights(g, juce::Colours::orange, 150, 400, 8, 8, 4.0f);
    }
    
    if(!audioProcessor.getReverbState())
    {
        // Draw particles
        for (auto& particle : particlesReverb)
        {
            particle->draw(g);
        }
    }
    
    if(!audioProcessor.getDelayState())
    {
        // Draw particles
        for (auto& particle : particlesDelay)
        {
            particle->draw(g);
        }
    }
    
}

void ProjectHaloAudioProcessorEditor::generateReverbParticles()
{
    // Create Particles:
    particleBoundsReverb = juce::Rectangle<float>(5, 45, 320, 230); // X, Y, W, H

    // Create particles within the defined bounds
    for (int i = 0; i < 50; ++i)
        particlesReverb.push_back(std::make_unique<AnimatedParticles>(
            juce::Random::getSystemRandom().nextFloat() * particleBoundsReverb.getWidth() + particleBoundsReverb.getX(),
            juce::Random::getSystemRandom().nextFloat() * particleBoundsReverb.getHeight() + particleBoundsReverb.getY(),
            particleBoundsReverb));

    // Start timer for animation:
    startTimerHz(60);
}

void ProjectHaloAudioProcessorEditor::generateDelayParticles()
{
    // Create Particles:
    particleBoundsDelay = juce::Rectangle<float>(667, 45, 320, 230); // X, Y, W, H

    // Create particles within the defined bounds
    for (int i = 0; i < 50; ++i)
        particlesDelay.push_back(std::make_unique<AnimatedParticles>(
            juce::Random::getSystemRandom().nextFloat() * particleBoundsDelay.getWidth() + particleBoundsDelay.getX(),
            juce::Random::getSystemRandom().nextFloat() * particleBoundsDelay.getHeight() + particleBoundsDelay.getY(), particleBoundsDelay));

    // Start timer for animation:
    startTimerHz(60);
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

void ProjectHaloAudioProcessorEditor::drawText(juce::Graphics &g, juce::Colour color, float fontSize, const juce::String &text, int x, int y)
{
    g.setFont(juce::Font(juce::FontOptions("Copperplate", fontSize, 0)));
    g.setColour(color);
    g.drawText(text, x, y, 35, 20, juce::Justification::centred);
}

void ProjectHaloAudioProcessorEditor::drawLabel(juce::Graphics &g, float fontSize, const juce::String &text, int x, int y)
{
    g.setFont(juce::Font(juce::FontOptions("Copperplate", fontSize, juce::Font::bold)));
    g.setColour(juce::Colours::white);
    g.drawText(text, x, y, 85, 35, juce::Justification::centred);
}

void ProjectHaloAudioProcessorEditor::drawLEDLights(juce::Graphics& g, juce::Colour color, float x, float y, float w, float h, float cornerSize)
{
    g.setColour(color);
    g.fillRoundedRectangle(x, y, w, h, cornerSize);
}

void ProjectHaloAudioProcessorEditor::showActivationWindow()
{
    
    if (!audioProcessor.getIsActivated())
    {
        //        std::cout << "ACTIVATION STATUS: " << audioProcessor.getIsActivated() << std::endl;
        juce::DialogWindow::LaunchOptions options;
        options.content.setOwned(new LicenseActivationWindow());
        options.dialogTitle = "Activate Project Halo";
        options.dialogBackgroundColour = juce::Colours::black;
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.resizable = false;
        
        auto* dialog = options.launchAsync();
        dialog->centreWithSize(400, 250);
        dialog->setAlwaysOnTop(true);
        
    }
}

void ProjectHaloAudioProcessorEditor::resized()
{
//    animatedKnob1.setBounds(783, 400, 100, 100);
//    audioProcessor.getVisualizer().setBounds(681, 374, 300, 65); // x, y , w, h
}

void ProjectHaloAudioProcessorEditor::addImagesToArray()
{
    // Defines light mode images:
    auto initLightBackground = juce::ImageCache::getFromMemory(BinaryData::PH_InitBG_png, BinaryData::PH_InitBG_pngSize);
    auto bg1Light = juce::ImageCache::getFromMemory(BinaryData::PH_RonDoff_png, BinaryData::PH_RonDoff_pngSize);
    auto bg2Light = juce::ImageCache::getFromMemory(BinaryData::PH_RoffDon_png, BinaryData::PH_RoffDon_pngSize);
    auto bg3Light = juce::ImageCache::getFromMemory(BinaryData::PH_RonDon_png, BinaryData::PH_RonDon_pngSize);
    
    // Defines dark mode images:
    auto initDarkBackground = juce::ImageCache::getFromMemory(BinaryData::PH_DMInit_png, BinaryData::PH_DMInit_pngSize);
    auto bg1Dark = juce::ImageCache::getFromMemory(BinaryData::PH_DMRonDoff_png, BinaryData::PH_DMRonDoff_pngSize);
    auto bg2Dark = juce::ImageCache::getFromMemory(BinaryData::PH_DMRoffDon_png, BinaryData::PH_DMRoffDon_pngSize);
    auto bg3Dark = juce::ImageCache::getFromMemory(BinaryData::PH_DMRonDon_png, BinaryData::PH_DMRonDon_pngSize);
    
    // Adds all images to array
    imagesArray.add(initLightBackground);
    imagesArray.add(bg1Light);
    imagesArray.add(bg2Light);
    imagesArray.add(bg3Light);
    
    imagesArray.add(initDarkBackground);
    imagesArray.add(bg1Dark);
    imagesArray.add(bg2Dark);
    imagesArray.add(bg3Dark);
    
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
        {393, 30, 50, 50}, // Distortion FX on/off
        {393, 123, 50, 50}, // Cozy Mode FX on/off
        {553, 30, 50, 50}, // Shifter FX on/off
        {553, 123, 50, 50}, // Sick-o-Mode FX on/off
        {347, 25, 20, 20}, // Distortion Amt Up
        {347, 75, 20, 20}, // Distortion Amt Down
        {347, 115, 20, 20}, // Cozy Mode Amt Up
        {347, 165, 20, 20}, // Cozy Mode Amt Down
        {512, 25, 20, 20}, // Shifter Amt Up
        {512, 75, 20, 20}, // Shifter Amt Down
        {512, 115, 20, 20}, // Sick-o-Mode Amt Up
        {512, 165, 20, 20}, // Sick-o-Mode Amt Down
        
        {52, 385, 64, 64}, // Tap Tempo Feature
        {183, 385, 20, 20}, // BPM Up
        {183, 430, 20, 20}, // BPM Down
        
        {335, 195, 328, 90}, // Light/Dark Mode Selector
    };
    
    for (const auto& rect : rectangles)
    {
        rectangleArr.add(rect);
    }
}

std::vector<juce::Component*>ProjectHaloAudioProcessorEditor::getDelayComps(int curPage)
{
    if (curPage == 0)
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
    else 
    {
        return {
            &delayFeedback,
            &delayHPF,
            &delayLPF
        };
    }
    
}

std::vector<juce::Component*>ProjectHaloAudioProcessorEditor::getReverbComps(int pageNum)
{
    if (pageNum == 0)
    {
        return {
            &reverbRoomSize,
            &reverbPreDelay,
            &reverbDamping
        };
    }
    else
    {
        return {
            &reverbWidth,
            &reverbHPF,
            &reverbLPF
        };
    }
}

std::vector<juce::Component*>ProjectHaloAudioProcessorEditor::getOtherComps()
{
    return 
    {
        &audioProcessor.getVisualizer()
    };
}
// ====================================================================================================================
void ProjectHaloAudioProcessorEditor::handleCompClick(const juce::Rectangle<int> &rect)
{
    int x = rect.getX();
    int y = rect.getY();
    
    switch (x)
    {
        case 10:
            handlePanelLeft(x, y);
            break;
        case 52:
            updateTempo();
            break;
        case 183:
            handleManualTempoChange(x, y);
            break;
        case 304:
            handlePanelRight(x, y);
            break;
        case 670:
            handlePanelLeft(x, y);
            break;
        case 965:
            handlePanelRight(x, y);
            break;
        case 110:
            handleReverbPowerToggle();
            break;
        case 776:
            handleDelayToggle();
            break;
        case 347:
            handleFXAmounts1(y);
            break;
        case 335:
            if (!darkModeState)
                darkModeState = true;
            else
                darkModeState = false;
            
            // Updates background when toggling dark mode
            if (!audioProcessor.getReverbState() && !audioProcessor.getDelayState())
                background = darkModeState ? backgroundGenerator(4) : backgroundGenerator(0);
            else if (!audioProcessor.getReverbState() && audioProcessor.getDelayState())
                background = darkModeState ? backgroundGenerator(6) : backgroundGenerator(2);
            else if (audioProcessor.getReverbState() && !audioProcessor.getDelayState())
                background = darkModeState ? backgroundGenerator(5) : backgroundGenerator(1);
            else if (audioProcessor.getReverbState() && audioProcessor.getDelayState())
                background = darkModeState ? backgroundGenerator(7) : backgroundGenerator(3);
            break;
        case 512:
            handleFXAmounts2(y);
            break;
        case 393:
            handleFXPowerToggles1(y);
            break;
        case 553:
            handleFXPowerToggles2(y);
            break;
        default:
            break;
    }
}

void ProjectHaloAudioProcessorEditor::handlePanelLeft(int x, int y)
{
    if (x == 10 && y == 150) // LP1
    {
        if (audioProcessor.getReverbState())
        {
            renderReverbComps(0, -1);
            if (currentVerbIndex > 0)
                currentVerbIndex--;
        }
    }
    else if (x == 670 && y == 150) // LP3
    {
        if (audioProcessor.getDelayState())
        {
            renderDelayComps(0, -1);
            if (currentDelayIndex > 0)
                currentDelayIndex--;
        }
    }
    else if (x == 10 && y == 299) // presetBank1L
    {
        currentIndexPresetBank1 = (currentIndexPresetBank1 == 0) ? presentBank1Settings.size() - 1 : currentIndexPresetBank1 - 1;
        repaint();
    }
    else if (x == 670 && y == 299) // presetBank2L
    {
        currentIndexPresetBank2 = (currentIndexPresetBank2 == 0) ? presentBank2Settings.size() - 1 : currentIndexPresetBank2 - 1;
        repaint();
    }
}

void ProjectHaloAudioProcessorEditor::handlePanelRight(int x, int y)
{
    if (x == 304 && y == 150) // RP1
    {
        if (audioProcessor.getReverbState())
        {
            int numPages = 1;
            renderReverbComps(1, 1);
            if (currentVerbIndex < numPages)
                currentVerbIndex++;
        }
    }
    else if (x == 965 && y == 150) // RP3
    {
        if (audioProcessor.getDelayState())
        {
            int numPages = 1;
            renderDelayComps(1, 1);
            if (currentDelayIndex < numPages)
                currentDelayIndex++;
        }
    }
    else if (x == 304 && y == 299) // presetBank1R
    {
        currentIndexPresetBank1 = (currentIndexPresetBank1 + 1) % presentBank1Settings.size();
        repaint();
    }
    else if (x == 965 && y == 299) // presetBank1R
    {
        currentIndexPresetBank2 = (currentIndexPresetBank2 + 1) % presentBank2Settings.size();
        repaint();
    }
}

void ProjectHaloAudioProcessorEditor::handleReverbPowerToggle()
{
    if (!audioProcessor.getReverbState() && !audioProcessor.getDelayState())
    {
        background = darkModeState ? backgroundGenerator(5) : backgroundGenerator(1);
        audioProcessor.setReverbState(true);
        renderReverbComps(currentVerbIndex, 0);
    }
    else if (!audioProcessor.getReverbState() && audioProcessor.getDelayState())
    {
        background = darkModeState ? backgroundGenerator(7) : backgroundGenerator(3);
        audioProcessor.setReverbState(true);
        renderReverbComps(currentVerbIndex, 0);
    }
    else if (audioProcessor.getReverbState() && !audioProcessor.getDelayState())
    {
        background = darkModeState ? backgroundGenerator(4) : backgroundGenerator(0);
        audioProcessor.setReverbState(false);
        hideReverbComps(currentVerbIndex);
    }
    else if (audioProcessor.getReverbState() && audioProcessor.getDelayState())
    {
        background = darkModeState ? backgroundGenerator(6) : backgroundGenerator(2);
        audioProcessor.setReverbState(false);
        hideReverbComps(currentVerbIndex);
    }
}

void ProjectHaloAudioProcessorEditor::handleDelayToggle()
{
    if (!audioProcessor.getDelayState() && !audioProcessor.getReverbState())
    {
        background = darkModeState ? backgroundGenerator(6) : backgroundGenerator(2);
        audioProcessor.setDelayState(true);
        renderDelayComps(currentDelayIndex, 0);
    }
    else if (!audioProcessor.getDelayState() && audioProcessor.getReverbState())
    {
        background = darkModeState ? backgroundGenerator(7) : backgroundGenerator(3);
        audioProcessor.setDelayState(true);
        renderDelayComps(currentDelayIndex, 0);
    }
    else if (audioProcessor.getDelayState() && !audioProcessor.getReverbState())
    {
        background = darkModeState ? backgroundGenerator(4) : backgroundGenerator(0);
        audioProcessor.setDelayState(false);
        hideDelayComps(currentDelayIndex);
    }
    else if (audioProcessor.getDelayState() && audioProcessor.getReverbState())
    {
        background = darkModeState ? backgroundGenerator(5) : backgroundGenerator(1);
        audioProcessor.setDelayState(false);
        hideDelayComps(currentDelayIndex);
    }
}

void ProjectHaloAudioProcessorEditor::renderReverbComps(int page, int direction)
{
    if (direction == 0) // Default, used for power only
    {
        for (auto* comp : getReverbComps(page))
        {
            addAndMakeVisible(comp);
        }
    }
    else if (direction == -1) // Left arrow
    {
        for (auto* comp: getReverbComps(page))
        {
            addAndMakeVisible(comp);
            hideReverbComps(page + 1);
        }
    }
    else if (direction == 1) // Right arrow
    {
        for (auto* comp: getReverbComps(page))
        {
            addAndMakeVisible(comp);
            hideReverbComps(page - 1);
        }
    }
}

void ProjectHaloAudioProcessorEditor::renderDelayComps(int page, int direction)
{
    if (direction == 0) // Default, used for power only
    {
        for (auto* comp: getDelayComps(page))
        {
            addAndMakeVisible(comp);
        }
    }
    else if (direction == -1) // Left arrow
    {
        for (auto* comp: getDelayComps(page))
        {
            addAndMakeVisible(comp);
            hideDelayComps(page + 1);
        }
    }
    else if (direction == 1) // Right arrow
    {
        for (auto* comp: getDelayComps(page))
        {
            addAndMakeVisible(comp);
            hideDelayComps(page - 1);
        }
    }
}

void ProjectHaloAudioProcessorEditor::hideReverbComps(int page)
{
    for (auto* comp: getReverbComps(page))
    {
        removeChildComponent(comp);
    }
}

void ProjectHaloAudioProcessorEditor::hideDelayComps(int page)
{
    for (auto* comp: getDelayComps(page))
    {
        removeChildComponent(comp);
    }
}

void ProjectHaloAudioProcessorEditor::handleFXPowerToggles1(int y) // DIST. && COZY MODES
{
    if (y == 30)
    {
        int i = 0;

        if (!audioProcessor.getDistortionState() && i == 0)
        {
            i++;
            audioProcessor.setDistortionState(true);
            repaint();
        }
        else
        {
            audioProcessor.setDistortionState(false);
            i = 0;
            repaint();
        }
    }
    else if (y == 123)
    {
        int i = 0;

        if (!audioProcessor.getCozyModeState() && i == 0)
        {
            i++;
            audioProcessor.setCozyModeState(true);
            repaint();
        }
        else
        {
            i = 0;
            audioProcessor.setCozyModeState(false);
            repaint();
        }
    }
}

void ProjectHaloAudioProcessorEditor::handleFXPowerToggles2(int y) // SHIFTER && SICK-O-MODES
{
    if (y == 30)
    {
        int i = 0;

        if (!audioProcessor.getShifterState() && i == 0)
        {
            i++;
            audioProcessor.setShifterState(true);
            repaint();
        }
        else
        {
            i = 0;
            audioProcessor.setShifterState(false);
            repaint();
        }
    }
    else if (y == 123)
    {

        int i = 0;

        if (!audioProcessor.getSickOModeState() && i == 0)
        {
            i++;
            audioProcessor.setSickOModeState(true);
            repaint();
        }
        else
        {
            i = 0;
            audioProcessor.setSickOModeState(false);
            repaint();
        }
    }
}

void ProjectHaloAudioProcessorEditor::handleFXAmounts1(int y) // DIST. && COZY MODES AMT.
{
    if (y == 25) // Dist. Mode
    {
        fxHandler.startFXChange(true, FXHandler::Mode::Distortion);
    }
    else if (y == 75)
    {
        fxHandler.startFXChange(false, FXHandler::Mode::Distortion);
    }
    else if (y == 115) // Cozy Mode
    {
        fxHandler.startFXChange(true, FXHandler::Mode::CozyMode);
    }
    else if (y == 165)
    {
        fxHandler.startFXChange(false, FXHandler::Mode::CozyMode);
    }
}

void ProjectHaloAudioProcessorEditor::handleFXAmounts2(int y) // SHIFTER && SICK-O-MODES AMT.
{
    if (y == 25) // Shifter Mode
    {
        fxHandler.startFXChange(true, FXHandler::Mode::Shifter);
    }
    else if (y == 75)
    {
        fxHandler.startFXChange(false, FXHandler::Mode::Shifter);
    }
    else if (y == 115) // Sicko Mode
    {
        fxHandler.startFXChange(true, FXHandler::Mode::SickOMode);
    }
    else if (y == 165)
    {
        fxHandler.startFXChange(false, FXHandler::Mode::SickOMode);
    }
}
void ProjectHaloAudioProcessorEditor::mouseUp(const juce::MouseEvent &event)
{
    bpmHandler.stopBpmChange();
    fxHandler.stopFXChange();
}

void ProjectHaloAudioProcessorEditor::timerCallback()
{
    for (auto& particle : particlesReverb)
    {
        particle -> update();
        repaint();
    }
    
    for (auto& particle : particlesDelay)
    {
        particle -> update();
        repaint();
    }
}


void ProjectHaloAudioProcessorEditor::mouseDown(const juce::MouseEvent &event)
{
    juce::Point<int> clickPosition = event.getPosition();
    
    for (const auto& rect : rectangleArr)
    {
        if (rect.contains(clickPosition))
        {
            handleCompClick(rect);
            break;
        }
    }
}

void ProjectHaloAudioProcessorEditor::handleManualTempoChange(int x, int y)
{
    if (x == 183 && y == 385) // BPM Up
    {
        bpmHandler.startBpmChange(true);
    }
    else if (x == 183 && y == 430) // BPM Down
    {
        bpmHandler.startBpmChange(false);
    }
}

void ProjectHaloAudioProcessorEditor::updateTempo()
{
    static const int maxTaps = 4;  // Number of taps to average for tempo calc

    double currentTime = juce::Time::getMillisecondCounterHiRes() / 1000.0;

    if (!tapTimes.empty() && currentTime - tapTimes.back() > 1.5)  // Resets if last tap was more than 1.5 seconds ago
        tapTimes.clear();

    tapTimes.push_back(currentTime);

    if (tapTimes.size() == maxTaps)
    {
        double sumIntervals = 0.0;
        for (int i = 1; i < tapTimes.size(); ++i)
            sumIntervals += (tapTimes[i] - tapTimes[i - 1]);
        
        double averageInterval = sumIntervals / (tapTimes.size() - 1);
        double bpm = 60.0 / averageInterval;
        
        // Updates the current tempo
        audioProcessor.bpmVal = bpm;
        
        // Clear tap times to start new measurement
        tapTimes.clear();
    }
}
