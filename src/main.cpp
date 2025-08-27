#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "audioSystem.h"
#include "audioDevice.h"
#include "Midi/MidiDevice.h"
#include "notes.h"
#include "AudioSystemAdapter.h"
#include "ConfigReader.h"

/**
 * @brief Initialize and configure the audio system from XML configuration
 * @param config AudioConfig loaded from XML file
 * @return Configured AudioSystem instance
 */
AudioSystem initializeAudioSystem(const AudioConfig& config) {
    AudioSystem audioSystem(config.sampleRate);
    
    // Configure with loaded settings
    audioSystem.configure(config);
    
    return audioSystem;
}

/**
 * @brief Main application entry point
 */
int main() 
{
    try {
        std::cout << "Initializing Audio Synthesis System..." << std::endl;
        
        // Load configuration from XML file
        ConfigReader configReader;
        AudioConfig config;
        
        try {
            config = configReader.loadConfig("config.xml");
            std::cout << "Loaded configuration from config.xml" << std::endl;
            std::cout << "  Waveform: " << config.waveform << std::endl;
            std::cout << "  Sample Rate: " << config.sampleRate << " Hz" << std::endl;
            std::cout << "  Buffer Frames: " << config.bufferFrames << std::endl;
            std::cout << "  MIDI Port: " << config.midiPort << std::endl;
            std::cout << "  Effects: ";
            for (size_t i = 0; i < config.effects.size(); ++i) {
                std::cout << config.effects[i];
                if (i < config.effects.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Warning: Failed to load config.xml (" << e.what() << "), using defaults" << std::endl;
            // config will use default values from constructor
        }
        
        // Initialize audio system with configuration
        AudioSystem audioSystem = initializeAudioSystem(config);
        AudioDevice audioDevice(&audioSystem, config.sampleRate, config.bufferFrames);

        // Create the AudioSystemAdapter for MIDI integration
        AudioSystemAdapter audioSystemAdapter(&audioSystem);

        // Start the audio stream
        std::cout << "Starting audio device..." << std::endl;
        audioDevice.start();

        // Add a delay to let the audio system initialize fully
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        // Initialize MIDI device
        std::cout << "Initializing MIDI device..." << std::endl;
        MidiDevice midiDevice(config.midiPort);
        
        // Attach the AudioSystemAdapter to the MidiDevice before starting
        midiDevice.attach(&audioSystemAdapter);
        
        // Start MIDI processing
        std::cout << "Starting MIDI device..." << std::endl;
        midiDevice.start();
        
        std::cout << "Audio system ready! Play your MIDI controller or press Enter to stop..." << std::endl;
        
        // Main program loop - keep system alive while waiting for input
        std::cin.get();
        
        std::cout << "Shutting down gracefully..." << std::endl;
        
        // Stop devices in reverse order of starting
        midiDevice.stop();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        audioDevice.stop();
        
        // Final sleep to ensure all resources are released
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "Program terminated normally." << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}
