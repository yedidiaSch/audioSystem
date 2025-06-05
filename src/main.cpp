#include <iostream>
#include <thread>
#include <chrono>
#include "audioSystem.h"
#include "audioDevice.h"
#include "Midi/MidiDevice.h"
#include "notes.h"
#include "AudioSystemAdapter.h"



int main() 
{
    float           sampleRate      = 44100.0f;
    unsigned int    bufferFrames    = 512;

    AudioSystem audioSystem(sampleRate);
    AudioConfig config;
    config.waveform = "sine";
    config.effects = {"delay", "lowpass"};
    audioSystem.configure(config);
    AudioDevice audioDevice(&audioSystem, sampleRate, bufferFrames);

    // Create the AudioSystemAdapter
    AudioSystemAdapter audioSystemAdapter(&audioSystem);

    // Start the audio stream
    audioDevice.start();

    // Add a longer delay to let the audio system initialize fully
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    // Init the midi device
    MidiDevice midiDevice(1);
    
    // Attach the AudioSystemAdapter to the MidiDevice before starting
    midiDevice.attach(&audioSystemAdapter);
    
    // Now start the MIDI device
    midiDevice.start();
    
    std::cout << "Audio system running. Press Enter to stop..." << std::endl;
    
    // Main program loop - keep system alive while waiting for input
    while (!std::cin.get()) {
        // Process audio in small chunks to prevent underruns
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Shutting down..." << std::endl;
    
    // Stop devices in reverse order of starting
    midiDevice.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    audioDevice.stop();
    
    // Final sleep to ensure all resources are released
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
