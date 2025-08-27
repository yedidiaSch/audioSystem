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
    if (sampleRate > 0.0f) {
        m_sampleRate = sampleRate;
        updateAlpha();
    }
}

void LowPassEffect::setCutoff(float cutoff)
{
    // Validate cutoff frequency (should be positive and below Nyquist frequency)
    if (cutoff > 0.0f && cutoff < m_sampleRate * 0.5f) {
        m_cutoff = cutoff;
        updateAlpha();
    }
}

void LowPassEffect::updateAlpha()
{
    // Precompute filter coefficient from cutoff frequency
    if (m_sampleRate > 0.0f && m_cutoff > 0.0f) {
        float dt = 1.0f / m_sampleRate;
        float rc = 1.0f / (2.0f * M_PI * m_cutoff);
        m_alpha = dt / (rc + dt);
        
        // Clamp alpha to valid range [0, 1]
        if (m_alpha > 1.0f) m_alpha = 1.0f;
        if (m_alpha < 0.0f) m_alpha = 0.0f;
    }
}
