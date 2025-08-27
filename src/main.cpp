#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "audioSystem.h"
#include "audioDevice.h"
#include "Midi/MidiDevice.h"
#include "notes.h"
#include "AudioSystemAdapter.h"

/**
 * @brief Initialize and configure the audio system
 * @param sampleRate Sample rate for audio processing
 * @return Configured AudioSystem instance
 */
AudioSystem initializeAudioSystem(float sampleRate) {
    AudioSystem audioSystem(sampleRate);
    
    // Configure with a nice default setup
    AudioConfig config;
    config.waveform = "sine";
    config.effects = {"delay", "lowpass"};
    audioSystem.configure(config);
    
    return audioSystem;
}

/**
 * @brief Main application entry point
 */
int main() 
{
    try {
        // Configuration constants
        constexpr float SAMPLE_RATE = 44100.0f;
        constexpr unsigned int BUFFER_FRAMES = 512;
        constexpr int MIDI_PORT = 1;
        
        std::cout << "Initializing Audio Synthesis System..." << std::endl;
        
        // Initialize audio system
        AudioSystem audioSystem = initializeAudioSystem(SAMPLE_RATE);
        AudioDevice audioDevice(&audioSystem, SAMPLE_RATE, BUFFER_FRAMES);

        // Create the AudioSystemAdapter for MIDI integration
        AudioSystemAdapter audioSystemAdapter(&audioSystem);

        // Start the audio stream
        std::cout << "Starting audio device..." << std::endl;
        audioDevice.start();

        // Add a delay to let the audio system initialize fully
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        // Initialize MIDI device
        std::cout << "Initializing MIDI device..." << std::endl;
        MidiDevice midiDevice(MIDI_PORT);
        
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
