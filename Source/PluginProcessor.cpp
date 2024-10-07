/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProjectHaloAudioProcessor::ProjectHaloAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    distortionAmt = apvts.getRawParameterValue("distortionAmt");
    cozyModeAmt = apvts.getRawParameterValue("cozyModeAmt");
    shifterAmt = apvts.getRawParameterValue("shifterAmt");
    sickoModeAmt = apvts.getRawParameterValue("sickoModeAmt");
    
}

ProjectHaloAudioProcessor::~ProjectHaloAudioProcessor()
{
}

//==============================================================================
const juce::String ProjectHaloAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ProjectHaloAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ProjectHaloAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ProjectHaloAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ProjectHaloAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ProjectHaloAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ProjectHaloAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ProjectHaloAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ProjectHaloAudioProcessor::getProgramName (int index)
{
    return {};
}

void ProjectHaloAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void ProjectHaloAudioProcessor::setReverbState(bool rState)
{
    if (rState)
        reverbState = true;
    else
        reverbState = false;
}

void ProjectHaloAudioProcessor::setDelayState(bool dState)
{
    if (dState)
        delayState = true;
    else
        delayState = false;
}

void ProjectHaloAudioProcessor::setDistortionState(bool distState)
{
    if (distState)
        distortionState = true;
    else
        distortionState = false;
}

void ProjectHaloAudioProcessor::setShifterState(bool shiftState)
{
    if (shiftState)
        shifterState = true;
    else
        shifterState = false;
}

void ProjectHaloAudioProcessor::setCozyModeState(bool cozyState)
{
    if (cozyState)
        cozyModeState = true;
    else
        cozyModeState = false;
}

void ProjectHaloAudioProcessor::setSickOModeState(bool sickState)
{
    if (sickState)
        sickoModeState = true;
    else
        sickoModeState = false;
}
void ProjectHaloAudioProcessor::setIsActivated(bool state)
{
    if (state)
    {
        isActivated = true;
        std::cout << "MASSIVE YES ACTIVATION BROTHER" << std::endl;
    }
    isActivated = false;
}

float ProjectHaloAudioProcessor::calcDelayTime(int timeDivide)
{
    // TODO: This whole function probably needs to be rewritten for computational effciency. Math is a little weird.
    
    float temp = 60000/bpmVal;
    float result = 0;
    
    if (timeDivide > 0)
    {
        for (int i = 0; i < timeDivide; i++)
        {
            result = temp / 2;
            temp = result;
        }
    }
    else if (timeDivide == 0)
    {
        result = temp;
    }
    else if (timeDivide < 0)
    {
        result = (temp * timeDivide) * -1;
    }
    
    return result;
}

//==============================================================================
void ProjectHaloAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    reset(); // resets all parameters of the plugin

    smoothedReverbPreDelayTime.reset(sampleRate, 0.02);
    
    verbHPF.prepare(spec);
    verbLPF.prepare(spec);
    delayHPF.prepare(spec);
    delayLPF.prepare(spec);
    
    verbPreDelay.setMaximumDelayInSamples(static_cast<int>(sampleRate * maxDelayTime / 1000.0f));
    verbPreDelay.prepare(spec);
    
    effectChain.get<1>().setMaximumDelayInSamples(529200); // TODO: EVALUATE THIS NUMBER LATER...

    effectChain.prepare(spec);
    
    verbHPF.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    verbLPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    delayHPF.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    delayLPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    
//    visualizer.clear();
}

void ProjectHaloAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ProjectHaloAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ProjectHaloAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto vHPF = apvts.getRawParameterValue("Reverb HPF")->load();
    auto vLPF = apvts.getRawParameterValue("Reverb LPF")->load();
    auto dHPF = apvts.getRawParameterValue("Delay HPF")->load();
    auto dLPF = apvts.getRawParameterValue("Delay LPF")->load();
    
    verbHPF.setCutoffFrequency(vHPF);
    verbLPF.setCutoffFrequency(vLPF);
    delayHPF.setCutoffFrequency(dHPF);
    delayLPF.setCutoffFrequency(dLPF);
    
//    DBG(vHPF);
    
    auto delayFB = apvts.getRawParameterValue("Feedback")->load();
    auto delay64 = apvts.getRawParameterValue("Delay64")->load();
    auto delay32 = apvts.getRawParameterValue("Delay32")->load();
    auto delay16 = apvts.getRawParameterValue("Delay16")->load();
    auto delay8 = apvts.getRawParameterValue("Delay8")->load();
    auto delay4 = apvts.getRawParameterValue("Delay4")->load();
    auto delay2 = apvts.getRawParameterValue("Delay2")->load();
    auto delay1 = apvts.getRawParameterValue("Delay1")->load();
    
    auto vRoomSize = apvts.getRawParameterValue("Room Size");
    auto vDamping = apvts.getRawParameterValue("Damping");
    auto vWidth = apvts.getRawParameterValue("Width");
    
    auto vDryWet = apvts.getRawParameterValue("Dry Wet");
    float wetLevel = vDryWet->load();
    float dryLevel = 1.0f - wetLevel;
    
    reverbParams.roomSize = vRoomSize -> load();
    reverbParams.damping = vDamping -> load();
    reverbParams.width = vWidth -> load();
    reverbParams.wetLevel = wetLevel;
    reverbParams.dryLevel = dryLevel;
    reverbParams.freezeMode = 0.0f;
    
    auto vPreDelay = apvts.getRawParameterValue("Pre Delay");
    float preDelayTimeMs = vPreDelay->load();
    float preDelayTimeSamples = getSampleRate() * preDelayTimeMs / 1000.0f;
    verbPreDelay.setDelay(preDelayTimeSamples);
    
