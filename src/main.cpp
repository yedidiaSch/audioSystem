#include <iostream>
#include <thread>
#include <chrono>
#include "audioSystem.h"
#include "audioDevice.h"
#include "notes.h"




int main() 
{
    float sampleRate            = 44100.0f;
    unsigned int bufferFrames   = 512;


    AudioSystem audioSystem(sampleRate);
    AudioDevice audioDevice(&audioSystem, sampleRate, bufferFrames);


    // Start the audio stream
    audioDevice.start();


    // Simulate note sequence
    audioSystem.triggerNote(C3_FREQ); 
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    audioSystem.triggerNoteOff();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    audioSystem.triggerNote(D_SHARP3_FREQ);  
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    audioSystem.triggerNoteOff();

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    audioSystem.triggerNote(G3_FREQ);  
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    audioSystem.triggerNoteOff();

    // Stop the audio stream
    audioDevice.stop();


    return 0;
}
