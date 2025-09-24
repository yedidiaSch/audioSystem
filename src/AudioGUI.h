#pragma once

#include "guiBase_cpp/include/simple_gui.h"
#include "AudioConfig.h"
#include "audioSystem.h"
#include <memory>
#include <functional>

/**
 * @class AudioGUI
 * @brief GUI interface for the audio system
 * 
 * Provides a graphical user interface to control audio system parameters
 * that were previously configured via XML files.
 */
class AudioGUI {
public:
    /**
     * @brief Constructor
     * @param audioSystem Shared pointer to the audio system to control
     */
    AudioGUI(std::shared_ptr<AudioSystem> audioSystem);
    
    /**
     * @brief Destructor
     */
    ~AudioGUI();
    
    /**
     * @brief Initialize the GUI
     * @return true if initialization was successful
     */
    bool initialize();
    
    /**
     * @brief Run the GUI main loop (blocking)
     */
    void run();
    
    /**
     * @brief Get current audio configuration from GUI controls
     * @return AudioConfig structure with current GUI values
     */
    AudioConfig getConfig() const;

private:
    std::unique_ptr<GuiBase::SimpleGui> m_gui;
    GuiBase::GuiWindow* m_mainWindow;
    GuiBase::GuiWindow* m_effectsWindow;
    std::shared_ptr<AudioSystem> m_audioSystem;
    
    // GUI state variables
    AudioConfig m_config;
    int m_selectedWaveform;
    bool m_delayEnabled;
    bool m_lowPassEnabled;
    bool m_octaveEnabled;
    float m_volume;
    bool m_midiEnabled;
    
    // Available options
    const char* m_waveformOptions[4] = {"Sine", "Square", "Sawtooth", "Triangle"};
    const char* m_waveformValues[4] = {"sine", "square", "sawtooth", "triangle"};
    
    /**
     * @brief Render the main audio controls window
     */
    void renderMainControls();
    
    /**
     * @brief Render the effects controls window
     */
    void renderEffectsControls();
    
    /**
     * @brief Apply current GUI settings to the audio system
     */
    void applySettings();
    
    /**
     * @brief Update waveform selection based on config
     */
    void updateWaveformSelection();
    
    /**
     * @brief Update effects toggles based on config
     */
    void updateEffectsToggles();
};