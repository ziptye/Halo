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

//==============================================================================
void ProjectHaloAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    reset(); // resets all parameters of the plugin
    
    verbHPF.prepare(spec);
    verbLPF.prepare(spec);
    delayHPF.prepare(spec);
    delayLPF.prepare(spec);
    
    verbPreDelay.setMaximumDelayInSamples(static_cast<int>(sampleRate * maxDelayTime / 1000.0f));
    verbPreDelay.prepare(spec);
    reverb.prepare(spec);
    
    verbHPF.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    verbLPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    delayHPF.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    delayLPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
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

    auto vHPF = apvts.getRawParameterValue("Reverb HPF");
    auto vLPF = apvts.getRawParameterValue("Reverb LPF");
    auto dHPF = apvts.getRawParameterValue("Delay HPF");
    auto dLPF = apvts.getRawParameterValue("Delay LPF");
    
    verbHPF.setCutoffFrequency(vHPF -> load());
    verbLPF.setCutoffFrequency(vLPF -> load());
    delayHPF.setCutoffFrequency(dHPF -> load());
    delayLPF.setCutoffFrequency(dLPF -> load());
    
    auto vRoomSize = apvts.getRawParameterValue("Room Size");
    auto vDamping = apvts.getRawParameterValue("Damping");
    auto vWidth = apvts.getRawParameterValue("Width");
    
    reverbParams.roomSize = vRoomSize -> load();
    reverbParams.damping = vDamping -> load();
    reverbParams.width = vWidth -> load();
    
    auto vDryWet = apvts.getRawParameterValue("Dry Wet");
    float wetLevel = vDryWet->load();
    float dryLevel = 1.0f - wetLevel;
    
    reverbParams.wetLevel = wetLevel;
    reverbParams.dryLevel = dryLevel;
    
    reverbParams.freezeMode = 0.0f;
    
    reverb.setParameters(reverbParams);
    
    auto vPreDelay = apvts.getRawParameterValue("Pre Delay");
    float preDelayTimeMs = vPreDelay->load();
    float preDelayTimeSamples = getSampleRate() * preDelayTimeMs / 1000.0f;
    verbPreDelay.setDelay(preDelayTimeSamples);
    
    auto audioBlock = juce::dsp::AudioBlock<float> (buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
    
    verbPreDelay.process(context);
    verbHPF.process(context);
    verbLPF.process(context);
    delayHPF.process(context);
    delayLPF.process(context);
    reverb.process(context);
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
    float skewFactorHPF = 0.25f;
    auto roomSizeRange = juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f);
    auto preDelayRange = juce::NormalisableRange<float>(0.00f, 500.0f, 0.01f, predelaySkew);
    auto dampingRange = juce::NormalisableRange<float>(0.00f, 1.0f, 0.01f);
    auto widthRange = juce::NormalisableRange<float>(0.00f, 1.0f, 0.01f);
    auto HPFRange = juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, skewFactorHPF);
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
    
    return {params.begin(), params.end()};
}

void ProjectHaloAudioProcessor::reset()
{
    verbHPF.reset();
    verbLPF.reset();
    delayHPF.reset();
    delayLPF.reset();
    reverb.reset();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ProjectHaloAudioProcessor();
}
