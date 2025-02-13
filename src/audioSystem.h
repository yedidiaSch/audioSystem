#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <vector>
#include <memory>
#include <utility>
#include "Effects/IEffect.h"


class AudioSystem {
public:
    explicit AudioSystem(float sampleRate);

    void triggerNote(float newFrequency);
    void triggerNoteOff();

    // Generate the next stereo audio sample
    std::pair<float, float> getNextSample();

    // Add an effect to the processing chain
    void addEffect(std::shared_ptr<IEffect> effect);

    // Apply all effects to a stereo sample
    std::pair<float, float> applyEffects(std::pair<float, float> stereoSample);

    void resetEffects();

private:
    float m_frequency;       // Frequency of the note being played
    float m_sampleRate;      // Sample rate of the audio system
    float m_phase;           // Phase for generating the primary wave
    bool m_noteOn;           // Indicates if a note is currently being played

    std::vector<std::shared_ptr<IEffect>> m_effects; // List of effects in the chain
};

#endif // AUDIOSYSTEM_H
