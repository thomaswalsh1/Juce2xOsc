/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
_2xOscSynthAudioProcessor::_2xOscSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    pointers.attach(apvts);
    pointers.load(params);

    synth.addSound(new SynthSound());
    for (int i = 0; i < SynthConstants::numVoices; i++) {
        synth.addVoice(new SynthVoice(params, synth));
    }
}

_2xOscSynthAudioProcessor::~_2xOscSynthAudioProcessor()
{
}

//==============================================================================
const juce::String _2xOscSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _2xOscSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _2xOscSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _2xOscSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _2xOscSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _2xOscSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _2xOscSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _2xOscSynthAudioProcessor::setCurrentProgram (int)
{
}

const juce::String _2xOscSynthAudioProcessor::getProgramName (int)
{
    return {};
}

void _2xOscSynthAudioProcessor::changeProgramName (int, const juce::String&)
{
}

//==============================================================================
void _2xOscSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    // voices start from the current parameter values rather than ramping in
    pointers.load(params);
    lastMasterGain = juce::Decibels::decibelsToGain(params.masterGainDb);

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            voice->prepareToPlay(sampleRate, samplesPerBlock);
    }
}

void _2xOscSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _2xOscSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void _2xOscSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // voices read this snapshot while rendering
    pointers.load(params);

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // ramp across the block so master gain changes don't click
    const float masterGain = juce::Decibels::decibelsToGain(params.masterGainDb);
    buffer.applyGainRamp(0, buffer.getNumSamples(), lastMasterGain, masterGain);
    lastMasterGain = masterGain;
}

//==============================================================================
bool _2xOscSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _2xOscSynthAudioProcessor::createEditor()
{
    return new _2xOscSynthAudioProcessorEditor (*this);
}

//==============================================================================
void _2xOscSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void _2xOscSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _2xOscSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout _2xOscSynthAudioProcessor::createParameterLayout()
{
    using Float = juce::AudioParameterFloat;
    using Range = juce::NormalisableRange<float>;
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // carriers
    const std::pair<juce::String, juce::String> carriers[] = {
        { ParamIDs::osc1, "Osc 1" },
        { ParamIDs::osc2, "Osc 2" },
    };
    for (const auto& [prefix, name] : carriers) {
        layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ prefix + ParamIDs::wave, 1 }, name + " Wave", waveformNames, 0));
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{ prefix + ParamIDs::coarse, 1 }, name + " Coarse", -36, 36, 0));
        layout.add(std::make_unique<Float>(juce::ParameterID{ prefix + ParamIDs::fine, 1 }, name + " Fine", -50.0f, 50.0f, 0.0f));
    }

    // envelopes, all share the same ranges
    auto addEnvelope = [&layout](const juce::String& prefix, const juce::String& name, EnvelopeParameters defaults) {
        const Range timeRange(0.001f, 5.0f, 0.001f, 0.4f);
        layout.add(std::make_unique<Float>(juce::ParameterID{ prefix + ParamIDs::attack, 1 }, name + " Attack", timeRange, defaults.attack));
        layout.add(std::make_unique<Float>(juce::ParameterID{ prefix + ParamIDs::decay, 1 }, name + " Decay", timeRange, defaults.decay));
        layout.add(std::make_unique<Float>(juce::ParameterID{ prefix + ParamIDs::sustain, 1 }, name + " Sustain", 0.0f, 1.0f, defaults.sustain));
        layout.add(std::make_unique<Float>(juce::ParameterID{ prefix + ParamIDs::release, 1 }, name + " Release", timeRange, defaults.release));
    };
    addEnvelope(ParamIDs::amp, "Amp", {});
    addEnvelope(ParamIDs::filterEnv, "Filter Env", { 0.005f, 0.3f, 0.0f, 0.1f });
    addEnvelope(ParamIDs::fm, "FM", {});

    // fm modulator
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ ParamIDs::fm + ParamIDs::wave, 1 }, "FM Wave", waveformNames, 0));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::fm + ParamIDs::fine, 1 }, "FM Fine", -50.0f, 50.0f, 0.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::fmRatio, 1 }, "FM Ratio", Range(0.25f, 16.0f, 0.01f), 1.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::fmIndex, 1 }, "FM Index", Range(0.0f, 10.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ ParamIDs::fmToOsc1, 1 }, "FM to Osc 1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ ParamIDs::fmToOsc2, 1 }, "FM to Osc 2", false));

    // filter
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ ParamIDs::filterType, 1 }, "Filter Type", filterTypeNames, 0));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::filterCutoff, 1 }, "Filter Cutoff", Range(20.0f, 20000.0f, 0.1f, 0.25f), 20000.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::filterResonance, 1 }, "Filter Resonance", Range(0.3f, 10.0f, 0.01f, 0.4f), 0.707f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::filterEnvAmount, 1 }, "Filter Env Amount", Range(-4.0f, 4.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::filterKeytrack, 1 }, "Filter Keytrack", Range(0.0f, 1.0f, 0.01f), 0.0f));

    // lfo
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ ParamIDs::lfoWave, 1 }, "LFO Wave", lfoWaveformNames, 0));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::lfoRate, 1 }, "LFO Rate", Range(0.05f, 20.0f, 0.01f, 0.3f), 5.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::lfoToPitch, 1 }, "LFO to Pitch", Range(0.0f, 12.0f, 0.01f, 0.4f), 0.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::lfoToCutoff, 1 }, "LFO to Cutoff", Range(0.0f, 4.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::lfoToFmIndex, 1 }, "LFO to FM Index", Range(0.0f, 10.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::lfoModWheel, 1 }, "LFO Mod Wheel", Range(0.0f, 1.0f, 0.01f), 0.0f));

    // velocity
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::velToAmp, 1 }, "Velocity to Amp", Range(0.0f, 1.0f, 0.01f), 1.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::velToCutoff, 1 }, "Velocity to Cutoff", Range(0.0f, 4.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::velToFmIndex, 1 }, "Velocity to FM Index", Range(0.0f, 10.0f, 0.01f), 0.0f));

    // global
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::oscMix, 1 }, "Osc Mix", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::glideTime, 1 }, "Glide Time", Range(0.0f, 2.0f, 0.001f, 0.4f), 0.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{ ParamIDs::masterGain, 1 }, "Master Gain", Range(-36.0f, 6.0f, 0.1f), 0.0f));

    return layout;
}
