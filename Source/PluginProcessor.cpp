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
    // load params
    


    synth.addSound(new SynthSound());
    for (int i = 0; i < NUM_VOICES; i++) {
        synth.addVoice(new SynthVoice());
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

void _2xOscSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _2xOscSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void _2xOscSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _2xOscSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    // initialize voices
    for (int i = 0; i < NUM_VOICES; i++) {
        SynthVoice * voice = dynamic_cast<SynthVoice *> (synth.getVoice(i));
        voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
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

    // parameters
    OscParameters osc1, osc2;
	osc1.wave = apvts.getRawParameterValue("OSC1_WAVE")->load();
	osc2.wave = apvts.getRawParameterValue("OSC2_WAVE")->load();
	osc1.coarse = apvts.getRawParameterValue("OSC1_COARSE")->load();
	osc2.coarse = apvts.getRawParameterValue("OSC2_COARSE")->load();
	osc1.fine = apvts.getRawParameterValue("OSC1_FINE")->load();
	osc2.fine = apvts.getRawParameterValue("OSC2_FINE")->load();

    // adsr
	osc1.attack = apvts.getRawParameterValue("OSC1_ATTACK")->load();
	osc2.attack = apvts.getRawParameterValue("OSC2_ATTACK")->load();
	osc1.decay = apvts.getRawParameterValue("OSC1_DECAY")->load();
	osc2.decay = apvts.getRawParameterValue("OSC2_DECAY")->load();
	osc1.sustain = apvts.getRawParameterValue("OSC1_SUSTAIN")->load();
	osc2.sustain = apvts.getRawParameterValue("OSC2_SUSTAIN")->load();
	osc1.release = apvts.getRawParameterValue("OSC1_RELEASE")->load();
	osc2.release = apvts.getRawParameterValue("OSC2_RELEASE")->load();


    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	for (int i = 0; i < synth.getNumVoices(); i++) {
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
			voice->setOscParameters(0, osc1);
			voice->setOscParameters(1, osc2);
		}
	}
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void _2xOscSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _2xOscSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout _2xOscSynthAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // parameters

    layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{ "OSC1_WAVE", 1 }, "Osc 1 Wave", 0, 2, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{ "OSC2_WAVE", 1 }, "Osc 2 Wave", 0, 2, 0));

    layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{ "OSC1_COARSE", 1 }, "Osc 1 Coarse", -36, 36, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{ "OSC2_COARSE", 1 }, "Osc 2 Coarse", -36, 36, 0));
	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC1_FINE", 1 }, "Osc 1 Fine", -50.0f, 50.0f, 0.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC2_FINE", 1 }, "Osc 2 Fine", -50.0f, 50.0f, 0.0f));

	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC1_ATTACK", 1 }, "Osc 1 Attack", 0.001f, 5.0f, 0.005f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC2_ATTACK", 1 }, "Osc 2 Attack", 0.001f, 5.0f, 0.005f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC1_DECAY", 1 }, "Osc 1 Decay", 0.001f, 5.0f, 0.1f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC2_DECAY", 1 }, "Osc 2 Decay", 0.001f, 5.0f, 0.1f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC1_SUSTAIN", 1 }, "Osc 1 Sustain", 0.0f, 1.0f, 0.3f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC2_SUSTAIN", 1 }, "Osc 2 Sustain", 0.0f, 1.0f, 0.3f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC1_RELEASE", 1 }, "Osc 1 Release", 0.001f, 5.0f, 0.015f));
	layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "OSC2_RELEASE", 1 }, "Osc 2 Release", 0.001f, 5.0f, 0.015f));

    return layout;
}
