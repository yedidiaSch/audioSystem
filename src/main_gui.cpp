#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <memory>
#include "audioSystem.h"
#include "audioDevice.h"
#include "Midi/MidiDevice.h"
#include "AudioSequencer.h"
#include "notes.h"
#include "AudioSystemAdapter.h"
#include "ConfigReader.h"
#include "AudioConfig.h"
//#include "AudioGUI.h"

/**
 * @brief Initialize and configure the audio system from configuration
 * @param config AudioConfig structure
 * @return Shared pointer to configured AudioSystem instance
 */
std::shared_ptr<AudioSystem> initializeAudioSystem(const AudioConfig& config) {
    auto audioSystem = std::make_shared<AudioSystem>(config.sampleRate);
    
    // Configure with loaded settings
    audioSystem->configure(config);
    
    return audioSystem;
}

/**
 * @brief Run the traditional console-based audio system (fallback mode)
 * @param audioSystem Shared pointer to the audio system
 * @param config Audio configuration
 */
void runConsoleMode(std::shared_ptr<AudioSystem> audioSystem, const AudioConfig& config) {
    AudioDevice audioDevice(audioSystem.get(), config.sampleRate, config.bufferFrames);

    // Create the AudioSystemAdapter for MIDI integration
    AudioSystemAdapter audioSystemAdapter(audioSystem.get());

    // Start the audio stream
    std::cout << "Starting audio device..." << std::endl;
    audioDevice.start();

    // Add a delay to let the audio system initialize fully
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    // Choose input mode based on configuration
    if (config.inputMode == "sequencer") {
        // Sequencer mode - for testing without MIDI controller
        std::cout << "Initializing audio sequencer..." << std::endl;
        AudioSequencer sequencer;
        
        // Attach the AudioSystemAdapter to the sequencer
        sequencer.attach(&audioSystemAdapter);
        
        std::cout << "🎵 Audio system ready in SEQUENCER mode!" << std::endl;
        std::cout << "Playing " << config.sequenceType << " sequence..." << std::endl;
        std::cout << "Press Enter to replay, or Ctrl+C to stop." << std::endl;
        
        // Play the initial sequence
        sequencer.playSequenceOnce(config.sequenceType);
        
        // Main program loop - replay sequence when user presses Enter
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input.empty()) {
                // Empty input (just Enter pressed) - replay sequence
                std::cout << "\n🔄 Replaying sequence..." << std::endl;
                sequencer.playSequenceOnce(config.sequenceType);
            } else {
                // Any other input - exit
                break;
            }
        }
        
        std::cout << "Shutting down sequencer..." << std::endl;
        
    } else {
        // MIDI mode - traditional MIDI controller input
        std::cout << "Initializing MIDI device..." << std::endl;
        MidiDevice midiDevice(config.midiPort);
        
        // Attach the AudioSystemAdapter to the MidiDevice before starting
        midiDevice.attach(&audioSystemAdapter);
        
        // Start MIDI processing
        std::cout << "Starting MIDI device..." << std::endl;
        midiDevice.start();
        
        std::cout << "🎹 Audio system ready in MIDI mode! Play your MIDI controller or press Enter to stop..." << std::endl;
        
        // Main program loop - keep system alive while waiting for input
        std::cin.get();
        
        std::cout << "Shutting down MIDI device..." << std::endl;
        midiDevice.stop();
    }
    
    audioDevice.stop();
}

/**
 * @brief Main application entry point
 */
int main(int argc, char* argv[]) 
{
    try {
        std::cout << "Initializing Audio Synthesis System..." << std::endl;
        
        // Load initial configuration from XML file with automatic fallback to defaults
        ConfigReader    configReader;
        std::string     configPath = "config/config.xml";
        AudioConfig     config = configReader.loadConfigWithFallback(configPath);
        
        // Initialize audio system with configuration
        auto audioSystem = initializeAudioSystem(config);
        
        // Check if GUI mode is requested (default) or console mode
        bool useGUI = true;
        if (argc > 1) {
            std::string arg = argv[1];
            if (arg == "--console" || arg == "-c") {
                useGUI = false;
            }
        }
        
        if (useGUI) {
            std::cout << "GUI mode not yet fully implemented. Falling back to console mode." << std::endl;
            runConsoleMode(audioSystem, config);
            /*
            std::cout << "Starting GUI mode..." << std::endl;
            
            // Create and run the GUI
            AudioGUI gui(audioSystem);
            if (!gui.initialize()) {
                std::cerr << "Failed to initialize GUI. Falling back to console mode." << std::endl;
                runConsoleMode(audioSystem, config);
            } else {
                std::cout << "🎛️ Audio System Control Panel ready!" << std::endl;
                std::cout << "Use the GUI to control audio parameters." << std::endl;
                std::cout << "Close the window to exit." << std::endl;
                
                gui.run();
            }
            */
        } else {
            std::cout << "Starting console mode..." << std::endl;
            runConsoleMode(audioSystem, config);
        }
        
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