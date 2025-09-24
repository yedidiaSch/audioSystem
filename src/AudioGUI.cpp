#include "AudioGUI.h"
#include "ConfigReader.h"
#include <iostream>
#include <algorithm>

AudioGUI::AudioGUI(std::shared_ptr<AudioSystem> audioSystem) 
    : m_audioSystem(audioSystem)
    , m_mainWindow(nullptr)
    , m_effectsWindow(nullptr)
    , m_selectedWaveform(0)
    , m_delayEnabled(false)
    , m_lowPassEnabled(false)
    , m_octaveEnabled(false)
    , m_volume(0.8f)
    , m_midiEnabled(true)
{
    // Load current configuration
    ConfigReader reader;
    m_config = reader.loadConfigWithFallback("config/config.xml");
    
    // Initialize GUI state from config
    updateWaveformSelection();
    updateEffectsToggles();
}

AudioGUI::~AudioGUI() {
}

bool AudioGUI::initialize() {
    try {
        m_gui = std::make_unique<GuiBase::SimpleGui>(1200, 800, "Audio System Control Panel");
        if (!m_gui->initialize()) {
            std::cerr << "Failed to initialize GUI" << std::endl;
            return false;
        }
        
        // Create main controls window
        m_mainWindow = m_gui->createWindow("Audio Controls", GuiBase::Vec2(400, 300));
        
        // Create effects window
        m_effectsWindow = m_gui->createWindow("Effects", GuiBase::Vec2(350, 250));
        
        // Set up the render callback
        m_gui->setCustomRenderCallback([this]() {
            renderMainControls();
            renderEffectsControls();
        });
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "GUI initialization error: " << e.what() << std::endl;
        return false;
    }
}

void AudioGUI::run() {
    if (m_gui) {
        m_gui->run();
    }
}

AudioConfig AudioGUI::getConfig() const {
    return m_config;
}

void AudioGUI::renderMainControls() {
    if (!m_mainWindow) return;
    
    // Waveform selection
    m_mainWindow->text("Waveform Selection");
    m_mainWindow->separator();
    
    // Create radio buttons for waveform selection
    for (int i = 0; i < 4; i++) {
        bool selected = (m_selectedWaveform == i);
        if (m_mainWindow->checkbox(m_waveformOptions[i], selected)) {
            if (selected) {
                m_selectedWaveform = i;
                m_config.waveform = m_waveformValues[i];
                applySettings();
            }
        }
        if (i < 3) m_mainWindow->sameLine(); // Put on same line except last one
    }
    
    m_mainWindow->separator();
    
    // Audio parameters
    m_mainWindow->text("Audio Parameters");
    
    float sampleRate = m_config.sampleRate;
    if (m_mainWindow->slider("Sample Rate", sampleRate, 22050.0f, 96000.0f)) {
        m_config.sampleRate = sampleRate;
        applySettings();
    }
    
    int bufferFrames = static_cast<int>(m_config.bufferFrames);
    if (m_mainWindow->slider("Buffer Size", static_cast<float&>(bufferFrames), 128.0f, 2048.0f)) {
        m_config.bufferFrames = static_cast<unsigned int>(bufferFrames);
        applySettings();
    }
    
    float defaultFreq = m_config.defaultFrequency;
    if (m_mainWindow->slider("Default Frequency", defaultFreq, 220.0f, 880.0f)) {
        m_config.defaultFrequency = defaultFreq;
        applySettings();
    }
    
    m_mainWindow->separator();
    
    // MIDI settings
    m_mainWindow->text("MIDI Settings");
    if (m_mainWindow->checkbox("Enable MIDI", m_midiEnabled)) {
        m_config.midiPort = m_midiEnabled ? 1 : -1;
        applySettings();
    }
    
    if (m_midiEnabled) {
        int midiPort = m_config.midiPort;
        if (m_mainWindow->slider("MIDI Port", static_cast<float&>(midiPort), 0.0f, 4.0f)) {
            m_config.midiPort = midiPort;
            applySettings();
        }
    }
    
    m_mainWindow->separator();
    
    // Control buttons
    if (m_mainWindow->button("Apply All Settings")) {
        applySettings();
        std::cout << "Settings applied!" << std::endl;
    }
    
    m_mainWindow->sameLine();
    if (m_mainWindow->button("Reset to Defaults")) {
        m_config = AudioConfig(); // Reset to defaults
        updateWaveformSelection();
        updateEffectsToggles();
        applySettings();
    }
}

void AudioGUI::renderEffectsControls() {
    if (!m_effectsWindow) return;
    
    m_effectsWindow->text("Audio Effects");
    m_effectsWindow->separator();
    
    // Effect toggles
    if (m_effectsWindow->checkbox("Low Pass Filter", m_lowPassEnabled)) {
        updateEffectsToggles();
        applySettings();
    }
    
    if (m_effectsWindow->checkbox("Delay/Echo", m_delayEnabled)) {
        updateEffectsToggles();
        applySettings();
    }
    
    if (m_effectsWindow->checkbox("Octave Effect", m_octaveEnabled)) {
        updateEffectsToggles();
        applySettings();
    }
    
    m_effectsWindow->separator();
    
    // Volume control
    m_effectsWindow->text("Master Controls");
    if (m_effectsWindow->slider("Volume", m_volume, 0.0f, 1.0f)) {
        // Apply volume to audio system if needed
    }
    
    m_effectsWindow->separator();
    
    // Display current effects chain
    m_effectsWindow->text("Active Effects Chain:");
    if (m_config.effects.empty()) {
        m_effectsWindow->text("  (No effects)");
    } else {
        for (const auto& effect : m_config.effects) {
            std::string effectText = "  • " + effect;
            m_effectsWindow->text(effectText);
        }
    }
}

void AudioGUI::applySettings() {
    if (m_audioSystem) {
        // Apply configuration to audio system
        // Note: This would require extending AudioSystem with a updateConfig method
        std::cout << "Applying settings: " << std::endl;
        ConfigReader::printConfig(m_config, "GUI");
    }
}

void AudioGUI::updateWaveformSelection() {
    // Find current waveform in options
    for (int i = 0; i < 4; i++) {
        if (m_config.waveform == m_waveformValues[i]) {
            m_selectedWaveform = i;
            break;
        }
    }
}

void AudioGUI::updateEffectsToggles() {
    // Update effects list based on toggles
    m_config.effects.clear();
    
    if (m_lowPassEnabled) {
        m_config.effects.push_back("lowpass");
    }
    if (m_delayEnabled) {
        m_config.effects.push_back("delay");
    }
    if (m_octaveEnabled) {
        m_config.effects.push_back("octave");
    }
    
    // Also update toggles based on current effects
    m_lowPassEnabled = std::find(m_config.effects.begin(), m_config.effects.end(), "lowpass") != m_config.effects.end();
    m_delayEnabled = std::find(m_config.effects.begin(), m_config.effects.end(), "delay") != m_config.effects.end();
    m_octaveEnabled = std::find(m_config.effects.begin(), m_config.effects.end(), "octave") != m_config.effects.end();
}