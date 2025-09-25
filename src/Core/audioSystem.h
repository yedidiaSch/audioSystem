#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "Effects/IEffect.h"
#include "Waves/IWave.h"
#include "AudioConfig.h"

/**
 * @file audioSystem.h
 * @brief Main audio processing system that handles sound generation and effects processing
 * 
 * This class provides functionality for generating simple audio tones and
 * processing them through a chain of audio effects.
 */

/**
 * @class AudioSystem
 * @brief Core audio processing class that generates tones and applies effects
 * 
 * The AudioSystem class handles the generation of simple audio tones based on
 * frequency input and processes them through a configurable chain of audio effects.
 * It provides interfaces for triggering notes, managing effects, and retrieving
 * processed audio samples.
 */
class AudioSystem
{
public:
    /**
     * @brief Constructs an AudioSystem with the specified sample rate
     * @param sampleRate The number of samples per second (Hz)
     */
    explicit AudioSystem(float sampleRate);

    /**
     * @brief Triggers a note with the specified frequency
     * @param newFrequency The frequency in Hz of the note to play
     */
    void triggerNote(float newFrequency);

    /**
     * @brief Stops the currently playing note
     */
    void triggerNoteOff();

    /**
     * @brief Calculates and returns the next stereo audio sample
     * @return A pair of floats representing the left and right channel values
     */
    std::pair<float, float> getNextSample();

    /**
     * @brief Adds an audio effect to the processing chain
     * @param effect Shared pointer to an effect implementing the IEffect interface
     */
    void addEffect(std::shared_ptr<IEffect> effect);

    /**
     * @brief Processes a stereo sample through all added effects
     * @param stereoSample Input stereo sample (left and right channels)
     * @return The processed stereo sample after applying all effects
     */
    std::pair<float, float> applyEffects(std::pair<float, float> stereoSample);

    /**
     * @brief Resets all effects to their initial state
     */
    void resetEffects();

    /**
     * @brief Sets the waveform generator
     * @param waveform Shared pointer to a waveform generator implementing the IWave interface
     */
    void setWaveform(std::shared_ptr<IWave> waveform);

    /**
     * @brief Apply a configuration to choose waveform and effect chain
     *
     * The configuration structure contains the name of the desired waveform
     * and an ordered list of effect identifiers. Any existing effects are
     * cleared and replaced based on this information.
     */
    void configure(const AudioConfig& config);

private:
    float m_frequency;                                ///< Current note frequency in Hz
    float m_sampleRate;                               ///< Audio sample rate in Hz
    float m_phase;                                    ///< Current phase of the oscillator (0.0 to 1.0)
    bool m_noteOn;                                    ///< Flag indicating whether a note is currently playing
    std::vector<std::shared_ptr<IEffect>> m_effects;  ///< Chain of audio effects to apply
    std::shared_ptr<IWave> m_waveform;                ///< Waveform generator
};
