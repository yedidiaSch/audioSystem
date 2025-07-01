#include "audio_task.h"
#include <math.h>
#include <string.h>

// Audio buffer placed in RAM (two halves for double buffering)
int16_t audioBuffer[AUDIO_BUFFER_SIZE];
static volatile uint8_t bufferIndex = 0;

static float sinLut[AUDIO_LUT_SIZE];

WaveGenerator CurrentWaveGenerator = NULL;
EffectProcessor CurrentEffectProcessor = NULL;

static void InitSineLUT(void)
{
    for (uint32_t i = 0; i < AUDIO_LUT_SIZE; ++i)
    {
        sinLut[i] = sinf(2.0f * M_PI * (float)i / (float)AUDIO_LUT_SIZE);
    }
}

static void DefaultWaveGenerator(float *dst, uint32_t count, float frequency, float sampleRate)
{
    static float phase = 0.0f;
    float phaseInc = frequency * (float)AUDIO_LUT_SIZE / sampleRate;
    for (uint32_t i = 0; i < count; ++i)
    {
        uint32_t idx = (uint32_t)phase % AUDIO_LUT_SIZE;
        dst[i] = sinLut[idx];
        phase += phaseInc;
        if (phase >= AUDIO_LUT_SIZE)
            phase -= AUDIO_LUT_SIZE;
    }
}

static void DefaultEffectProcessor(float *samples, uint32_t count, float sampleRate)
{
    (void)samples;
    (void)count;
    (void)sampleRate;
}

void AudioTaskInit(WaveGenerator waveGen, EffectProcessor effect)
{
    InitSineLUT();
    CurrentWaveGenerator = waveGen ? waveGen : DefaultWaveGenerator;
    CurrentEffectProcessor = effect ? effect : DefaultEffectProcessor;
}

static void FillBuffer(uint32_t offset)
{
    const float frequency = 440.0f;
    const float sampleRate = 48000.0f;
    float tmp[AUDIO_BUFFER_HALF_SIZE];
    CurrentWaveGenerator(tmp, AUDIO_BUFFER_HALF_SIZE, frequency, sampleRate);
    CurrentEffectProcessor(tmp, AUDIO_BUFFER_HALF_SIZE, sampleRate);
    for (uint32_t i = 0; i < AUDIO_BUFFER_HALF_SIZE; ++i)
    {
        int16_t sample = (int16_t)(tmp[i] * 32767.0f);
        audioBuffer[offset + i] = sample;
    }
}

void DMA_HalfComplete_Callback(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
    bufferIndex = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xTaskGetCurrentTaskHandle(), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void DMA_Complete_Callback(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
    bufferIndex = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xTaskGetCurrentTaskHandle(), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void AudioTask(void *parameters)
{
    (void)parameters;
    // Fill entire buffer initially
    FillBuffer(0);
    FillBuffer(AUDIO_BUFFER_HALF_SIZE);

    // Start DMA transfer in circular mode to I2S/DAC
    HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)audioBuffer, AUDIO_BUFFER_SIZE);

    for (;;)
    {
        // Wait for notification from DMA interrupt
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (bufferIndex == 0)
        {
            FillBuffer(0);
        }
        else
        {
            FillBuffer(AUDIO_BUFFER_HALF_SIZE);
        }
    }
}
