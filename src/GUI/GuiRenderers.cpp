#include "GuiRenderers.h"
#include <iostream>

// WaveformControlRenderer implementation
WaveformControlRenderer::WaveformControlRenderer(ConfigurationManager& configManager)
    : configManager(configManager) {
}

void WaveformControlRenderer::render(GuiBase::GuiWindow& window) {
    window.text("Waveform4 Type:");
    
    const auto& waveformOptions = configManager.getWaveformOptions();
    size_t selectedWaveform = configManager.getSelectedWaveformIndex();
    
    for (size_t i = 0; i < waveformOptions.size(); ++i) {
        bool selected = (selectedWaveform == i);
        if (window.checkbox(waveformOptions[i], selected)) {
            if (selected) {
                configManager.setWaveformByIndex(i);
            }
        }
        if (i < waveformOptions.size() - 1) {
            window.sameLine();
        }
    }
}

// AudioParametersRenderer implementation
AudioParametersRenderer::AudioParametersRenderer(ConfigurationManager& configManager, 
                                                 SoundController& soundController)
    : configManager(configManager), soundController(soundController) {
}

void AudioParametersRenderer::render(GuiBase::GuiWindow& window) {
    window.text("Audio Parameters:");
    
    // Get current config
    AudioConfig config = configManager.getConfig();
    float frequency = soundController.getFrequency();
    
    // Frequency control
    if (window.slider("Frequency (Hz)", frequency, 220.0f, 880.0f)) {
        soundController.setFrequency(frequency);
        config.defaultFrequency = frequency;
        configManager.updateConfig(config);
        std::cout << "Frequency changed to: " << frequency << " Hz" << std::endl;
    }
    
    // Sample rate control
    if (window.slider("Sample Rate", config.sampleRate, 22050.0f, 96000.0f)) {
        configManager.updateConfig(config);
        std::cout << "Sample rate changed to: " << config.sampleRate 
                  << " Hz (restart audio system to apply)" << std::endl;
    }
    
    // Buffer frames control
    float bufferFramesF = static_cast<float>(config.bufferFrames);
    if (window.slider("Buffer Frames", bufferFramesF, 128.0f, 2048.0f)) {
        config.bufferFrames = static_cast<unsigned int>(bufferFramesF);
        configManager.updateConfig(config);
        std::cout << "Buffer frames changed to: " << config.bufferFrames 
                  << " (restart audio system to apply)" << std::endl;
    }
}

// SoundControlsRenderer implementation
SoundControlsRenderer::SoundControlsRenderer(AudioSystemManager& audioManager, 
                                           SoundController& soundController)
    : audioSystemManager(audioManager), soundController(soundController) {
}

void SoundControlsRenderer::render(GuiBase::GuiWindow& window) {
    window.text("Sound Controls:");
    
    // Audio system start/stop
    if (!audioSystemManager.isRunning()) {
        if (window.button("Start Audio System")) {
            audioSystemManager.start();
        }
        window.sameLine();
        window.text("(Audio system is stopped)");
    } else {
        if (window.button("Stop Audio System")) {
            audioSystemManager.stop();
        }
        window.sameLine();
        window.text("(Audio system is running)");
    }
    
    // Volume control
    float volume = soundController.getVolume();
    if (window.slider("Volume", volume, 0.0f, 1.0f)) {
        soundController.setVolume(volume);
        std::cout << "Volume: " << (volume * 100.0f) << "%" << std::endl;
    }
    
    // Play/stop controls
    if (window.button(soundController.isPlaying() ? "Stop Sound" : "Play Test Tone")) {
        if (soundController.isPlaying()) {
            soundController.stopSound();
        } else {
            soundController.playTestTone();
        }
    }
    
    window.sameLine();
    if (window.button("Play Demo")) {
        soundController.playDemoSequence("demo");
    }
    
    // Additional demo sequences
    if (window.button("Scale")) {
        soundController.playDemoSequence("scale");
    }
    window.sameLine();
    if (window.button("Chord")) {
        soundController.playDemoSequence("chord");
    }
    window.sameLine();
    if (window.button("Melody")) {
        soundController.playDemoSequence("melody");
    }
}

