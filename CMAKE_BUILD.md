# CMake Build Guide

## Overview
The AudioSystem project now uses CMake as the primary build system, providing cross-platform compatibility and easier dependency management.

## Quick Start

### Option 1: Using the Build Script (Recommended)
```bash
# Make the build script executable
chmod +x build.sh

# Build release version (default)
./build.sh

# Build debug version
./build.sh --debug

# Install dependencies and build
./build.sh --install-deps

# Build without GUI
./build.sh --no-gui

# Clean build
./build.sh --clean
```

### Option 2: Manual CMake Commands
```bash
# Configure the build
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build both applications
cmake --build build --parallel 4

# Or build specific targets
cmake --build build --target audioApp     # Console app only
cmake --build build --target audioGUI     # GUI app only
```

## Dependencies
The CMake build system will automatically find and link the following dependencies:
- **RtAudio** - Real-time audio I/O
- **RtMidi** - Real-time MIDI I/O  
- **LibXML2** - XML configuration parsing
- **ALSA** - Linux audio drivers
- **OpenGL** - Graphics rendering (GUI only)
- **GLFW** - Window management (GUI only)
- **X11, Xrandr, Xinerama, Xcursor** - Linux window system (GUI only)

### Installing Dependencies (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential cmake pkg-config \
    libasound2-dev librtmidi-dev libxml2-dev \
    libglfw3-dev libgl1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev
```

## Build Options
- `CMAKE_BUILD_TYPE`: Debug or Release (default: Release)
- `BUILD_GUI`: ON or OFF (default: ON)
- `BUILD_UTILITIES_TESTS`: ON or OFF (default: OFF)

Example:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_GUI=OFF
```

## Output Structure
```
build/
├── bin/
│   ├── audioApp          # Console application
│   ├── audioGUI          # GUI application  
│   ├── demo_app          # GUI library demo
│   └── examples/         # Additional examples
└── lib/
    ├── libguibase.a      # GUI library (static)
    └── libguibase.so     # GUI library (shared)
```

## VS Code Integration
New CMake-based tasks are available in VS Code:
- **CMake: Configure** - Configure the build system
- **CMake: Build All** - Build both applications
- **CMake: Build Console App** - Build only console app
- **CMake: Build GUI App** - Build only GUI app
- **CMake: Clean** - Clean build files
- **Run Console App** - Build and run console app
- **Run GUI App** - Build and run GUI app

## Installation
```bash
# Install system-wide (optional)
sudo cmake --install build

# This will install:
# - Binaries to /usr/local/bin/
# - Configuration to /usr/local/share/audioSystem/config/
```

## Troubleshooting

### GUI Build Issues
If GUI building fails, ensure the submodule is properly initialized:
```bash
git submodule update --init --recursive
```

### Dependency Issues
If dependencies are not found, use pkg-config to check:
```bash
pkg-config --exists rtaudio && echo "RtAudio found"
pkg-config --exists rtmidi && echo "RtMidi found"
```

### Clean Rebuild
For a completely clean rebuild:
```bash
rm -rf build
./build.sh --clean
```

## Legacy Build System
The old g++ build commands in `.vscode/tasks.json` are preserved as `tasks_legacy.json` for reference, but the new CMake system is recommended for all development.