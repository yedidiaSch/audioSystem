#include "OctaveEffect.h"
#include <cmath>
#include <algorithm>

OctaveEffect::OctaveEffect(bool higher, float blend)
    : m_higher(higher), 
      m_blend(std::min(std::max(blend, 0.0f), 1.0f)), 
      m_phase(0.0f), 
      m_frequency(0.0f), 
      m_sampleRate(44100.0f) // Default sample rate
{
}

std::pair<float, float> OctaveEffect::process(std::pair<float, float> stereoSample) 
{
    // Early return if invalid parameters
    if (m_frequency <= 0.0f || m_sampleRate <= 0.0f) 
    {
        return stereoSample;
    }

    // Calculate the octave frequency
    float octaveFrequency = m_frequency * (m_higher ? 2.0f : 0.5f);

    // Generate a simple square wave for the octave
    // Could be improved with band-limited synthesis to reduce aliasing
    float octaveSample = (m_phase < 0.5f) ? 1.0f : -1.0f;

    // Update the phase for the octave wave
    float phaseIncrement = octaveFrequency / m_sampleRate;
    m_phase += phaseIncrement;
    
    // Wrap phase to [0, 1) range
    if (m_phase >= 1.0f) 
    {
        m_phase = std::fmod(m_phase, 1.0f);
    }

    // Blend the original and octave samples
    float leftChannel = (1.0f - m_blend) * stereoSample.first + m_blend * octaveSample;
    float rightChannel = (1.0f - m_blend) * stereoSample.second + m_blend * octaveSample;

    return {leftChannel, rightChannel};
}

void OctaveEffect::reset() 
{
    m_phase = 0.0f;
}

void OctaveEffect::setHigher(bool higher) 
{
    m_higher = higher;
}

void OctaveEffect::setBlend(float blend) 
{
    m_blend = std::min(std::max(blend, 0.0f), 1.0f);
}

void OctaveEffect::setFrequency(float frequency) 
{
    // Validate frequency range
    if (frequency > 0.0f && frequency <= 20000.0f) {
        m_frequency = frequency;
    }
}

void OctaveEffect::setSampleRate(float sampleRate) 
{
    // Validate sample rate
    if (sampleRate > 0.0f) {
        m_sampleRate = sampleRate;
    }
}
