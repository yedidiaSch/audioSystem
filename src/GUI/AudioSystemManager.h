#pragma once

#include "audioSystem.h"
#include "audioDevice.h"
#include "AudioSystemAdapter.h"
#include "AudioConfig.h"
#include <memory>
#include <functional>

/**
 * @class AudioSystemManager
 * @brief Manages the lifecycle and operations of the audio system
 * 
 * This class encapsulates all audio system related operations following
 * the Single Responsibility Principle. It handles initialization, configuration,
 * starting/stopping, and provides a clean interface for audio operations.
 */
class AudioSystemManager {
public:
    using StateChangeCallback = std::function<void(bool isRunning)>;

    /**
     * @brief Constructor initializes the audio system with given configuration
     * @param config Initial audio configuration
     */
    explicit AudioSystemManager(const AudioConfig& config);

    /**
     * @brief Destructor ensures proper cleanup
     */
    ~AudioSystemManager();

    /**
     * @brief Start the audio system
     * @return true if started successfully, false otherwise
     */
    bool start();

    /**
     * @brief Stop the audio system
     */
    void stop();

    /**
     * @brief Check if audio system is currently running
     * @return true if running, false otherwise
     */
    bool isRunning() const { return audioDeviceStarted; }

    /**
     * @brief Apply new configuration to the audio system
     * @param config New configuration to apply
     */
    void configure(const AudioConfig& config);

    /**
     * @brief Trigger a note with specified frequency
     * @param frequency Frequency in Hz
     */
    void triggerNote(float frequency);

    /**
     * @brief Stop the currently playing note
     */
    void triggerNoteOff();

    /**
     * @brief Set callback for audio system state changes
     * @param callback Function to call when state changes
     */
    void setStateChangeCallback(StateChangeCallback callback) {
        stateChangeCallback = callback;
    }

    /**
     * @brief Get current audio configuration
     * @return Current configuration
     */
    const AudioConfig& getConfig() const { return currentConfig; }

    /**
     * @brief Get the audio system instance (for parameter updates)
     * @return Shared pointer to the audio system, may be null if not initialized
     */
    std::shared_ptr<AudioSystem> getAudioSystem() const { return audioSystem; }

private:
    std::shared_ptr<AudioSystem> audioSystem;
    std::unique_ptr<AudioSystemAdapter> adapter;
    std::unique_ptr<AudioDevice> audioDevice;
    AudioConfig currentConfig;
    bool audioDeviceStarted;
    StateChangeCallback stateChangeCallback;

    void notifyStateChange();
    void initializeAudioSystem(const AudioConfig& config);
};