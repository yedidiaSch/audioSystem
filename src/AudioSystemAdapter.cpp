#include "AudioSystemAdapter.h"

AudioSystemAdapter::AudioSystemAdapter(AudioSystem* pAudioSystem) : itsAudioSystem(pAudioSystem) 
{
    if (itsAudioSystem == nullptr) {
        throw std::invalid_argument("AudioSystem pointer cannot be null");
    }
}


void AudioSystemAdapter::update(void* params) 
{
    if (params == nullptr) {
        throw std::invalid_argument("Params pointer cannot be null");
    }
    
    // Cast params to MidiEvent
    const MidiEvent* event = static_cast<const MidiEvent*>(params);
    
    switch (event->type) {
        case MidiEventType::NOTE_ON:
            itsAudioSystem->triggerNote(event->value);  // Use the frequency value
            break;
            
        case MidiEventType::NOTE_OFF:
            itsAudioSystem->triggerNoteOff();
            break;
            
        // Handle other event types as needed
        default:
            break;
    }
}
