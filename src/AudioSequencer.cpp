#include "AudioSequencer.h"
#include <iostream>
#include <cmath>
#include <thread>
#include "notes.h"

// Musical note frequencies for easy reference
namespace Notes {
    constexpr float C4 = 261.63f;
    constexpr float D4 = 293.66f;
    constexpr float E4 = 329.63f;
    constexpr float F4 = 349.23f;
    constexpr float G4 = 392.00f;
    constexpr float A4 = 440.00f;
    constexpr float B4 = 493.88f;
    constexpr float C5 = 523.25f;
    constexpr float D5 = 587.33f;
    constexpr float E5 = 659.25f;
    constexpr float F5 = 698.46f;
    constexpr float G5 = 783.99f;
}

AudioSequencer::AudioSequencer() 
    : m_playing(false), m_currentSequenceType("demo") {
}

AudioSequencer::~AudioSequencer() {
    stopSequence();
}

void AudioSequencer::startSequence(const std::string& sequenceType) {
    if (m_playing) {
        stopSequence();
    }
    
    m_currentSequenceType = sequenceType;
    m_playing = true;
    
    // Start the thread using ThreadBase's start method
    start();
    
    std::cout << "🎵 Starting audio sequence: " << sequenceType << std::endl;
    std::cout << "   Press Enter to replay sequence or Ctrl+C to stop..." << std::endl;
}

void AudioSequencer::stopSequence() {
    if (m_playing) {
        m_playing = false;
        // Stop the thread using ThreadBase's stop method
        stop();
        sendNoteOff(); // Ensure any playing note is stopped
        std::cout << "🔇 Audio sequence stopped." << std::endl;
    }
}

bool AudioSequencer::isPlaying() const {
    return m_playing;
}

void AudioSequencer::playSequenceOnce(const std::string& sequenceType) {
    m_currentSequenceType = sequenceType;
    
    // Generate the appropriate sequence
    if (sequenceType == "scale") {
        m_currentSequence = generateMajorScale();
    } else if (sequenceType == "chord") {
        m_currentSequence = generateChordProgression();
    } else if (sequenceType == "melody") {
        m_currentSequence = generateSimpleMelody();
    } else {
        m_currentSequence = generateDemoSequence();
    }
    
    std::cout << "🎵 Playing " << sequenceType << " sequence..." << std::endl;
    
    // Play the sequence once
    for (const auto& note : m_currentSequence) {
        // Only break if we're in threaded mode and the thread should stop
        if (m_playing && !m_running) break;
        
        // Send note on
        sendNoteOn(note.frequency, note.velocity);
        std::cout << "♪ Playing: " << note.frequency << " Hz (MIDI ~" 
                  << frequencyToMidiNote(note.frequency) << ") for " 
                  << note.duration << "s" << std::endl;
        
        // Wait for note duration
        std::this_thread::sleep_for(std::chrono::milliseconds(
            static_cast<int>(note.duration * 1000)));
        
        // Send note off
        sendNoteOff();
        
        // Wait for pause if specified
        if (note.pauseAfter > 0.0f) {
            std::this_thread::sleep_for(std::chrono::milliseconds(
                static_cast<int>(note.pauseAfter * 1000)));
        }
    }
    
    std::cout << "✓ Sequence complete!" << std::endl;
}