// EffectsControlRenderer implementation
EffectsControlRenderer::EffectsControlRenderer(ConfigurationManager& configManager, AudioSystemManager& audioManager)
    : configManager(configManager), audioSystemManager(audioManager) {
    
    // Initialize parameter window with default values from config manager
    if (auto delayParams = configManager.getEffectParameters<DelayParameters>("delay")) {
        parameterWindow.setDelayParameters(*delayParams);
    }
    if (auto lowPassParams = configManager.getEffectParameters<LowPassParameters>("lowpass")) {
        parameterWindow.setLowPassParameters(*lowPassParams);
    }
    if (auto octaveParams = configManager.getEffectParameters<OctaveParameters>("octave")) {
        parameterWindow.setOctaveParameters(*octaveParams);
    }
    
    // Set up parameter change callback
    parameterWindow.setParameterChangeCallback([this, &configManager](const std::string& effectName, std::unique_ptr<IEffectParameters> params) {
        // Update configuration manager with new parameters
        auto paramsCopy = params->clone();
        configManager.setEffectParameters(effectName, std::move(paramsCopy));
        
        // Apply parameters to audio system
        if (audioSystemManager.getAudioSystem()) {
            audioSystemManager.getAudioSystem()->updateEffectParameters(effectName, *params);
        }
    });
}

void EffectsControlRenderer::render(GuiBase::GuiWindow& window) {
    window.text("Audio Effects:");
    
    // Group related effects for organized display
    std::vector<std::pair<std::string, std::vector<std::string>>> effectGroups = {
        {"Filters", {"lowpass", "lpf", "filter"}},
        {"Time Effects", {"delay", "echo"}},
        {"Harmonic", {"octave"}}
    };
    
    for (const auto& group : effectGroups) {
        window.text(group.first + ":");
        
        for (const std::string& effect : group.second) {
            bool enabled = configManager.hasEffect(effect);
            if (window.checkbox(effect, enabled)) {
                configManager.toggleEffect(effect, enabled);
            }
            
            // Add parameter button if effect is enabled
            if (enabled) {
                window.sameLine();
                if (window.button(("Params##" + effect).c_str())) {
                    // Show parameter window for this effect
                    if (effect == "delay" || effect == "echo") {
                        parameterWindow.showDelayWindow(true);
                    } else if (effect == "lowpass" || effect == "lpf" || effect == "filter") {
                        parameterWindow.showLowPassWindow(true);
                    } else if (effect == "octave") {
                        parameterWindow.showOctaveWindow(true);
                    }
                }
            }
            window.sameLine();
        }
        window.text(""); // New line after group
    }
}

void EffectsControlRenderer::renderParameterWindows() {
    parameterWindow.render();
}

// StatusDisplayRenderer implementation
StatusDisplayRenderer::StatusDisplayRenderer(const AudioSystemManager& audioManager,
                                           const ConfigurationManager& configManager,
                                           const SoundController& soundController)
    : audioSystemManager(audioManager), configManager(configManager), soundController(soundController) {
}

void StatusDisplayRenderer::render(GuiBase::GuiWindow& window) {
    window.text("System Status:");
    
    const AudioConfig& config = configManager.getConfig();
    
    window.text("Waveform: " + config.waveform);
    window.text("Audio: " + std::string(audioSystemManager.isRunning() ? "Running" : "Stopped"));
    window.text("Playback: " + std::string(soundController.isPlaying() ? "Playing" : "Ready"));
    window.text("Frequency: " + std::to_string(static_cast<int>(soundController.getFrequency())) + " Hz");
    window.text("Sample Rate: " + std::to_string(static_cast<int>(config.sampleRate)) + " Hz");
    window.text("Buffer: " + std::to_string(config.bufferFrames) + " frames");
    window.text("Volume: " + std::to_string(static_cast<int>(soundController.getVolume() * 100)) + "%");
    
    // Effects status
    if (!config.effects.empty()) {
        std::string effectsStr = "Effects: ";
        for (size_t i = 0; i < config.effects.size(); ++i) {
            effectsStr += config.effects[i];
            if (i < config.effects.size() - 1) effectsStr += ", ";
        }
        window.text(effectsStr);
    } else {
        window.text("Effects: None");
    }
    
    window.text("Input Mode: " + config.inputMode);
}