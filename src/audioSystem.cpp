#include <cmath>
#include <algorithm> // For std::find
#include "audioSystem.h"

AudioSystem::AudioSystem(float sampleRate) : m_frequency(0.0f),
                                             m_sampleRate(sampleRate),
                                             m_phase(0.0f),
                                             m_noteOn(false) 
{
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
    if (!m_noteOn) 
    {
        return {0.0f, 0.0f};
    }

    // Generate a square wave sample
    float sample = (m_phase < 0.5f) ? 1.0f : -1.0f;

    // Update the phase for the square wave
    m_phase += m_frequency / m_sampleRate;
    if (m_phase >= 1.0f) 
    {
        m_phase -= 1.0f;
    }

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