void AudioSequencer::thread() {
    while (m_playing && m_running) {  // Use ThreadBase's m_running flag
        playSequenceOnce(m_currentSequenceType);
        
        if (m_running) {  // Use ThreadBase's m_running flag
            std::cout << "🔄 Sequence complete. Will repeat..." << std::endl;
            // Brief pause between repetitions
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    m_playing = false;
}

std::vector<SequenceNote> AudioSequencer::generateMajorScale() {
    return {
        SequenceNote(Notes::C4, 0.5f, 0.8f, 0.1f),  // C
        SequenceNote(Notes::D4, 0.5f, 0.8f, 0.1f),  // D
        SequenceNote(Notes::E4, 0.5f, 0.8f, 0.1f),  // E
        SequenceNote(Notes::F4, 0.5f, 0.8f, 0.1f),  // F
        SequenceNote(Notes::G4, 0.5f, 0.8f, 0.1f),  // G
        SequenceNote(Notes::A4, 0.5f, 0.8f, 0.1f),  // A
        SequenceNote(Notes::B4, 0.5f, 0.8f, 0.1f),  // B
        SequenceNote(Notes::C5, 1.0f, 0.9f, 0.5f)   // C (octave)
    };
}

std::vector<SequenceNote> AudioSequencer::generateChordProgression() {
    return {
        // C major chord (C-E-G played as arpeggios)
        SequenceNote(Notes::C4, 0.3f, 0.8f, 0.05f),
        SequenceNote(Notes::E4, 0.3f, 0.8f, 0.05f),
        SequenceNote(Notes::G4, 0.3f, 0.8f, 0.05f),
        SequenceNote(Notes::C5, 0.5f, 0.9f, 0.2f),
        
        // F major chord (F-A-C)
        SequenceNote(Notes::F4, 0.3f, 0.8f, 0.05f),
        SequenceNote(Notes::A4, 0.3f, 0.8f, 0.05f),
        SequenceNote(Notes::C5, 0.3f, 0.8f, 0.05f),
        SequenceNote(Notes::F5, 0.5f, 0.9f, 0.2f),
        
        // G major chord (G-B-D)
        SequenceNote(Notes::G4, 0.3f, 0.8f, 0.05f),
        SequenceNote(Notes::B4, 0.3f, 0.8f, 0.05f),
        SequenceNote(Notes::D5, 0.3f, 0.8f, 0.05f),
        SequenceNote(Notes::G5, 0.5f, 0.9f, 0.2f),
        
        // Back to C major
        SequenceNote(Notes::C5, 1.0f, 1.0f, 0.5f)
    };
}

std::vector<SequenceNote> AudioSequencer::generateSimpleMelody() {
    // "Twinkle Twinkle Little Star" pattern
    return {
        SequenceNote(Notes::C4, 0.4f, 0.8f, 0.1f),  // Twin-
        SequenceNote(Notes::C4, 0.4f, 0.8f, 0.1f),  // kle
        SequenceNote(Notes::G4, 0.4f, 0.8f, 0.1f),  // twin-
        SequenceNote(Notes::G4, 0.4f, 0.8f, 0.1f),  // kle
        SequenceNote(Notes::A4, 0.4f, 0.8f, 0.1f),  // lit-
        SequenceNote(Notes::A4, 0.4f, 0.8f, 0.1f),  // tle
        SequenceNote(Notes::G4, 0.8f, 0.9f, 0.2f),  // star
        
        SequenceNote(Notes::F4, 0.4f, 0.8f, 0.1f),  // How
        SequenceNote(Notes::F4, 0.4f, 0.8f, 0.1f),  // I
        SequenceNote(Notes::E4, 0.4f, 0.8f, 0.1f),  // won-
        SequenceNote(Notes::E4, 0.4f, 0.8f, 0.1f),  // der
        SequenceNote(Notes::D4, 0.4f, 0.8f, 0.1f),  // what
        SequenceNote(Notes::D4, 0.4f, 0.8f, 0.1f),  // you
        SequenceNote(Notes::C4, 1.0f, 0.9f, 0.5f)   // are
    };
}

std::vector<SequenceNote> AudioSequencer::generateDemoSequence() {
    return {
        // Quick ascending scale
        SequenceNote(Notes::C4, 0.2f, 0.6f, 0.05f),
        SequenceNote(Notes::D4, 0.2f, 0.7f, 0.05f),
        SequenceNote(Notes::E4, 0.2f, 0.8f, 0.05f),
        SequenceNote(Notes::F4, 0.2f, 0.8f, 0.05f),
        SequenceNote(Notes::G4, 0.2f, 0.9f, 0.05f),
        SequenceNote(Notes::A4, 0.2f, 0.9f, 0.05f),
        SequenceNote(Notes::B4, 0.2f, 1.0f, 0.05f),
        SequenceNote(Notes::C5, 0.5f, 1.0f, 0.3f),
        
        // Chord arpeggios with varying velocities
        SequenceNote(Notes::C4, 0.3f, 0.9f, 0.1f),
        SequenceNote(Notes::E4, 0.3f, 0.7f, 0.1f),
        SequenceNote(Notes::G4, 0.3f, 0.8f, 0.1f),
        SequenceNote(Notes::C5, 0.6f, 1.0f, 0.3f),
        
        // Descending pattern with different rhythms
        SequenceNote(Notes::B4, 0.25f, 0.8f, 0.1f),
        SequenceNote(Notes::A4, 0.25f, 0.8f, 0.1f),
        SequenceNote(Notes::G4, 0.5f, 0.9f, 0.1f),
        SequenceNote(Notes::F4, 0.25f, 0.7f, 0.1f),
        SequenceNote(Notes::E4, 0.25f, 0.7f, 0.1f),
        SequenceNote(Notes::D4, 0.5f, 0.8f, 0.1f),
        SequenceNote(Notes::C4, 1.0f, 1.0f, 0.5f)
    };
}

void AudioSequencer::sendNoteOn(float frequency, float velocity) {
    MidiEvent event;
    event.type = MidiEventType::NOTE_ON;
    event.channel = 0;
    event.data1 = static_cast<unsigned char>(frequencyToMidiNote(frequency));
    event.data2 = static_cast<unsigned char>(velocity * 127);
    event.value = static_cast<int>(frequency);  // Store frequency for processing
    
    // Notify observers (AudioSystemAdapter) about the note event
    notify(&event);
}

void AudioSequencer::sendNoteOff() {
    MidiEvent event;
    event.type = MidiEventType::NOTE_OFF;
    event.channel = 0;
    event.data1 = 0;  // Note number not important for note off in this context
    event.data2 = 0;  // Velocity not important for note off
    event.value = 0;
    
    // Notify observers about the note off event
    notify(&event);
}

int AudioSequencer::frequencyToMidiNote(float frequency) {
    // Convert frequency to MIDI note number using A4 = 440 Hz = note 69
    if (frequency <= 0) return 0;
    
    float noteNumber = 69.0f + 12.0f * log2f(frequency / 440.0f);
    return static_cast<int>(round(noteNumber));
}
