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
    float sampleRate            = 44100.0f;
    unsigned int bufferFrames   = 512;


    AudioSystem audioSystem(sampleRate);
    AudioDevice audioDevice(&audioSystem, sampleRate, bufferFrames);


    // Start the audio stream
    audioDevice.start();

    // Init the midi device
    MidiDevice midiDevice(1);
    midiDevice.start();
 

    // Create the AudioSystemAdapter
    AudioSystemAdapter audioSystemAdapter(&audioSystem);
    // Attach the AudioSystemAdapter to the MidiDevice
    midiDevice.attach(&audioSystemAdapter);


    // Sleep for 50 seconds to allow MIDI input
    std::cout << "Press any key to stop..." << std::endl;
    std::cin.get();
    // Stop the MIDI device
    midiDevice.stop();
    // Stop the audio device
    audioDevice.stop();
    // Sleep for a short duration to ensure all audio is processed
    std::this_thread::sleep_for(std::chrono::milliseconds(100));


    return 0;
}
