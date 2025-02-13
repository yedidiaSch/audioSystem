#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <iostream>
#include <vector>
#include <memory>
#include "RtAudio.h"
#include "audioSystem.h"
#include "Effects/IEffect.h"

class AudioDevice 
{

public:

    AudioDevice         (AudioSystem* audioSystem, float sampleRate, unsigned int bufferFrames);
    ~AudioDevice        ();

    void start          ();
    void stop           ();

private:
    static int audioCallback    (void* outputBuffer,
                                 void* inputBuffer,
                                 unsigned int nBufferFrames,
                                 double streamTime,
                                 RtAudioStreamStatus status,
                                 void* userData);

    AudioSystem*        itsAudioSystem;
    RtAudio             m_dac;
    float               m_sampleRate;
    unsigned int        m_bufferFrames;
};

#endif
