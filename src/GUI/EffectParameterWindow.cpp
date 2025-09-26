#include "EffectParameterWindow.h"
#include "../../guiBase_cpp/external/imgui/imgui.h"
#include <cmath>

EffectParameterWindow::EffectParameterWindow() {
    // Initialize with default parameters
    delayParams_.reset();
    lowPassParams_.reset();
    octaveParams_.reset();
}

void EffectParameterWindow::render() {
    if (showDelayWindow_) {
        renderDelayWindow();
    }
    
    if (showLowPassWindow_) {
        renderLowPassWindow();
    }
    
    if (showOctaveWindow_) {
        renderOctaveWindow();
    }
}

void EffectParameterWindow::renderDelayWindow() {
    if (ImGui::Begin("Delay Effect Parameters", &showDelayWindow_)) {
        bool changed = false;
        
        // Delay Time slider (0.1 - 2.0 seconds)
        if (ImGui::SliderFloat("Delay Time (s)", &delayParams_.delayTime, 0.1f, 2.0f, "%.2f")) {
            changed = true;
        }
        ImGui::Text("Controls the time between echoes");
        
        ImGui::Separator();
        
        // Feedback slider (0.0 - 0.9)
        if (ImGui::SliderFloat("Feedback", &delayParams_.feedback, 0.0f, 0.9f, "%.2f")) {
            changed = true;
        }
        ImGui::Text("Amount of delayed signal fed back into the delay line");
        
        ImGui::Separator();
        
        // Mix slider (0.0 - 1.0)
        if (ImGui::SliderFloat("Mix", &delayParams_.mix, 0.0f, 1.0f, "%.2f")) {
            changed = true;
        }
        ImGui::Text("Balance between dry (0.0) and wet (1.0) signal");
        
        ImGui::Separator();
        
        // Reset button
        if (ImGui::Button("Reset to Defaults")) {
            delayParams_.reset();
            changed = true;
        }
        
        // Apply changes if any parameter changed
        if (changed && onParameterChange_) {
            onParameterChange_("delay", std::make_unique<DelayParameters>(delayParams_));
        }
    }
    ImGui::End();
}

void EffectParameterWindow::renderLowPassWindow() {
    if (ImGui::Begin("Low Pass Filter Parameters", &showLowPassWindow_)) {
        bool changed = false;
        
        // Cutoff Frequency slider (20 - 20000 Hz, logarithmic scale)
        float logCutoff = std::log10(lowPassParams_.cutoffFreq);
        if (ImGui::SliderFloat("Cutoff Frequency (Hz)", &logCutoff, std::log10(20.0f), std::log10(20000.0f),
            [](float value, char* buf, int buf_size) {
                float freq = std::pow(10.0f, value);
                snprintf(buf, buf_size, "%.0f Hz", freq);
                return true;
            })) {
            lowPassParams_.cutoffFreq = std::pow(10.0f, logCutoff);
            changed = true;
        }
        // Display actual frequency value
        ImGui::Text("Actual Frequency: %.0f Hz", lowPassParams_.cutoffFreq);
        ImGui::Text("Frequencies above this will be attenuated");
        
        ImGui::Separator();
        
        // Resonance slider (0.1 - 2.0)
        if (ImGui::SliderFloat("Resonance", &lowPassParams_.resonance, 0.1f, 2.0f, "%.2f")) {
            changed = true;
        }
        ImGui::Text("Filter resonance - higher values create emphasis at cutoff");
        
        ImGui::Separator();
        
        // Reset button
        if (ImGui::Button("Reset to Defaults")) {
            lowPassParams_.reset();
            changed = true;
        }
        
        // Apply changes if any parameter changed
        if (changed && onParameterChange_) {
            onParameterChange_("lowpass", std::make_unique<LowPassParameters>(lowPassParams_));
        }
    }
    ImGui::End();
}

void EffectParameterWindow::renderOctaveWindow() {
    if (ImGui::Begin("Octave Effect Parameters", &showOctaveWindow_)) {
        bool changed = false;
        
        // Octave Shift as discrete options
        const char* octaveOptions[] = { "Down 1 Octave (0.5x)", "Normal (1.0x)", "Up 1 Octave (2.0x)", "Up 2 Octaves (4.0x)" };
        const float octaveValues[] = { 0.5f, 1.0f, 2.0f, 4.0f };
        
        int currentOctaveIndex = 1; // Default to 1.0x
        for (int i = 0; i < 4; i++) {
            if (std::abs(octaveParams_.octaveShift - octaveValues[i]) < 0.01f) {
                currentOctaveIndex = i;
                break;
            }
        }
        
        if (ImGui::Combo("Octave Shift", &currentOctaveIndex, octaveOptions, 4)) {
            octaveParams_.octaveShift = octaveValues[currentOctaveIndex];
            changed = true;
        }
        ImGui::Text("Pitch shift amount - multiplies frequency");
        
        ImGui::Separator();
        
        // Mix slider (0.0 - 1.0)
        if (ImGui::SliderFloat("Mix", &octaveParams_.mix, 0.0f, 1.0f, "%.2f")) {
            changed = true;
        }
        ImGui::Text("Balance between original (0.0) and shifted (1.0) signal");
        
        ImGui::Separator();
        
        // Reset button
        if (ImGui::Button("Reset to Defaults")) {
            octaveParams_.reset();
            changed = true;
        }
        
        // Apply changes if any parameter changed
        if (changed && onParameterChange_) {
            onParameterChange_("octave", std::make_unique<OctaveParameters>(octaveParams_));
        }
    }
    ImGui::End();
}