#include "AudioSystemManager.h"
#include "ConfigReader.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>

AudioSystemManager::AudioSystemManager(const AudioConfig& config) 
    : currentConfig(config), audioDeviceStarted(false) {
    initializeAudioSystem(config);
    std::cout << "🎛️ Audio System Manager initialized" << std::endl;
}

AudioSystemManager::~AudioSystemManager() {
    if (audioDeviceStarted) {
        stop();
    }
}

void AudioSystemManager::initializeAudioSystem(const AudioConfig& config) {
    // Initialize audio system
    audioSystem = std::make_shared<AudioSystem>(config.sampleRate);
    audioSystem->configure(config);
    
    // Initialize adapter for unified interface
    adapter = std::make_unique<AudioSystemAdapter>(audioSystem.get());
    
    // Initialize audio device
    audioDevice = std::make_unique<AudioDevice>(
        audioSystem.get(), 
        config.sampleRate, 
        config.bufferFrames
    );
    
    currentConfig = config;
}

bool AudioSystemManager::start() {
    if (audioDeviceStarted || !audioDevice) {
        return false;
    }
    
    try {
        std::cout << "🔊 Starting audio device..." << std::endl;
        audioDevice->start();
        audioDeviceStarted = true;
        
        // Add a small delay to let the audio system initialize
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::cout << "✅ Audio system started successfully!" << std::endl;
        notifyStateChange();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Failed to start audio system: " << e.what() << std::endl;
        audioDeviceStarted = false;
        notifyStateChange();
        return false;
    }
}

void AudioSystemManager::stop() {
    if (!audioDeviceStarted || !audioDevice) {
        return;
    }
    
    try {
        std::cout << "🔇 Stopping audio device..." << std::endl;
        
        // Stop any currently playing notes first
        if (audioSystem) {
            audioSystem->triggerNoteOff();
        }
        
        audioDevice->stop();
        audioDeviceStarted = false;
        std::cout << "✅ Audio system stopped successfully!" << std::endl;
        notifyStateChange();
    } catch (const std::exception& e) {
        std::cerr << "❌ Failed to stop audio system: " << e.what() << std::endl;
    }
}

void AudioSystemManager::configure(const AudioConfig& config) {
    currentConfig = config;
    
    if (audioSystem) {
        audioSystem->configure(config);
        std::cout << "⚙️ Configuration applied to audio system" << std::endl;
    }
    
    // If sample rate or buffer size changed, we need to reinitialize
    if (config.sampleRate != currentConfig.sampleRate || 
        config.bufferFrames != currentConfig.bufferFrames) {
        
        bool wasRunning = audioDeviceStarted;
        if (wasRunning) {
            stop();
        }
        
        initializeAudioSystem(config);
        
        if (wasRunning) {
            start();
        }
    }
}

void AudioSystemManager::triggerNote(float frequency) {
    if (!audioSystem) {
        std::cerr << "❌ Audio system not initialized" << std::endl;
        return;
    }
    
    if (!audioDeviceStarted) {
        std::cout << "❌ Please start the audio system first!" << std::endl;
        return;
    }
    
    audioSystem->triggerNote(frequency);
    std::cout << "🎵 Note triggered at " << frequency << " Hz" << std::endl;
}

void AudioSystemManager::triggerNoteOff() {
    if (audioSystem) {
        audioSystem->triggerNoteOff();
        std::cout << "⏹️ Note stopped" << std::endl;
    }
}

void AudioSystemManager::notifyStateChange() {
    if (stateChangeCallback) {
        stateChangeCallback(audioDeviceStarted);
    }
}