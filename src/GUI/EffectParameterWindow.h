#pragma once

#include "../Effects/EffectParameters.h"
#include <functional>

/**
 * GUI window for controlling effect parameters
 */
class EffectParameterWindow {
public:
    using ParameterChangeCallback = std::function<void(const std::string&, std::unique_ptr<IEffectParameters>)>;
    
    EffectParameterWindow();
    ~EffectParameterWindow() = default;
    
    // Set callback for when parameters change
    void setParameterChangeCallback(ParameterChangeCallback callback) {
        onParameterChange_ = callback;
    }
    
    // Show/hide parameter windows
    void showDelayWindow(bool show) { showDelayWindow_ = show; }
    void showLowPassWindow(bool show) { showLowPassWindow_ = show; }
    void showOctaveWindow(bool show) { showOctaveWindow_ = show; }
    
    // Set current parameters (for initialization)
    void setDelayParameters(const DelayParameters& params) { delayParams_ = params; }
    void setLowPassParameters(const LowPassParameters& params) { lowPassParams_ = params; }
    void setOctaveParameters(const OctaveParameters& params) { octaveParams_ = params; }
    
    // Render the parameter windows
    void render();
    
private:
    void renderDelayWindow();
    void renderLowPassWindow();
    void renderOctaveWindow();
    
    // Window visibility flags
    bool showDelayWindow_ = false;
    bool showLowPassWindow_ = false;
    bool showOctaveWindow_ = false;
    
    // Parameter instances
    DelayParameters delayParams_;
    LowPassParameters lowPassParams_;
    OctaveParameters octaveParams_;
    
    // Callback for parameter changes
    ParameterChangeCallback onParameterChange_;
};