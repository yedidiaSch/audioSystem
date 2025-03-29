#ifndef SINE_WAVE_H
#define SINE_WAVE_H

#include "../IWave.h"
#include <cmath>

class SineWave : public IWave {
public:
    float generate(float frequency, float sampleRate, float& phase) override {
        float sample = std::sin(2.0f * M_PI * phase);
        phase += frequency / sampleRate;
        if (phase >= 1.0f) phase -= 1.0f;
        return sample;
    }
    
    void reset() override {
        // Nothing to reset for stateless sine wave
    }
};

#endif // SINE_WAVE_H