#pragma once
#include "IEffect.h"
#include <vector>

/**
 * @brief Simple delay line effect with feedback
 *
 * This effect stores past samples in a circular buffer to create an echo.
 * The @p feedback parameter determines how much of the delayed signal is
 * fed back into the buffer, while @p mix controls the wet/dry ratio.
 */
class DelayEffect : public IEffect
{
public:
    /**
     * @brief Construct a DelayEffect with common parameters
     * @param delayTime  Delay time in seconds
     * @param feedback   Amount of delayed signal fed back [0.0 - 1.0]
     * @param mix        Blend between dry (0.0) and wet (1.0) signal
     * @param sampleRate Sampling rate of the audio system
     */
    DelayEffect(float delayTime = 0.3f, float feedback = 0.5f, float mix = 0.5f,
                float sampleRate = 44100.0f);

    /** Process a stereo sample and return the delayed result */
    std::pair<float, float> process(std::pair<float, float> stereoSample) override;
    /** Reset the internal delay buffer */
    void reset() override;

    /// Change the sampling rate and resize the buffer accordingly
    void setSampleRate(float sampleRate);
    /// Set the delay time in seconds
    void setDelayTime(float delayTime);
    /// Set the feedback level [0.0 - 1.0]
    void setFeedback(float feedback);
    /// Set the wet/dry mix [0.0 - 1.0]
    void setMix(float mix);

private:
    std::vector<float> m_bufferLeft;  ///< Circular buffer for left channel
    std::vector<float> m_bufferRight; ///< Circular buffer for right channel
    unsigned int m_index;             ///< Current write/read index in buffers
    float m_delayTime;                ///< Delay time in seconds
    float m_feedback;                 ///< Feedback amount
    float m_mix;                      ///< Wet/dry mix
    float m_sampleRate;               ///< Current sampling rate

    /** Resize internal buffers based on delay time and sample rate */
    void updateBufferSize();
};
