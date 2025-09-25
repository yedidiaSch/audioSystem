#pragma once

#include <vector>
#include <chrono>
#include <atomic>
#include "subject.h"
#include "threadBase.h"
#include "MidiEvent.h"

/**
 * @file AudioSequencer.h
 * @brief Audio sequencer for testing without MIDI controller
 */

/**
 * @struct SequenceNote
 * @brief Represents a single note in a musical sequence
 */
struct SequenceNote {
    float frequency;        ///< Note frequency in Hz
    float duration;         ///< Note duration in seconds
    float velocity;         ///< Note velocity/intensity (0.0-1.0)
    float pauseAfter;       ///< Pause duration after note in seconds
    
    SequenceNote(float freq, float dur, float vel = 0.8f, float pause = 0.0f) 
        : frequency(freq), duration(dur), velocity(vel), pauseAfter(pause) {}
};

/**
 * @class AudioSequencer
 * @brief Plays predefined musical sequences for testing audio system without MIDI
 * 
 * This class generates musical sequences that can be used to test the audio system
 * when no MIDI controller is available. It provides several predefined musical
 * patterns and can notify observers (like AudioSystemAdapter) of note events.
 */
class AudioSequencer : public Subject, public ThreadBase {
public:
    /**
     * @brief Constructor
     */
    AudioSequencer();
    
    /**
     * @brief Destructor - ensures proper cleanup
     */
    ~AudioSequencer();
    
    /**
     * @brief Start playing a musical sequence
     * @param sequenceType Type of sequence to play ("scale", "chord", "melody", "demo")
     */
    void startSequence(const std::string& sequenceType = "demo");
    
    /**
     * @brief Stop the current sequence
     */
    void stopSequence();
    
    /**
     * @brief Check if a sequence is currently playing
     * @return true if sequence is playing, false otherwise
     */
    bool isPlaying() const;
    
    /**
     * @brief Play a sequence once and then stop
     * @param sequenceType Type of sequence to play
     */
    void playSequenceOnce(const std::string& sequenceType = "demo");

private:
    /**
     * @brief Thread function that plays the sequence (implements ThreadBase::thread)
     */
    void thread() override;
    
    /**
     * @brief Generate a major scale sequence (C major)
     * @return Vector of notes forming a major scale
     */
    std::vector<SequenceNote> generateMajorScale();
    
    /**
     * @brief Generate a chord progression sequence
     * @return Vector of notes forming chord progressions
     */
    std::vector<SequenceNote> generateChordProgression();
    
    /**
     * @brief Generate a simple melody sequence
     * @return Vector of notes forming a simple melody
     */
    std::vector<SequenceNote> generateSimpleMelody();
    
    /**
     * @brief Generate a demo sequence showcasing various features
     * @return Vector of notes forming a demonstration sequence
     */
    std::vector<SequenceNote> generateDemoSequence();
    
    /**
     * @brief Send a note on event to observers
     * @param frequency Note frequency in Hz
     * @param velocity Note velocity (0.0-1.0)
     */
    void sendNoteOn(float frequency, float velocity);
    
    /**
     * @brief Send a note off event to observers
     */
    void sendNoteOff();
    
    /**
     * @brief Convert frequency to MIDI note number for logging
     * @param frequency Frequency in Hz
     * @return Approximate MIDI note number
     */
    int frequencyToMidiNote(float frequency);

private:
    std::vector<SequenceNote> m_currentSequence;    ///< Current sequence being played
    std::atomic<bool> m_playing;                     ///< Whether sequence is currently playing
    std::string m_currentSequenceType;              ///< Type of current sequence
};
