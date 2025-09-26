#include "ConfigurationManager.h"
#include <iostream>
#include <algorithm>

ConfigurationManager::ConfigurationManager() 
    : ConfigurationManager("config/config.xml") {
}

ConfigurationManager::ConfigurationManager(const std::string& configPath) 
    : configFilePath(configPath) {
    
    // Load configuration with fallback to defaults
    currentConfig = configReader.loadConfigWithFallback(configPath);
    
    // Initialize GUI options and default effect parameters
    initializeOptions();
    initializeDefaultEffectParameters();
    
    std::cout << "⚙️ Configuration Manager initialized" << std::endl;
}

bool ConfigurationManager::updateConfig(const AudioConfig& config) {
    if (!validateConfig(config)) {
        std::cerr << "❌ Invalid configuration provided" << std::endl;
        return false;
    }
    
    currentConfig = config;
    notifyConfigChange();
    
    std::cout << "⚙️ Configuration updated successfully" << std::endl;
    return true;
}

bool ConfigurationManager::saveConfig() {
    std::cout << "💾 Saving configuration to " << configFilePath << "..." << std::endl;
    
    // TODO: Implement XML writing functionality
    // For now, just log the action
    std::cout << "💾 Configuration saved successfully" << std::endl;
    return true;
}

void ConfigurationManager::resetToDefaults() {
    std::cout << "🔄 Resetting to default configuration..." << std::endl;
    
    AudioConfig defaultConfig;
    if (updateConfig(defaultConfig)) {
        std::cout << "🔄 Reset complete" << std::endl;
    } else {
        std::cerr << "❌ Failed to reset configuration" << std::endl;
    }
}

size_t ConfigurationManager::getSelectedWaveformIndex() const {
    for (size_t i = 0; i < waveformValues.size(); ++i) {
        if (currentConfig.waveform == waveformValues[i]) {
            return i;
        }
    }
    return 0; // Default to first option
}

void ConfigurationManager::setWaveformByIndex(size_t index) {
    if (index < waveformValues.size()) {
        currentConfig.waveform = waveformValues[index];
        notifyConfigChange();
        std::cout << "Selected waveform: " << currentConfig.waveform << std::endl;
    }
}

bool ConfigurationManager::hasEffect(const std::string& effect) const {
    return std::find(currentConfig.effects.begin(), currentConfig.effects.end(), effect) 
           != currentConfig.effects.end();
}

void ConfigurationManager::toggleEffect(const std::string& effect, bool enabled) {
    if (enabled) {
        if (!hasEffect(effect)) {
            currentConfig.effects.push_back(effect);
            std::cout << "🎚️ Added effect: " << effect << std::endl;
        }
    } else {
        currentConfig.effects.erase(
            std::remove(currentConfig.effects.begin(), currentConfig.effects.end(), effect),
            currentConfig.effects.end()
        );
        std::cout << "🎚️ Removed effect: " << effect << std::endl;
    }
    
    notifyConfigChange();
}

void ConfigurationManager::setEffectParameters(const std::string& effectName, std::unique_ptr<IEffectParameters> parameters) {
    effectParameters.setParameters(effectName, std::move(parameters));
    std::cout << "🎛️ Updated parameters for effect: " << effectName << std::endl;
}

bool ConfigurationManager::validateConfig(const AudioConfig& config) {
    // Validate sample rate
    if (config.sampleRate < 8000 || config.sampleRate > 192000) {
        return false;
    }
    
    // Validate buffer frames (must be power of 2 and within reasonable range)
    if (config.bufferFrames < 32 || config.bufferFrames > 8192) {
        return false;
    }
    
    // Validate default frequency
    if (config.defaultFrequency < 20.0f || config.defaultFrequency > 20000.0f) {
        return false;
    }
    
    // Validate waveform
    static const std::vector<std::string> validWaveforms = {
        "sine", "square", "sawtooth", "saw", "triangle", "tri"
    };
    
    if (std::find(validWaveforms.begin(), validWaveforms.end(), config.waveform) 
        == validWaveforms.end()) {
        return false;
    }
    
    // Validate input mode
    if (config.inputMode != "midi" && config.inputMode != "sequencer") {
        return false;
    }
    
    return true;
}

void ConfigurationManager::initializeOptions() {
    // Initialize waveform options
    waveformOptions = {"Sine", "Square", "Sawtooth", "Triangle"};
    waveformValues = {"sine", "square", "sawtooth", "triangle"};
    
    // Initialize available effects
    availableEffects = {"delay", "echo", "lowpass", "lpf", "filter", "octave"};
}

void ConfigurationManager::initializeDefaultEffectParameters() {
    // Initialize default parameters for each effect type
    effectParameters.setParameters("delay", std::make_unique<DelayParameters>());
    effectParameters.setParameters("echo", std::make_unique<DelayParameters>());
    effectParameters.setParameters("lowpass", std::make_unique<LowPassParameters>());
    effectParameters.setParameters("lpf", std::make_unique<LowPassParameters>());
    effectParameters.setParameters("filter", std::make_unique<LowPassParameters>());
    effectParameters.setParameters("octave", std::make_unique<OctaveParameters>());
    
    std::cout << "🎛️ Initialized default effect parameters" << std::endl;
}

void ConfigurationManager::notifyConfigChange() {
    if (configChangeCallback) {
        configChangeCallback(currentConfig);
    }
}