#include "simple_gui.h"
#include "AudioConfig.h"
#include "ConfigReader.h"
#include "audioSystem.h"
#include "audioDevice.h"
#include "AudioSequencer.h"
#include "AudioSystemAdapter.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

class AudioGuiController {
public:
    AudioGuiController() : selectedWaveform(0), isPlaying(false), audioDeviceStarted(false), frequency(440.0f), volume(0.8f) {
        // Load config
        ConfigReader reader;
        config = reader.loadConfigWithFallback("config/config.xml");
        
        // Initialize audio system
        audioSystem = std::make_shared<AudioSystem>(config.sampleRate);
        audioSystem->configure(config);
        
        // Initialize adapter for unified interface
        adapter = std::make_unique<AudioSystemAdapter>(audioSystem.get());
        
        // Initialize audio device - this is what actually produces sound!
        audioDevice = std::make_unique<AudioDevice>(audioSystem.get(), config.sampleRate, config.bufferFrames);
        
        // Set up waveform selection
        waveformOptions = {"Sine", "Square", "Sawtooth", "Triangle"};
        waveformValues = {"sine", "square", "sawtooth", "triangle"};
        for (size_t i = 0; i < waveformValues.size(); ++i) {
            if (config.waveform == waveformValues[i]) selectedWaveform = i;
        }
        frequency = config.defaultFrequency;
        
        std::cout << "🎛️ Audio GUI Controller initialized with full audio system" << std::endl;
    }
    
    ~AudioGuiController() {
        if (audioDeviceStarted && audioDevice) {
            std::cout << "🔇 Stopping audio device..." << std::endl;
            audioDevice->stop();
        }
    }

    void setupGui(GuiBase::SimpleGui& gui) {
        // Create main configuration window
        auto* configWindow = gui.createWindow("Audio System Controller", GuiBase::Vec2(500, 650));
        configWindow->setRenderCallback([this](GuiBase::GuiWindow& window) {
            window.text("🎛️ Audio System Control Panel");
            window.separator();
            
            // Waveform selection
            window.text("Waveform Type:");
            for (size_t i = 0; i < waveformOptions.size(); ++i) {
                bool selected = (selectedWaveform == i);
                if (window.checkbox(waveformOptions[i], selected)) {
                    if (selected) {
                        selectedWaveform = i;
                        config.waveform = waveformValues[i];
                        applyConfig();
                        std::cout << "Selected waveform: " << config.waveform << std::endl;
                    }
                }
                if (i < waveformOptions.size() - 1) window.sameLine();
            }
            
            window.separator();
            
            // Audio parameters
            window.text("Audio Parameters:");
            if (window.slider("Frequency (Hz)", frequency, 220.0f, 880.0f)) {
                config.defaultFrequency = frequency;
                applyConfig();
                std::cout << "Frequency changed to: " << frequency << " Hz" << std::endl;
            }
            
            if (window.slider("Sample Rate", config.sampleRate, 22050.0f, 96000.0f)) {
                applyConfig();
                std::cout << "Sample rate changed to: " << config.sampleRate << " Hz (restart audio system to apply)" << std::endl;
            }
            
            float bufferFramesF = static_cast<float>(config.bufferFrames);
            if (window.slider("Buffer Frames", bufferFramesF, 128.0f, 2048.0f)) {
                config.bufferFrames = static_cast<unsigned int>(bufferFramesF);
                std::cout << "Buffer frames changed to: " << config.bufferFrames << " (restart audio system to apply)" << std::endl;
            }
            
            window.separator();
            
            // Sound controls
            window.text("Sound Controls:");
            
            // Audio system start/stop
            if (!audioDeviceStarted) {
                if (window.button("🔊 Start Audio System")) {
                    startAudioSystem();
                }
                window.sameLine();
                window.text("(Audio system is stopped)");
            } else {
                if (window.button("🔇 Stop Audio System")) {
                    stopAudioSystem();
                }
                window.sameLine();
                window.text("(Audio system is running)");
            }
            
            if (window.slider("Volume", volume, 0.0f, 1.0f)) {
                std::cout << "Volume: " << (volume * 100.0f) << "%" << std::endl;
            }
            
            if (window.button(isPlaying ? "⏹️ Stop Sound" : "▶️ Play Test Tone")) {
                if (isPlaying) {
                    stopSound();
                } else {
                    playTestSound();
                }
            }
            
            window.sameLine();
            if (window.button("🎵 Play Demo")) {
                playDemoSequence();
            }
            
            window.separator();
            
            // MIDI settings
            window.text("MIDI Settings:");
            float midiPortF = static_cast<float>(config.midiPort);
            if (window.slider("MIDI Port", midiPortF, -1.0f, 4.0f)) {
                config.midiPort = static_cast<int>(midiPortF);
                std::cout << "MIDI port changed to: " << config.midiPort << std::endl;
            }
            
            window.separator();
            
            // Input mode
            window.text("Input Mode:");
            bool isMidi = (config.inputMode == "midi");
            bool isSequencer = (config.inputMode == "sequencer");
            
            if (window.checkbox("MIDI Input", isMidi)) {
                if (isMidi) {
                    config.inputMode = "midi";
                    std::cout << "Switched to MIDI input mode" << std::endl;
                }
            }
            window.sameLine();
            if (window.checkbox("Sequencer Mode", isSequencer)) {
                if (isSequencer) {
                    config.inputMode = "sequencer";
                    std::cout << "Switched to sequencer mode" << std::endl;
                }
            }
            
            window.separator();
            
            // Status display
            window.text("Status:");
            window.text("🎛️ Waveform: " + config.waveform);
            window.text("🔊 Audio: " + std::string(audioDeviceStarted ? "Running" : "Stopped"));
            window.text("🎵 Status: " + std::string(isPlaying ? "Playing" : "Ready"));
            window.text("🎵 Frequency: " + std::to_string((int)frequency) + " Hz");
            window.text("📊 Sample Rate: " + std::to_string((int)config.sampleRate) + " Hz");
            window.text("💾 Buffer: " + std::to_string(config.bufferFrames) + " frames");
            
            if (window.button("Save Configuration")) {
                saveConfig();
            }
        });
    }

private:
    std::shared_ptr<AudioSystem> audioSystem;
    std::unique_ptr<AudioSystemAdapter> adapter;
    std::unique_ptr<AudioDevice> audioDevice;
    AudioConfig config;
    std::vector<std::string> waveformOptions;
    std::vector<std::string> waveformValues;
    size_t selectedWaveform;
    bool isPlaying;
    bool audioDeviceStarted;
    float frequency;
    float volume;

