#ifndef OCTAVE_EFFECT_H
#define OCTAVE_EFFECT_H

#include "IEffect.h"

class OctaveEffect : public IEffect 
{
    
public:
    OctaveEffect(bool higher = true, float blend = 0.5f);

    std::pair<float, float> process(std::pair<float, float> stereoSample) override;
    void reset() override;

    void setHigher(bool higher);
    void setBlend(float blend);
    void setFrequency(float frequency);
    void setSampleRate(float sampleRate);

private:
    bool m_higher;   // Whether the effect generates a higher or lower octave
    float m_blend;   // Blending factor between original and octave sample
    float m_phase;   // Phase for generating the octave wave
    float m_frequency;
    float m_sampleRate;
};

#endif // OCTAVE_EFFECT_H