//    auto vPreDelay = apvts.getRawParameterValue("Pre Delay")->load();
//    smoothedReverbPreDelayTime.setTargetValue(vPreDelay);
//    float preDelayTimeSamples = getSampleRate() * smoothedReverbPreDelayTime.getNextValue() / 1000.0f;
//    verbPreDelay.setDelay(preDelayTimeSamples);
    
    auto audioBlock = juce::dsp::AudioBlock<float> (buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
    
    
    if (reverbState)
    {
        effectChain.get<0>().setParameters(reverbParams);
        effectChain.setBypassed<0>(false);
        verbPreDelay.process(context);
        verbHPF.process(context);
        verbLPF.process(context);
    }
    else
    {
        effectChain.setBypassed<0>(true);
    }
    
    if (delayState)
    {
        effectChain.setBypassed<1>(false);
        float delayTime = 0.0f;
        
        if (delay64)
        {
            delayTime = calcDelayTime(4);
        }
        else if (delay32)
        {
            delayTime = calcDelayTime(3);
        }
        else if (delay16)
        {
            delayTime = calcDelayTime(2);
        }
        else if (delay8)
        {
            delayTime = calcDelayTime(1);
        }
        else if (delay4)
        {
            delayTime = calcDelayTime(0);
        }
        else if (delay2)
        {
            delayTime = calcDelayTime(-2);
        }
        else if (delay1)
        {
            delayTime = calcDelayTime(-4);
        }
        
//        smoothedDelayTime.setTargetValue(delayTime);
        auto delayToSamples = delayTime * (getSampleRate() / 1000.0f);

//        auto delayToSamples = smoothedDelayTime.getNextValue() * (getSampleRate() / 1000.0f);
        effectChain.get<1>().setDelay(delayToSamples);
        
        delayHPF.process(context);
        delayLPF.process(context);
    }
    else
    {
        effectChain.setBypassed<1>(true);
    }
    
    effectChain.process(context);
    
}

//==============================================================================
bool ProjectHaloAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ProjectHaloAudioProcessor::createEditor()
{
    return new ProjectHaloAudioProcessorEditor (*this);
}

//==============================================================================
void ProjectHaloAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ProjectHaloAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout ProjectHaloAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    float predelaySkew = 0.25f;
    float skewFactorLPF = 0.5f;
    float skewFactorHPF = 0.5f;
    auto roomSizeRange = juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f);
    auto preDelayRange = juce::NormalisableRange<float>(0.00f, 500.0f, 0.01f, predelaySkew);
    auto dampingRange = juce::NormalisableRange<float>(0.00f, 1.0f, 0.01f);
    auto widthRange = juce::NormalisableRange<float>(0.00f, 1.0f, 0.01f);
    auto HPFRange = juce::NormalisableRange<float>(20.0f, 2000.0f, 1.0f, skewFactorHPF);
    auto LPFRange = juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, skewFactorLPF);
    auto feedbackRange = juce::NormalisableRange<float>(0.0f, 200.0f, 1.0f);
    auto dryWetRange = juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f);
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Room Size", 1), "Room Size", roomSizeRange, 0.00f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Pre Delay", 1), "Pre Delay", preDelayRange, 0.00f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Damping", 1), "Damping", dampingRange, 0.00f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Width", 1), "Width", widthRange, 0.00f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Reverb HPF", 1), "Reverb HPF", HPFRange, 20.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Reverb LPF", 1), "Reverb LPF", LPFRange, 20000.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Feedback", 1), "Feedback", feedbackRange, 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Delay HPF", 1), "Delay HPF", HPFRange, 20.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Delay LPF", 1), "Delay LPF", LPFRange, 20000.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Dry Wet", 1), "Dry Wet", dryWetRange, 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Delay64", 1), "Delay64", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Delay32", 1), "Delay32", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Delay16", 1), "Delay16", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Delay8", 1), "Delay8", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Delay4", 1), "Delay4", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Delay2", 1), "Delay2", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Delay1", 1), "Delay1", false));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("distortionAmt" , 1), "Distortion Amount", 0, 100, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("cozyModeAmt" , 1), "Cozy Mode Amount", 0, 100, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("shifterAmt" , 1), "Shifter Amount", 0, 100, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("sickoModeAmt" , 1), "Sicko Amount", 0, 100, 0));
    
    return {params.begin(), params.end()};
}

void ProjectHaloAudioProcessor::reset()
{
    verbHPF.reset();
    verbLPF.reset();
    delayHPF.reset();
    delayLPF.reset();
    verbPreDelay.reset();
    effectChain.reset();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ProjectHaloAudioProcessor();
}
