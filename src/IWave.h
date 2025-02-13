#pragma once

class IWave 
{

public:

    virtual ~IWave() = default;

    // Generate the next sample for the waveform
    virtual float generate(float frequency, float sampleRate, float& phase) = 0;

    // Reset the state of the waveform
    virtual void reset() = 0;
    
};
