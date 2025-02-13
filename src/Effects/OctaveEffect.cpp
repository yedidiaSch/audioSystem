#include "OctaveEffect.h"

OctaveEffect::OctaveEffect(bool higher, float blend)
    : m_higher(higher), m_blend(blend), m_phase(0.0f), m_frequency(0.0f), m_sampleRate(0.0f) 
{
}

std::pair<float, float> OctaveEffect::process(std::pair<float, float> stereoSample) 
{
    if (m_frequency <= 0.0f || m_sampleRate <= 0.0f) 
    {
        return stereoSample; // If invalid frequency or sample rate, pass input unchanged
    }

    // Calculate the octave frequency
    float octaveFrequency = m_frequency * (m_higher ? 2.0f : 0.5f);

    // Generate the octave sample
    float octaveSample = (m_phase < 0.5f) ? 1.0f : -1.0f;

    // Update the phase for the octave wave
    m_phase += octaveFrequency / m_sampleRate;
    if (m_phase >= 1.0f) 
    {
        m_phase -= 1.0f;
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
    m_blend = (blend < 0.0f) ? 0.0f : (blend > 1.0f) ? 1.0f : blend;
}

void OctaveEffect::setFrequency(float frequency) 
{
    m_frequency = frequency;
}

void OctaveEffect::setSampleRate(float sampleRate) 
{
    m_sampleRate = sampleRate;
}
