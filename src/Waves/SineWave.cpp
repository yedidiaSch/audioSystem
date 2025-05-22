#include "SineWave.h"
#include <cmath>

// Define PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SineWave::SineWave()
{
    // No initialization needed for sine wave
}

SineWave::~SineWave()
{
    // No cleanup needed
}

float SineWave::generate(float frequency, float sampleRate, float& phase)
{
    // Generate a sine wave sample using the current phase
    // sin(2π * phase) gives us a value between -1.0 and 1.0
    float sample = std::sin(2.0f * M_PI * phase);
    
    // Update the phase for the next sample
    // The phase increment is frequency/sampleRate
    // This gives us exactly one full cycle (0 to 1) at the requested frequency
    phase += frequency / sampleRate;
    
    // Wrap the phase back to [0, 1) range if it exceeds 1.0
    // This is more efficient than using fmod
    if (phase >= 1.0f) {
        phase -= 1.0f;
    }
    
    return sample;
}

void SineWave::reset()
{
    // Sine wave generation is stateless, nothing to reset
}
