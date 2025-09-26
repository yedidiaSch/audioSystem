#include "simple_gui.h"
#include "GUI/AudioSystemManager.h"
#include "GUI/ConfigurationManager.h"
#include "GUI/SoundController.h"
#include "GUI/GuiRenderers.h"
#include <memory>
#include <vector>
#include <iostream>

/**
 * @class AudioGuiApplication
 * @brief Main application class that coordinates all GUI components
 * 
 * This class follows the Dependency Injection pattern and coordinates
 * the interaction between different managers and renderers. It maintains
 * separation of concerns while providing a cohesive application interface.
 */
class AudioGuiApplication {
public:
    AudioGuiApplication() {
        // Initialize managers in dependency order
        configManager = std::make_unique<ConfigurationManager>();
        audioManager = std::make_unique<AudioSystemManager>(configManager->getConfig());
        soundController = std::make_unique<SoundController>(*audioManager, *configManager);
        
        // Initialize GUI renderers
        initializeRenderers();
        
        // Set up callbacks for cross-component communication
        setupCallbacks();
        
        std::cout << "🎛️ Audio GUI Application initialized with clean architecture" << std::endl;
    }
    
    ~AudioGuiApplication() {
        std::cout << "🔇 Audio GUI Application shutting down..." << std::endl;
    }

    void setupGui(GuiBase::SimpleGui& gui) {
        // Create main window with organized layout
        auto* mainWindow = gui.createWindow("Audio System Controller", GuiBase::Vec2(600, 700));
        
        mainWindow->setRenderCallback([this](GuiBase::GuiWindow& window) {
            window.text("🎛️ Audio System Control Panel");
            window.separator();
            
            // Render each component section
            for (auto& renderer : renderers) {
                if (renderer) {
                    renderer->render(window);
                    window.separator();
                }
            }
            
            // Render effects controls separately
            if (effectsRenderer) {
                effectsRenderer->render(window);
                window.separator();
            }
            
            // Configuration management buttons
            renderConfigurationButtons(window);
            
            // Render effect parameter windows (separate from main window)
            renderParameterWindows();
        });
    }

private:
    // Core managers
    std::unique_ptr<ConfigurationManager> configManager;
    std::unique_ptr<AudioSystemManager> audioManager;
    std::unique_ptr<SoundController> soundController;
    
    // GUI renderers
    std::vector<std::unique_ptr<IGuiRenderer>> renderers;
    std::unique_ptr<EffectsControlRenderer> effectsRenderer; // Keep separate reference for parameter windows
    
    void initializeRenderers() {
        // Create renderers in logical order
        renderers.emplace_back(std::make_unique<WaveformControlRenderer>(*configManager));
        renderers.emplace_back(std::make_unique<AudioParametersRenderer>(*configManager, *soundController));
        renderers.emplace_back(std::make_unique<SoundControlsRenderer>(*audioManager, *soundController));
        
        // Keep separate reference to effects renderer for parameter windows
        effectsRenderer = std::make_unique<EffectsControlRenderer>(*configManager, *audioManager);
        
        renderers.emplace_back(std::make_unique<StatusDisplayRenderer>(*audioManager, *configManager, *soundController));
    }
    
    void setupCallbacks() {
        // Configuration changes should update audio system
        configManager->setConfigChangeCallback([this](const AudioConfig& config) {
            audioManager->configure(config);
        });
        
        // Audio system state changes can trigger UI updates
        audioManager->setStateChangeCallback([this](bool isRunning) {
            std::cout << "Audio system state changed: " 
                      << (isRunning ? "Running" : "Stopped") << std::endl;
        });
        
        // Playback state changes
        soundController->setPlaybackStateCallback([this](bool isPlaying) {
            std::cout << "Playback state changed: " 
                      << (isPlaying ? "Playing" : "Stopped") << std::endl;
        });
    }
    
    void renderConfigurationButtons(GuiBase::GuiWindow& window) {
        window.text("Configuration:");
        
        if (window.button("💾 Save Configuration")) {
            configManager->saveConfig();
        }
        
        window.sameLine();
        if (window.button("🔄 Reset to Defaults")) {
            configManager->resetToDefaults();
        }
        
        // MIDI settings
        window.separator();
        window.text("MIDI Settings:");
        
        AudioConfig config = configManager->getConfig();
        float midiPortF = static_cast<float>(config.midiPort);
        if (window.slider("MIDI Port", midiPortF, -1.0f, 4.0f)) {
            config.midiPort = static_cast<int>(midiPortF);
            configManager->updateConfig(config);
            std::cout << "MIDI port changed to: " << config.midiPort << std::endl;
        }
        
        // Input mode selection
        window.text("Input Mode:");
        bool isMidi = (config.inputMode == "midi");
        bool isSequencer = (config.inputMode == "sequencer");
        
        if (window.checkbox("MIDI Input", isMidi)) {
            if (isMidi) {
                config.inputMode = "midi";
                configManager->updateConfig(config);
                std::cout << "Switched to MIDI input mode" << std::endl;
            }
        }
        window.sameLine();
        if (window.checkbox("Sequencer Mode", isSequencer)) {
            if (isSequencer) {
                config.inputMode = "sequencer";
                configManager->updateConfig(config);
                std::cout << "Switched to sequencer mode" << std::endl;
            }
        }
    }
    
    void renderParameterWindows() {
        if (effectsRenderer) {
            effectsRenderer->renderParameterWindows();
        }
    }
};

int main() {
    try {
        std::cout << "🎛️ Starting Audio System GUI with Clean Architecture..." << std::endl;
        
        GuiBase::SimpleGui gui(650, 750, "Audio System - Clean Architecture");
        if (!gui.initialize()) {
            std::cerr << "❌ Failed to initialize GUI!" << std::endl;
            return 1;
        }
        
        AudioGuiApplication application;
        application.setupGui(gui);
        
        std::cout << "✅ GUI initialized successfully with clean architecture" << std::endl;
        std::cout << "🎵 All audio controls are organized using SOLID principles!" << std::endl;
        
        gui.run();
        
        std::cout << "👋 Audio GUI closed gracefully." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Application error: " << e.what() << std::endl;
        return 1;
    }
}