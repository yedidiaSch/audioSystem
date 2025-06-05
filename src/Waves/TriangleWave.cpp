#include "TriangleWave.h"

// -----------------------------------------------------------------------------
// TriangleWave implementation
// -----------------------------------------------------------------------------

TriangleWave::TriangleWave() {}
TriangleWave::~TriangleWave() {}

float TriangleWave::generate(float frequency, float sampleRate, float& phase)
{
    // Piecewise linear ramp creating a symmetric triangle
    float sample = phase < 0.5f ? 4.0f * phase - 1.0f : -4.0f * phase + 3.0f;
    phase += frequency / sampleRate;
    if (phase >= 1.0f)
        phase -= 1.0f;
    return sample;
}

void TriangleWave::reset()
{
    // no state to reset
}
