#include "SquareWave.h"

SquareWave::SquareWave()
{
    // Initialize any member variables here if needed
}

SquareWave::~SquareWave()
{
    // Cleanup resources if needed
}

float SquareWave::generate(float frequency, float sampleRate, float& phase)
{
    // Generate a square wave sample
    float sample = (phase < 0.5f) ? 1.0f : -1.0f;

    // Update the phase for the square wave
    phase += frequency / sampleRate;
    if (phase >= 1.0f) {
        phase -= 1.0f;
    }

    return sample;
}

void SquareWave::reset()
{
    // No state to reset for a simple square wave
    // If we add state variables later, reset them here
}