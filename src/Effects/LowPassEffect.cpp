#include "LowPassEffect.h"
#include <cmath>

// -----------------------------------------------------------------------------
// LowPassEffect implementation
// -----------------------------------------------------------------------------

LowPassEffect::LowPassEffect(float cutoff, float sampleRate)
    : m_cutoff(cutoff), m_sampleRate(sampleRate), m_stateL(0.0f), m_stateR(0.0f)
{
    updateAlpha();
}

std::pair<float, float> LowPassEffect::process(std::pair<float, float> stereoSample)
{
    // Apply simple one-pole low-pass filter to each channel
    m_stateL = m_stateL + m_alpha * (stereoSample.first - m_stateL);
    m_stateR = m_stateR + m_alpha * (stereoSample.second - m_stateR);
    return {m_stateL, m_stateR};
}

void LowPassEffect::reset()
{
    m_stateL = 0.0f;
    m_stateR = 0.0f;
}

void LowPassEffect::setSampleRate(float sampleRate)
{
    m_sampleRate = sampleRate;
    updateAlpha();
}

void LowPassEffect::setCutoff(float cutoff)
{
    m_cutoff = cutoff;
    updateAlpha();
}

void LowPassEffect::updateAlpha()
{
    // Precompute filter coefficient from cutoff frequency
    float dt = 1.0f / m_sampleRate;
    float rc = 1.0f / (2.0f * M_PI * m_cutoff);
    m_alpha = dt / (rc + dt);
}
