# FreeRTOS STM32 Example

This folder contains example source files demonstrating how to run the audio
synthesis code on a STM32 microcontroller using FreeRTOS. The audio task fills a
double-buffer that is streamed to the DAC/I2S peripheral via DMA.

## Files
- `main.c` initializes peripherals, creates the FreeRTOS audio task and starts
  the scheduler.
- `audio_task.c` implements the audio generation task and DMA callbacks.
- `audio_task.h` header for the task and buffer definitions.

The audio task builds a sine lookup table at startup and exposes function
pointers so you can provide your own waveform generator and effect processor.
Call `AudioTaskInit` with your callbacks before starting the task.

The code assumes use of STM32 HAL drivers and should be compiled with your
preferred STM32 toolchain. Link FreeRTOS sources and ensure that the HAL I2S and
DMA callbacks call `DMA_HalfComplete_Callback` and `DMA_Complete_Callback` when
half/full transfer events occur.

## Build Notes
1. Add the FreeRTOS and STM32 HAL source files to your project.
2. Configure the linker script for your device to provide memory for the RTOS
   and audio buffers (e.g., place `audioBuffer` in DMA-accessible memory).
3. Enable the DMA interrupt handlers to call the provided callback functions.
4. Use a minimal `heap_4.c` or static allocation to avoid `malloc`/`free`.
