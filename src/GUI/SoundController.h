#pragma once

#include "AudioSystemManager.h"
#include "ConfigurationManager.h"
#include <functional>
#include <thread>
#include <atomic>
#include <memory>

/**
 * @class SoundController
 * @brief Handles sound playback operations and demo sequences
 * 
 * This class manages all sound playback functionality including test tones,
 * demo sequences, and note management. It depends on AudioSystemManager
 * for actual audio operations, following Dependency Inversion Principle.
 */
class SoundController {
public:
    using PlaybackStateCallback = std::function<void(bool isPlaying)>;

    /**
     * @brief Constructor
     * @param audioManager Reference to audio system manager
     * @param configManager Reference to configuration manager
     */
    SoundController(AudioSystemManager& audioManager, ConfigurationManager& configManager);

    /**
     * @brief Destructor ensures proper cleanup
     */
    ~SoundController();

    /**
     * @brief Play a test tone with current frequency
     * @param durationMs Duration in milliseconds (default: 3000ms)
     */
    void playTestTone(int durationMs = 3000);

    /**
     * @brief Stop currently playing sound
     */
    void stopSound();

    /**
     * @brief Play a demo sequence
     * @param sequenceType Type of sequence to play ("scale", "chord", "melody", "demo")
     */
    void playDemoSequence(const std::string& sequenceType = "demo");

    /**
     * @brief Check if sound is currently playing
     * @return true if playing, false otherwise
     */
    bool isPlaying() const { return currentlyPlaying.load(); }

    /**
     * @brief Set callback for playback state changes
     * @param callback Function to call when playback state changes
     */
    void setPlaybackStateCallback(PlaybackStateCallback callback) {
        playbackStateCallback = callback;
    }

    /**
     * @brief Set the current frequency for test tones
     * @param frequency Frequency in Hz
     */
    void setFrequency(float frequency) { currentFrequency = frequency; }

    /**
     * @brief Get the current frequency
     * @return Current frequency in Hz
     */
    float getFrequency() const { return currentFrequency; }

    /**
     * @brief Set volume level
     * @param volume Volume level (0.0 to 1.0)
     */
    void setVolume(float volume) { currentVolume = volume; }

    /**
     * @brief Get current volume level
     * @return Volume level (0.0 to 1.0)
     */
    float getVolume() const { return currentVolume; }

private:
    AudioSystemManager& audioSystemManager;
    ConfigurationManager& configurationManager;
    
    std::atomic<bool> currentlyPlaying{false};
    float currentFrequency{440.0f};
    float currentVolume{0.8f};
    
    PlaybackStateCallback playbackStateCallback;
    std::atomic<bool> shouldStop{false};

    void notifyPlaybackStateChange();
    void playSequenceScale();
    void playSequenceChord();
    void playSequenceMelody();
    void playSequenceDemo();
};