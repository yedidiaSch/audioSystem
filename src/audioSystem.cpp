#include <cmath>
#include <algorithm> // For std::find and std::transform
#include <cctype>    // For std::tolower
#include "audioSystem.h"
#include "Waves/SquareWave.h" // Include the square wave implementation
#include "Waves/SineWave.h"
#include "Waves/SawtoothWave.h"
#include "Waves/TriangleWave.h"
#include "Effects/OctaveEffect.h"
#include "Effects/DelayEffect.h"
#include "Effects/LowPassEffect.h"

namespace {
    /**
     * @brief Convert string to lowercase for case-insensitive comparison
     */
    std::string toLowercase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }
}

AudioSystem::AudioSystem(float sampleRate) : m_frequency(0.0f),
                                             m_sampleRate(sampleRate > 0.0f ? sampleRate : 44100.0f),
                                             m_phase(0.0f),
                                             m_noteOn(false) 
{
    // Validate sample rate
    if (sampleRate <= 0.0f) {
        // Use a reasonable default and potentially log warning
        m_sampleRate = 44100.0f;
    }
    
    // Default to square wave
    m_waveform = std::make_shared<SquareWave>();
}

void AudioSystem::setWaveform(std::shared_ptr<IWave> waveform)
{
    if (waveform) {
        m_waveform = waveform;
    }
    // If waveform is null, keep existing waveform
}

// Configure the oscillator and effects based on the provided AudioConfig
void AudioSystem::configure(const AudioConfig& config)
{
    // Select waveform (case-insensitive)
    std::string waveformLower = toLowercase(config.waveform);
    
    if (waveformLower == "sine") {
        m_waveform = std::make_shared<SineWave>();
    } else if (waveformLower == "sawtooth" || waveformLower == "saw") {
        m_waveform = std::make_shared<SawtoothWave>();
    } else if (waveformLower == "triangle" || waveformLower == "tri") {
        m_waveform = std::make_shared<TriangleWave>();
    } else if (waveformLower == "square" || waveformLower.empty()) {
        // Default to square wave for empty or unrecognized waveforms
        m_waveform = std::make_shared<SquareWave>();
    } else {
        // Fallback to square wave for unrecognized waveforms
        m_waveform = std::make_shared<SquareWave>();
    }

    // Clear existing effects
    m_effects.clear();

    // Instantiate effects listed in the configuration (case-insensitive)
    for (const auto& name : config.effects)
    {
        std::string effectLower = toLowercase(name);
        
        if (effectLower == "octave") {
            auto eff = std::make_shared<OctaveEffect>();
            m_effects.push_back(eff);
        } else if (effectLower == "delay" || effectLower == "echo") {
            auto eff = std::make_shared<DelayEffect>(0.3f, 0.5f, 0.5f, m_sampleRate);
            m_effects.push_back(eff);
        } else if (effectLower == "lowpass" || effectLower == "lpf" || effectLower == "filter") {
            auto eff = std::make_shared<LowPassEffect>(1000.0f, m_sampleRate);
            m_effects.push_back(eff);
        }
        // Silently ignore unrecognized effect names
    }
}

void AudioSystem::triggerNote(float newFrequency)
{
    // Validate frequency range (20 Hz to 20 kHz is typical audio range)
    if (newFrequency <= 0.0f || newFrequency > 20000.0f) {
        return; // Ignore invalid frequencies
    }
    
    m_frequency = newFrequency;
    m_noteOn = true;
    m_phase = 0.0f;

    // Configure effects with the new frequency and sample rate
    for (auto& effect : m_effects)
    {
        if (auto octave = std::dynamic_pointer_cast<OctaveEffect>(effect))
        {
            octave->setFrequency(newFrequency);
            octave->setSampleRate(m_sampleRate);
        }
        else if (auto delay = std::dynamic_pointer_cast<DelayEffect>(effect))
        {
            delay->setSampleRate(m_sampleRate);
        }
        else if (auto lp = std::dynamic_pointer_cast<LowPassEffect>(effect))
        {
            lp->setSampleRate(m_sampleRate);
        }
    }

    resetEffects();
}

void AudioSystem::triggerNoteOff() 
{
    m_noteOn = false;
}

std::pair<float, float> AudioSystem::getNextSample() 
{
    if (!m_noteOn || !m_waveform) 
    {
        return {0.0f, 0.0f};
    }

    // Generate a sample using the waveform generator
    float sample = m_waveform->generate(m_frequency, m_sampleRate, m_phase);

    // Create a stereo sample (initially identical in both channels)
    std::pair<float, float> stereoSample{sample, sample};

    // Apply effects to the stereo sample
    stereoSample = applyEffects(stereoSample);

    return stereoSample;
}

std::pair<float, float> AudioSystem::applyEffects(std::pair<float, float> stereoSample) 
{
    // Apply each effect in the chain to the stereo sample
    for (const auto& effect : m_effects) 
    {
        if (effect) { // Null check for safety
            stereoSample = effect->process(stereoSample);
        }
    }

    return stereoSample;
}

void AudioSystem::addEffect(std::shared_ptr<IEffect> effect) 
{
    if (!effect) {
        return; // Don't add null effects
    }
    
    // Check if the effect already exists in the vector
    auto it = std::find(m_effects.begin(), m_effects.end(), effect);
    
    if (it == m_effects.end()) 
    {
        // Effect not found, so add it to the vector
        m_effects.push_back(effect);
    } 
}


void AudioSystem::resetEffects() 
{
    for (const auto& effect : m_effects) 
    {
        if (effect) { // Null check for safety
            effect->reset();
        }
    }
}
