#include "SoundController.h"
#include <iostream>
#include <chrono>
#include <cmath>

SoundController::SoundController(AudioSystemManager& audioManager, ConfigurationManager& configManager)
    : audioSystemManager(audioManager), configurationManager(configManager) {
    
    // Initialize frequency from configuration
    currentFrequency = configManager.getConfig().defaultFrequency;
    
    std::cout << "🎵 Sound Controller initialized" << std::endl;
}

SoundController::~SoundController() {
    // Signal any background threads to stop
    shouldStop.store(true);
    currentlyPlaying.store(false);
    
    // Stop audio first
    if (audioSystemManager.isRunning()) {
        audioSystemManager.triggerNoteOff();
    }
    
    // Give background threads a moment to finish naturally
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void SoundController::playTestTone(int durationMs) {
    if (!audioSystemManager.isRunning()) {
        std::cout << "❌ Please start the audio system first!" << std::endl;
        return;
    }
    
    if (currentlyPlaying.load()) {
        stopSound();
        // Wait a bit for the previous playback to stop
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "🎵 Playing test tone: " << configurationManager.getConfig().waveform 
              << " wave at " << currentFrequency << "Hz for " << durationMs << "ms" << std::endl;
    
    currentlyPlaying.store(true);
    notifyPlaybackStateChange();
    
    // Trigger the note
    audioSystemManager.triggerNote(currentFrequency);
    
    // Start detached thread for auto-stop (safer than joinable thread)
    std::thread([this, durationMs]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
        if (currentlyPlaying.load() && !shouldStop.load()) {
            // Just stop the audio, don't try to manage thread from within thread
            shouldStop.store(false); // Reset flag first
            currentlyPlaying.store(false);
            audioSystemManager.triggerNoteOff();
            notifyPlaybackStateChange();
            std::cout << "⏹️ Auto-stop completed" << std::endl;
        }
    }).detach();
}

void SoundController::stopSound() {
    if (!currentlyPlaying.load()) {
        return;
    }
    
    std::cout << "⏹️ Stopping sound" << std::endl;
    
    shouldStop.store(true);
    currentlyPlaying.store(false);
    
    audioSystemManager.triggerNoteOff();
    notifyPlaybackStateChange();
}

void SoundController::playDemoSequence(const std::string& sequenceType) {
    if (!audioSystemManager.isRunning()) {
        std::cout << "❌ Please start the audio system first!" << std::endl;
        return;
    }
    
    if (currentlyPlaying.load()) {
        stopSound();
        // Wait a bit for the previous playback to stop
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "🎵 Playing demo sequence: " << sequenceType << " pattern" << std::endl;
    
    currentlyPlaying.store(true);
    notifyPlaybackStateChange();
    
    // Start detached thread for sequence playback (safer approach)
    std::thread([this, sequenceType]() {
        shouldStop.store(false);
        
        try {
            if (sequenceType == "scale") {
                playSequenceScale();
            } else if (sequenceType == "chord") {
                playSequenceChord();
            } else if (sequenceType == "melody") {
                playSequenceMelody();
            } else {
                playSequenceDemo();
            }
        } catch (const std::exception& e) {
            std::cerr << "❌ Error during sequence playback: " << e.what() << std::endl;
        }
        
        if (!shouldStop.load()) {
            std::cout << "🎼 Demo sequence completed" << std::endl;
        }
        
        currentlyPlaying.store(false);
        notifyPlaybackStateChange();
    }).detach();
}

void SoundController::playSequenceScale() {
    // Play a C major scale
    const float notes[] = {261.63f, 293.66f, 329.63f, 349.23f, 392.00f, 440.00f, 493.88f, 523.25f};
    const int noteCount = sizeof(notes) / sizeof(notes[0]);
    
    for (int i = 0; i < noteCount && !shouldStop.load(); ++i) {
        audioSystemManager.triggerNote(notes[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        audioSystemManager.triggerNoteOff();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void SoundController::playSequenceChord() {
    // Play chord progression: C - F - G - C
    const float chordNotes[][3] = {
        {261.63f, 329.63f, 392.00f}, // C major
        {349.23f, 440.00f, 523.25f}, // F major
        {392.00f, 493.88f, 587.33f}, // G major
        {261.63f, 329.63f, 392.00f}  // C major
    };
    
    for (int chord = 0; chord < 4 && !shouldStop.load(); ++chord) {
        // Play each note in the chord with slight offset
        for (int note = 0; note < 3; ++note) {
            audioSystemManager.triggerNote(chordNotes[chord][note]);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        audioSystemManager.triggerNoteOff();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

void SoundController::playSequenceMelody() {
    // Play "Twinkle Twinkle Little Star" melody
    const float melody[] = {
        261.63f, 261.63f, 392.00f, 392.00f, 440.00f, 440.00f, 392.00f,
        349.23f, 349.23f, 329.63f, 329.63f, 293.66f, 293.66f, 261.63f
    };
    const int noteCount = sizeof(melody) / sizeof(melody[0]);
    
    for (int i = 0; i < noteCount && !shouldStop.load(); ++i) {
        audioSystemManager.triggerNote(melody[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        audioSystemManager.triggerNoteOff();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void SoundController::playSequenceDemo() {
    // Play the original demo sequence: A4, C5, E5, G5
    const float notes[] = {440.0f, 523.25f, 659.25f, 783.99f};
    const int noteCount = sizeof(notes) / sizeof(notes[0]);
    
    for (int i = 0; i < noteCount && !shouldStop.load(); ++i) {
        audioSystemManager.triggerNote(notes[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        audioSystemManager.triggerNoteOff();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void SoundController::notifyPlaybackStateChange() {
    if (playbackStateCallback) {
        playbackStateCallback(currentlyPlaying.load());
    }
}