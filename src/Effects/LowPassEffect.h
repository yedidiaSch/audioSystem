#pragma once
#include "IEffect.h"

/**
 * @brief Simple first-order low-pass filter
 *
 * The filter smooths out high-frequency components using an exponential
 * moving average. The cutoff frequency determines how quickly the signal
 * responds to changes.
 */
class LowPassEffect : public IEffect
{
public:
    /**
     * @brief Construct a LowPassEffect
     * @param cutoff     Cutoff frequency in Hertz
     * @param sampleRate Sampling rate used for processing
     */
    LowPassEffect(float cutoff = 1000.0f, float sampleRate = 44100.0f);

    /// Process a stereo sample through the filter
    std::pair<float, float> process(std::pair<float, float> stereoSample) override;
    /// Reset internal filter state
    void reset() override;

    /// Update the sampling rate
    void setSampleRate(float sampleRate);
    /// Set the cutoff frequency
    void setCutoff(float cutoff);

private:
    float m_cutoff;      ///< Current cutoff frequency
    float m_sampleRate;  ///< System sampling rate
    float m_alpha;       ///< Filter coefficient
    float m_stateL;      ///< Previous left sample
    float m_stateR;      ///< Previous right sample

    /** Recalculate the filter coefficient based on cutoff and sampleRate */
    void updateAlpha();
};
