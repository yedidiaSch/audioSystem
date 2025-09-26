# AudioSystem Copilot Instructions

## Project Overview
This is a real-time audio synthesis engine with dual interfaces (console + GUI), supporting MIDI input and built-in sequencer modes. The system uses modular architecture with Observer patterns for MIDI events, configurable effects chains, and XML-based configuration.

## Architecture Deep Dive

### Core Audio Flow
- **AudioSystem** (`src/Core/audioSystem.h`): Core synthesis engine with frequency-based oscillation and effects chain
- **AudioDevice** (`src/Core/audioDevice.h`): RtAudio wrapper handling real-time callbacks and hardware interface  
- **AudioSystemAdapter** (`src/Adapters/`): Observer pattern bridge converting MIDI events to audio system calls
- Audio flows: `MidiDevice` → `AudioSystemAdapter` → `AudioSystem` → `AudioDevice` → Hardware

### Key Patterns
- **Strategy Pattern**: Interchangeable waveforms (`IWave`) and effects (`IEffect`) via polymorphic interfaces
- **Observer Pattern**: MIDI events propagated through `Subject/IObserver` (see `utilities/` for implementation)
- **Adapter Pattern**: `AudioSystemAdapter` decouples MIDI processing from audio synthesis
- **Configuration-driven**: Runtime waveform/effects selection via `AudioConfig` struct

### Threading Model
- **Real-time audio thread**: RtAudio callback in `AudioDevice::audioCallback()` - NEVER block here
- **GUI thread**: Main thread running Dear ImGui render loop in `main_gui.cpp`
- **Background threads**: Used for demo sequences and config loading
- **Thread safety**: Audio system is single-threaded by design; GUI controls trigger reconfigurations

## Build System

### Primary: CMake (Recommended)
```bash
./build.sh --debug    # Debug build with submodule handling
./build.sh --release  # Optimized build (default)
```

### VS Code Tasks
- Use CMake tasks (`CMake: Build All`, `Run GUI App`) over legacy manual builds
- Build dependencies: `CMake: Configure` runs automatically before builds
- Debug/run: Use integrated tasks rather than terminal commands

### Key Dependencies
- **RtAudio/RtMidi**: Real-time audio/MIDI (system packages preferred)
- **LibXML2**: Configuration parsing (`config/config.xml`)
- **Dear ImGui**: GUI framework (via `guiBase_cpp` submodule)
- **ALSA**: Linux audio drivers

## Configuration System

### XML Structure (`config/config.xml`)
```xml
<audioSystemConfig>
  <waveform><type>sine|square|sawtooth|triangle</type></waveform>
  <effects>
    <effect>delay</effect>    <!-- Order matters for effects chain -->
    <effect>lowpass</effect>
  </effects>
  <input><mode>midi|sequencer</mode></input>
</audioSystemConfig>
```

### Runtime Configuration
```cpp
AudioConfig cfg;
cfg.waveform = "sawtooth";
cfg.effects = {"delay", "lowpass"};
audioSystem.configure(cfg);  // Rebuilds entire effects chain
```

## Development Patterns

### Adding New Waveforms
1. Inherit from `IWave` interface (`src/Waves/IWave.h`)
2. Implement `generate(float phase)` method returning [-1.0, 1.0] 
3. Add case-insensitive alias in `audioSystem.cpp` waveform factory
4. Update `config.xml` examples and waveform selector in GUI

### Adding New Effects  
1. Inherit from `IEffect` interface (`src/Effects/IEffect.h`)
2. Implement `process(std::pair<float, float> stereoSample)` 
3. Add stateful effects with proper `reset()` implementation
4. Register in `audioSystem.cpp` effects factory with aliases
5. Effects are applied in XML order - consider placement impact

### MIDI Event Handling
- MIDI events flow through Observer pattern: `MidiDevice` → `AudioSystemAdapter` → `AudioSystem`
- New MIDI event types: Add to `MidiEvent.h` enum, handle in `AudioSystemAdapter::update()`
- For complex MIDI features, extend the adapter rather than modifying core audio system

### GUI Development
- GUI built on `guiBase_cpp` submodule wrapping Dear ImGui
- Real-time controls: Modify `AudioConfig` and call `audioSystem->configure()` 
- Audio system lifecycle: Start/stop via `AudioDevice::start()` and `AudioDevice::stop()`
- Threading: GUI runs on main thread, audio processing in RtAudio callback thread

## Critical Development Notes

### Audio Callback Constraints
- **AudioDevice::audioCallback()** is real-time: NO allocations, file I/O, mutexes, or blocking calls
- Only call `AudioSystem::getNextSample()` and basic arithmetic 
- Configuration changes must happen outside the callback

### Memory Management
- Smart pointers preferred: `std::shared_ptr` for effects, `std::unique_ptr` for devices
- RAII pattern: Audio streams auto-close in destructors
- Avoid raw pointers except for Observer pattern interfaces

### Error Handling Philosophy
- Audio system continues with defaults if configuration fails
- Input validation throughout with graceful degradation
- Console apps exit on critical failures, GUI shows error states

### Embedded Port (`embedded/`)
- FreeRTOS reference implementation for STM32
- Shows DMA-driven audio output with sine LUT
- Custom waveform/effect callbacks via function pointers
- **Do not** modify for desktop development - it's a separate embedded example

## Common Workflows

### Debug Audio Issues
1. Check audio device initialization in console output
2. Verify RtAudio stream status and sample rate
3. Test with sequencer mode before MIDI input
4. Monitor `AudioDevice::audioCallback()` performance

### Performance Optimization
- Reduce buffer size for lower latency (increase CPU usage)
- Optimize effects chain order (cheaper effects first)
- Profile real-time callback with minimal instrumentation

### Testing Without MIDI Controller
- Set `<mode>sequencer</mode>` in `config.xml`  
- Use GUI demo sequences or console sequencer mode
- Multiple sequence types: `scale`, `chord`, `melody`, `demo`