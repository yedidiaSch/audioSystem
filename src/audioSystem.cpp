#include <cmath>
#include <algorithm> // For std::find
#include "audioSystem.h"
#include "Waves/SquareWave.h" // Include the square wave implementation

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

void AudioSystem::triggerNote(float newFrequency) 
{
    m_frequency = newFrequency;
    m_noteOn = true;
    m_phase = 0.0f;

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
