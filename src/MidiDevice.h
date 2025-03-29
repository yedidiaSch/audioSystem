#pragma once

#include "RtMidi.h"
#include "AudioSystem.h"
#include <vector>
#include <string>

class MidiDevice {
public:
    // Constructor with optional port selection
    MidiDevice                          (AudioSystem* audioSystem, int portNumber = 0);
    
    // Destructor
    ~MidiDevice                         ();
    
    // Methods for controlling the MIDI device
    void start                          ();

    void stop                           ();
    
    // Port management
    std::vector<std::string> listPorts  ();

    bool changePort                     (unsigned int portNumber);
    
private:
    RtMidiIn midiin;
    AudioSystem* audioSystem;
    bool isInitialized;
    
    // Static callback function
    static void midiCallback            (double timeStamp, std::vector<unsigned char>* message, void* userData);
    
    // MIDI message handlers
    void handleControlChange            (unsigned char channel, unsigned char controller, unsigned char value);
    
    void handlePitchBend                (unsigned char channel, int value);
};