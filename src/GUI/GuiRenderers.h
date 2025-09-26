#pragma once

#include "simple_gui.h"
#include "AudioSystemManager.h"
#include "ConfigurationManager.h"
#include "SoundController.h"
#include "EffectParameterWindow.h"

/**
 * @interface IGuiRenderer
 * @brief Interface for GUI rendering components
 * 
 * This interface follows the Interface Segregation Principle by defining
 * a minimal interface for GUI rendering components. Different UI panels
 * can implement this interface independently.
 */
class IGuiRenderer {
public:
    virtual ~IGuiRenderer() = default;
    
    /**
     * @brief Render the GUI component
     * @param window Reference to the GUI window
     */
    virtual void render(GuiBase::GuiWindow& window) = 0;
    
    /**
     * @brief Get the title for this renderer component
     * @return Component title
     */
    virtual std::string getTitle() const = 0;
};

/**
 * @class WaveformControlRenderer
 * @brief Renders waveform selection controls
 */
class WaveformControlRenderer : public IGuiRenderer {
public:
    explicit WaveformControlRenderer(ConfigurationManager& configManager);
    void render(GuiBase::GuiWindow& window) override;
    std::string getTitle() const override { return "Waveform Controls"; }

private:
    ConfigurationManager& configManager;
};

/**
 * @class AudioParametersRenderer
 * @brief Renders audio parameter controls (frequency, sample rate, buffer size)
 */
class AudioParametersRenderer : public IGuiRenderer {
public:
    AudioParametersRenderer(ConfigurationManager& configManager, SoundController& soundController);
    void render(GuiBase::GuiWindow& window) override;
    std::string getTitle() const override { return "Audio Parameters"; }

private:
    ConfigurationManager& configManager;
    SoundController& soundController;
};

/**
 * @class SoundControlsRenderer  
 * @brief Renders sound control buttons and volume
 */
class SoundControlsRenderer : public IGuiRenderer {
public:
    SoundControlsRenderer(AudioSystemManager& audioManager, SoundController& soundController);
    void render(GuiBase::GuiWindow& window) override;
    std::string getTitle() const override { return "Sound Controls"; }

private:
    AudioSystemManager& audioSystemManager;
    SoundController& soundController;
};

/**
 * @class EffectsControlRenderer
 * @brief Renders effects controls
 */
class EffectsControlRenderer : public IGuiRenderer {
public:
    EffectsControlRenderer(ConfigurationManager& configManager, AudioSystemManager& audioManager);
    void render(GuiBase::GuiWindow& window) override;
    std::string getTitle() const override { return "Audio Effects"; }
    
    // Render effect parameter windows
    void renderParameterWindows();

private:
    ConfigurationManager& configManager;
    AudioSystemManager& audioSystemManager;
    EffectParameterWindow parameterWindow;
};

/**
 * @class StatusDisplayRenderer
 * @brief Renders system status information
 */
class StatusDisplayRenderer : public IGuiRenderer {
public:
    StatusDisplayRenderer(const AudioSystemManager& audioManager, 
                         const ConfigurationManager& configManager,
                         const SoundController& soundController);
    void render(GuiBase::GuiWindow& window) override;
    std::string getTitle() const override { return "System Status"; }

private:
    const AudioSystemManager& audioSystemManager;
    const ConfigurationManager& configManager;
    const SoundController& soundController;
};