#pragma once

#include "AudioConfig.h"
#include "ConfigReader.h"
#include "Effects/EffectParameters.h"
#include <functional>
#include <vector>
#include <string>

/**
 * @class ConfigurationManager
 * @brief Manages audio system configuration with validation and persistence
 * 
 * This class handles all configuration-related operations including loading,
 * saving, validation, and providing configuration options for the GUI.
 * Follows Single Responsibility Principle by focusing only on configuration management.
 */
class ConfigurationManager {
public:
    using ConfigChangeCallback = std::function<void(const AudioConfig&)>;

    /**
     * @brief Constructor loads configuration from default path
     */
    ConfigurationManager();

    /**
     * @brief Constructor with custom configuration path
     * @param configPath Path to configuration file
     */
    explicit ConfigurationManager(const std::string& configPath);

    /**
     * @brief Get current configuration
     * @return Current audio configuration
     */
    const AudioConfig& getConfig() const { return currentConfig; }

    /**
     * @brief Update configuration and notify listeners
     * @param config New configuration
     * @return true if configuration is valid and applied
     */
    bool updateConfig(const AudioConfig& config);

    /**
     * @brief Save current configuration to file
     * @return true if saved successfully
     */
    bool saveConfig();

    /**
     * @brief Reset to default configuration
     */
    void resetToDefaults();

    /**
     * @brief Set callback for configuration changes
     * @param callback Function to call when configuration changes
     */
    void setConfigChangeCallback(ConfigChangeCallback callback) {
        configChangeCallback = callback;
    }

    // Configuration options for GUI
    const std::vector<std::string>& getWaveformOptions() const { return waveformOptions; }
    const std::vector<std::string>& getWaveformValues() const { return waveformValues; }
    const std::vector<std::string>& getAvailableEffects() const { return availableEffects; }

    /**
     * @brief Get index of current waveform for GUI selection
     * @return Index of selected waveform
     */
    size_t getSelectedWaveformIndex() const;

    /**
     * @brief Set waveform by index
     * @param index Index from waveformOptions
     */
    void setWaveformByIndex(size_t index);

    /**
     * @brief Check if effect is currently enabled
     * @param effect Effect name to check
     * @return true if effect is enabled
     */
    bool hasEffect(const std::string& effect) const;

    /**
     * @brief Toggle effect on/off
     * @param effect Effect name
     * @param enabled Whether to enable or disable
     */
    void toggleEffect(const std::string& effect, bool enabled);

    /**
     * @brief Set parameters for a specific effect
     * @param effectName Name of the effect
     * @param parameters Parameters to set
     */
    void setEffectParameters(const std::string& effectName, std::unique_ptr<IEffectParameters> parameters);

    /**
     * @brief Get parameters for a specific effect
     * @param effectName Name of the effect
     * @return Pointer to effect parameters, or nullptr if not found
     */
    template<typename T>
    T* getEffectParameters(const std::string& effectName) {
        return effectParameters.getParameters<T>(effectName);
    }

    /**
     * @brief Get the effect parameters manager
     * @return Reference to the effect parameters manager
     */
    const EffectParametersManager& getEffectParametersManager() const { return effectParameters; }

    /**
     * @brief Validate configuration parameters
     * @param config Configuration to validate
     * @return true if configuration is valid
     */
    static bool validateConfig(const AudioConfig& config);

private:
    AudioConfig currentConfig;
    ConfigReader configReader;
    std::string configFilePath;
    ConfigChangeCallback configChangeCallback;
    EffectParametersManager effectParameters;

    // GUI options
    std::vector<std::string> waveformOptions;
    std::vector<std::string> waveformValues;
    std::vector<std::string> availableEffects;

    void initializeOptions();
    void initializeDefaultEffectParameters();
    void notifyConfigChange();
};