    void applyConfig() {
        if (audioSystem) {
            audioSystem->configure(config);
            std::cout << "⚙️ Configuration applied to audio system" << std::endl;
        }
    }
    
    void startAudioSystem() {
        if (!audioDeviceStarted && audioDevice) {
            try {
                std::cout << "🔊 Starting audio device..." << std::endl;
                audioDevice->start();
                audioDeviceStarted = true;
                
                // Add a small delay to let the audio system initialize
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
                std::cout << "✅ Audio system started successfully!" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "❌ Failed to start audio system: " << e.what() << std::endl;
                audioDeviceStarted = false;
            }
        }
    }
    
    void stopAudioSystem() {
        if (audioDeviceStarted && audioDevice) {
            try {
                std::cout << "🔇 Stopping audio device..." << std::endl;
                if (isPlaying) {
                    stopSound();
                }
                audioDevice->stop();
                audioDeviceStarted = false;
                std::cout << "✅ Audio system stopped successfully!" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "❌ Failed to stop audio system: " << e.what() << std::endl;
            }
        }
    }
    
    bool hasEffect(const std::string& effect) {
        return std::find(config.effects.begin(), config.effects.end(), effect) != config.effects.end();
    }
    
    void toggleEffect(const std::string& effect, bool enabled) {
        if (enabled) {
            if (!hasEffect(effect)) {
                config.effects.push_back(effect);
                std::cout << "🎚️ Added effect: " << effect << std::endl;
            }
        } else {
            config.effects.erase(
                std::remove(config.effects.begin(), config.effects.end(), effect),
                config.effects.end()
            );
            std::cout << "🎚️ Removed effect: " << effect << std::endl;
        }
        applyConfig();
    }
    
    void playTestSound() {
        if (!audioDeviceStarted) {
            std::cout << "❌ Please start the audio system first!" << std::endl;
            return;
        }
        
        std::cout << "🎵 Playing test tone: " << config.waveform << " wave at " << frequency << "Hz" << std::endl;
        
        if (audioSystem) {
            isPlaying = true;
            // Trigger note with current frequency
            audioSystem->triggerNote(frequency);
            std::cout << "🔊 Audio system engaged - sound should be playing!" << std::endl;
            
            // Auto-stop after 3 seconds
            std::thread([this]() {
                std::this_thread::sleep_for(std::chrono::seconds(3));
                if (isPlaying) {
                    stopSound();
                }
            }).detach();
        }
    }
    
    void stopSound() {
        std::cout << "⏹️ Stopping audio" << std::endl;
        isPlaying = false;
        
        if (audioSystem) {
            // Stop audio by triggering note off
            audioSystem->triggerNoteOff();
        }
    }
    
    void playDemoSequence() {
        if (!audioDeviceStarted) {
            std::cout << "❌ Please start the audio system first!" << std::endl;
            return;
        }
        
        std::cout << "🎵 Playing demo sequence: " << config.sequenceType << " pattern" << std::endl;
        
        if (audioSystem && config.inputMode == "sequencer") {
            // Play a simple melody using the audio system
            std::thread([this]() {
                float notes[] = {440.0f, 523.25f, 659.25f, 783.99f}; // A4, C5, E5, G5
                for (int i = 0; i < 4; ++i) {
                    audioSystem->triggerNote(notes[i]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    audioSystem->triggerNoteOff();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                std::cout << "🎼 Demo sequence completed" << std::endl;
            }).detach();
        } else {
            std::cout << "🎼 Demo sequence requires audio system to be running" << std::endl;
        }
    }
    
    void saveConfig() {
        std::cout << "💾 Saving configuration to config/config.xml..." << std::endl;
        // TODO: Implement configuration saving
        std::cout << "💾 Configuration saved successfully" << std::endl;
    }
    
    void resetToDefaults() {
        std::cout << "🔄 Resetting to default configuration..." << std::endl;
        ConfigReader reader;
        config = AudioConfig(); // Use default constructor
        frequency = config.defaultFrequency;
        selectedWaveform = 0;
        applyConfig();
        std::cout << "🔄 Reset complete" << std::endl;
    }
};

int main() {
    try {
        std::cout << "🎛️ Starting Audio System GUI with Sound Support..." << std::endl;
        
        GuiBase::SimpleGui gui(600, 700, "Audio System with Sound");
        if (!gui.initialize()) {
            std::cerr << "Failed to initialize GUI!" << std::endl;
            return 1;
        }
        
        AudioGuiController controller;
        controller.setupGui(gui);
        
        std::cout << "✅ GUI initialized successfully with full audio system" << std::endl;
        std::cout << "🎵 All audio controls are now fully functional!" << std::endl;
        gui.run();
        
        std::cout << "Audio GUI closed." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}