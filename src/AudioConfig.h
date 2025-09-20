#pragma once

#include <string>
#include <vector>

/**
 * @file AudioConfig.h
 * @brief Configuration structure for the audio system
 */

/**
 * @brief Configuration options for selecting waveform and effects
 */
struct AudioConfig
{
    std::string waveform;               ///< Name of the oscillator to use
    std::vector<std::string> effects;   ///< Ordered list of effect names
    float sampleRate;                   ///< Audio sample rate in Hz
    unsigned int bufferFrames;          ///< Number of frames per audio buffer
    int midiPort;                       ///< MIDI port number
    float defaultFrequency;             ///< Default frequency for testing (Hz)
    std::string inputMode;              ///< Input mode: "midi" or "sequencer" for testing
    std::string sequenceType;           ///< Type of sequence for sequencer mode
    
    // Default constructor with sensible defaults
    AudioConfig() : 
        waveform("sine"),
        sampleRate(44100.0f),
        bufferFrames(512),
        midiPort(1),
        defaultFrequency(440.0f),
        inputMode("midi"),
        sequenceType("demo")
    {}
};
