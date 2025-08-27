#include "DelayEffect.h"
#include <algorithm>

// -----------------------------------------------------------------------------
// DelayEffect implementation
// -----------------------------------------------------------------------------

DelayEffect::DelayEffect(float delayTime, float feedback, float mix, float sampleRate)
    : m_index(0), m_delayTime(delayTime), m_feedback(feedback), m_mix(mix), m_sampleRate(sampleRate)
{
    updateBufferSize();
}

std::pair<float, float> DelayEffect::process(std::pair<float, float> stereoSample)
{
    // Read the current delayed samples from the circular buffer
    float delayedLeft = m_bufferLeft[m_index];
    float delayedRight = m_bufferRight[m_index];

    // Write the new input along with feedback into the buffer
    m_bufferLeft[m_index] = stereoSample.first + delayedLeft * m_feedback;
    m_bufferRight[m_index] = stereoSample.second + delayedRight * m_feedback;

    // Blend dry and wet signals according to mix
    float outLeft = (1.0f - m_mix) * stereoSample.first + m_mix * delayedLeft;
    float outRight = (1.0f - m_mix) * stereoSample.second + m_mix * delayedRight;

    // Advance the buffer index and wrap around if needed
    m_index++;
    if (m_index >= m_bufferLeft.size())
        m_index = 0;

    return {outLeft, outRight};
}

void DelayEffect::reset()
{
    std::fill(m_bufferLeft.begin(), m_bufferLeft.end(), 0.0f);
    std::fill(m_bufferRight.begin(), m_bufferRight.end(), 0.0f);
    m_index = 0;
}

void DelayEffect::setSampleRate(float sampleRate)
{
    if (sampleRate > 0.0f) {
        m_sampleRate = sampleRate;
        updateBufferSize();
    }
}

void DelayEffect::setDelayTime(float delayTime)
{
    // Limit delay time to reasonable range (0.001s to 5s)
    if (delayTime >= 0.001f && delayTime <= 5.0f) {
        m_delayTime = delayTime;
        updateBufferSize();
    }
}

void DelayEffect::setFeedback(float feedback)
{
    // Clamp feedback to prevent runaway feedback
    m_feedback = std::min(std::max(feedback, 0.0f), 0.95f);
}

void DelayEffect::setMix(float mix)
{
    // Clamp mix to valid range
    m_mix = std::min(std::max(mix, 0.0f), 1.0f);
}

// Resize the delay buffers whenever parameters change
void DelayEffect::updateBufferSize()
{
    unsigned int size = static_cast<unsigned int>(m_delayTime * m_sampleRate);
    size = std::max(1u, size);
    m_bufferLeft.assign(size, 0.0f);
    m_bufferRight.assign(size, 0.0f);
    m_index = 0; // reset buffer position
}
