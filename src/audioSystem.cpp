#include <cmath>
#include <algorithm> // For std::find
#include "audioSystem.h"
#include "Waves/SquareWave.h" // Include the square wave implementation
#include "Waves/SineWave.h"
#include "Waves/SawtoothWave.h"
#include "Waves/TriangleWave.h"
#include "Effects/OctaveEffect.h"
#include "Effects/DelayEffect.h"
#include "Effects/LowPassEffect.h"

AudioSystem::AudioSystem(float sampleRate) : m_frequency(0.0f),
                                             m_sampleRate(sampleRate),
                                             m_phase(0.0f),
                                             m_noteOn(false) 
{
    // Default to square wave
    m_waveform = std::make_shared<SquareWave>();
}

void AudioSystem::setWaveform(std::shared_ptr<IWave> waveform)
{
    m_waveform = waveform;
}

// Configure the oscillator and effects based on the provided AudioConfig
void AudioSystem::configure(const AudioConfig& config)
{
    // Select waveform
    if (config.waveform == "sine")
        m_waveform = std::make_shared<SineWave>();
    else if (config.waveform == "sawtooth")
        m_waveform = std::make_shared<SawtoothWave>();
    else if (config.waveform == "triangle")
        m_waveform = std::make_shared<TriangleWave>();
    else
        m_waveform = std::make_shared<SquareWave>();

    // Clear existing effects
    m_effects.clear();

    // Instantiate effects listed in the configuration
    for (const auto& name : config.effects)
    {
        if (name == "octave")
        {
            auto eff = std::make_shared<OctaveEffect>();
            m_effects.push_back(eff);
        }
        else if (name == "delay")
        {
            auto eff = std::make_shared<DelayEffect>(0.3f, 0.5f, 0.5f, m_sampleRate);
            m_effects.push_back(eff);
        }
        else if (name == "lowpass")
        {
            auto eff = std::make_shared<LowPassEffect>(1000.0f, m_sampleRate);
            m_effects.push_back(eff);
        }
    }
}

void AudioSystem::triggerNote(float newFrequency)
{
    m_frequency = newFrequency;
    m_noteOn = true;
    m_phase = 0.0f;

    for (auto& effect : m_effects)
    {
        if (auto octave = std::dynamic_pointer_cast<OctaveEffect>(effect))
        {
            octave->setFrequency(newFrequency);
            octave->setSampleRate(m_sampleRate);
        }
        else if (auto delay = std::dynamic_pointer_cast<DelayEffect>(effect))
        {
            delay->setSampleRate(m_sampleRate);
        }
        else if (auto lp = std::dynamic_pointer_cast<LowPassEffect>(effect))
        {
            lp->setSampleRate(m_sampleRate);
        }
    }

    resetEffects();
}

void AudioSystem::triggerNoteOff() 
{
    m_noteOn = false;
}

std::pair<float, float> AudioSystem::getNextSample() 
{
    if (!m_noteOn || !m_waveform) 
    {
        return {0.0f, 0.0f};
    }

    // Generate a sample using the waveform generator
    float sample = m_waveform->generate(m_frequency, m_sampleRate, m_phase);

    // Create a stereo sample (initially identical in both channels)
    std::pair<float, float> stereoSample{sample, sample};

    // Apply effects to the stereo sample
    stereoSample = applyEffects(stereoSample);

    return stereoSample;
}

std::pair<float, float> AudioSystem::applyEffects(std::pair<float, float> stereoSample) 
{
    // Apply each effect in the chain to the stereo sample
    for (auto& effect : m_effects) 
    {
        stereoSample = effect->process(stereoSample);
    }

    return stereoSample;
}


void AudioSystem::addEffect(std::shared_ptr<IEffect> effect) 
{
    // Check if the effect already exists in the vector
    auto it = std::find(m_effects.begin(), m_effects.end(), effect);
    
    if (it == m_effects.end()) 
    {
        // Effect not found, so add it to the vector
        m_effects.push_back(effect);
    } 
}


void AudioSystem::resetEffects() 
{
    for (auto& effect : m_effects) 
    {
        effect->reset();
    }
}
