#include "SawtoothWave.h"

// -----------------------------------------------------------------------------
// SawtoothWave implementation
// -----------------------------------------------------------------------------

SawtoothWave::SawtoothWave() {}
SawtoothWave::~SawtoothWave() {}

float SawtoothWave::generate(float frequency, float sampleRate, float& phase)
{
    // Sawtooth is a simple ramp from -1 to 1
    float sample = 2.0f * phase - 1.0f;
    phase += frequency / sampleRate;
    if (phase >= 1.0f)
        phase -= 1.0f;
    return sample;
}

void SawtoothWave::reset()
{
    // no state to reset
}
