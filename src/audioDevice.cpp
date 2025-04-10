#include "audioDevice.h"

AudioDevice::AudioDevice(AudioSystem* audioSystem, float sampleRate, unsigned int bufferFrames) :
                                                                    itsAudioSystem  (audioSystem),
                                                                    m_sampleRate    (sampleRate),
                                                                    m_bufferFrames  (bufferFrames) 
{
    if (m_dac.getDeviceCount() < 1) 
    {
        std::cerr << "No audio devices found!" << std::endl;
        exit(EXIT_FAILURE);
    }

    RtAudio::StreamParameters parameters;
    parameters.deviceId = m_dac.getDefaultOutputDevice();
    parameters.nChannels = 2;  // Stereo output (2 channels)
    parameters.firstChannel = 0;

    int error = m_dac.openStream(&parameters, nullptr, RTAUDIO_FLOAT32, 
                                static_cast<unsigned int>(sampleRate), &bufferFrames, 
                                &AudioDevice::audioCallback, this);
    if (error) 
    {
        std::cerr << "Failed to open audio stream!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void AudioDevice::start() 
{
    int error = m_dac.startStream();
    if (error) 
    {
        std::cerr << "Failed to start audio stream!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void AudioDevice::stop() 
{
    if (m_dac.isStreamRunning()) m_dac.stopStream();
}

AudioDevice::~AudioDevice() 
{
    if (m_dac.isStreamOpen()) m_dac.closeStream();
}

int AudioDevice::audioCallback(void* outputBuffer, void* /*inputBuffer*/, unsigned int nBufferFrames,
                                double /*streamTime*/, RtAudioStreamStatus /*status*/, void* userData) 
{
    auto* device = static_cast<AudioDevice*>(userData);
    float* buffer = static_cast<float*>(outputBuffer);

    for (unsigned int i = 0; i < nBufferFrames; i++) 
    {
        // Get the next stereo sample from the AudioSystem
        std::pair<float, float> stereoSample = device->itsAudioSystem->getNextSample();
    
        // Write the processed stereo sample to the buffer
        buffer[2 * i] = stereoSample.first;  // Left channel
        buffer[2 * i + 1] = stereoSample.second;  // Right channel
    }

    return 0;
}
