# Audio core library sources
set(AUDIO_CORE_SOURCES
    ConfigReader.cpp
    audioSystem.cpp
    audioDevice.cpp
    AudioSequencer.cpp
    AudioSystemAdapter.cpp
    Midi/MidiDevice.cpp
    Effects/DelayEffect.cpp
    Effects/IEffect.cpp
    Effects/LowPassEffect.cpp
    Effects/OctaveEffect.cpp
    Waves/SineWave.cpp
    Waves/SquareWave.cpp
    Waves/SawtoothWave.cpp
    Waves/TriangleWave.cpp
    Envelope/ADSREnvelope.cpp
)

# Create object library for audio core
add_library(audio_core OBJECT ${AUDIO_CORE_SOURCES})

target_include_directories(audio_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${RTAUDIO_INCLUDE_DIRS}
    ${RTMIDI_INCLUDE_DIRS}
    ${LIBXML2_INCLUDE_DIR}
)

target_compile_options(audio_core PRIVATE
    ${RTAUDIO_CFLAGS_OTHER}
    ${RTMIDI_CFLAGS_OTHER}
)