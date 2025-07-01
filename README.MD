# Audio Synthesis System

A real-time audio synthesis engine that responds to MIDI input, generating digital audio with customizable effects chains.

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## Features
- Real-time audio processing using RtAudio
- MIDI input support via RtMidi
- Modular effects chain system
- Configurable sample rate and buffer size
- Multiple waveforms including sine, square, sawtooth and triangle
- Built-in delay and low-pass filter effects
- Simple configuration API for selecting waves and effects

## Requirements
- C++14 compatible compiler
- RtAudio library
- RtMidi library
- ALSA development libraries (Linux)
- pthread library

## Setup

### Installation of Dependencies
```
# Install ALSA development libraries
sudo apt-get install libasound2-dev

# Install RtMidi
sudo apt-get install librtmidi-dev

# Build and install RtAudio (if not available as package)
git clone https://github.com/thestk/rtaudio.git
cd rtaudio
mkdir build && cd build
cmake ..
make
sudo make install
```

### Usage
1. Connect a MIDI input device to your computer
2. Run the application:

```
./bin/audioApp
```

3. Play notes on your MIDI controller to generate audio
4. Press any key in the terminal window to stop the application

### Configuration Example
The audio system can be configured at runtime with a simple structure:

```cpp
AudioConfig cfg;
cfg.waveform = "sawtooth";
cfg.effects = {"delay", "lowpass"};
audioSystem.configure(cfg);
```

This selects a sawtooth oscillator and adds delay and low-pass filter effects to the processing chain.
 

## Embedded FreeRTOS STM32 Example
See `embedded/` for a reference port using FreeRTOS and the STM32 HAL. It shows how to stream synthesized audio via DMA to I2S or DAC. The example builds a sine lookup table and allows custom waveform or effect callbacks via `AudioTaskInit`.

### Future Improvements
- Add more waveform types (sine, sawtooth, triangle)
- Implement ADSR envelope for amplitude shaping
- Add polyphony support (multiple notes at once)
- Create more audio effects (delay, reverb, filters)
- Add GUI for parameter control
- Support for audio file playback and recording

### License
This project is available under the MIT License.

### Contributing
Contributions are welcome! Please feel free to submit a Pull Request.









