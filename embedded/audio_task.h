#ifndef AUDIO_TASK_H
#define AUDIO_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
extern I2S_HandleTypeDef hi2s3;

#ifdef __cplusplus
extern "C" {
#endif

// Size of each half of the audio buffer (samples per channel)
#define AUDIO_BUFFER_HALF_SIZE 256

// Stereo frame count in entire buffer (2 halves)
#define AUDIO_BUFFER_SIZE (2 * AUDIO_BUFFER_HALF_SIZE)

extern int16_t audioBuffer[AUDIO_BUFFER_SIZE];

// Number of entries in the sine lookup table
#define AUDIO_LUT_SIZE 512

// Function pointer type for generating raw waveform samples
typedef void (*WaveGenerator)(float *dst, uint32_t count, float frequency, float sampleRate);

// Function pointer type for processing effects on the generated samples
typedef void (*EffectProcessor)(float *samples, uint32_t count, float sampleRate);

extern WaveGenerator CurrentWaveGenerator;
extern EffectProcessor CurrentEffectProcessor;

void AudioTaskInit(WaveGenerator waveGen, EffectProcessor effect);

void AudioTask(void *parameters);
void DMA_HalfComplete_Callback(DMA_HandleTypeDef *hdma);
void DMA_Complete_Callback(DMA_HandleTypeDef *hdma);

#ifdef __cplusplus
}
#endif

#endif // AUDIO_TASK_H
