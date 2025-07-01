#include "audio_task.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

UART_HandleTypeDef huart2;
I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_i2s3_tx;

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S3_Init(void);
static void MX_USART2_UART_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2S3_Init();
    MX_USART2_UART_Init();

    // Initialize waveform and effect callbacks (NULL for defaults)
    AudioTaskInit(NULL, NULL);

    xTaskCreate(AudioTask, "audio", 256, NULL, tskIDLE_PRIORITY + 2, NULL);
    vTaskStartScheduler();

    while (1)
    {
    }
}

static void SystemClock_Config(void)
{
    // Configure system clock (implementation depends on board)
}

static void MX_GPIO_Init(void)
{
    // Initialize GPIO pins
}

static void MX_DMA_Init(void)
{
    // Initialize DMA controller and interrupts
}

static void MX_I2S3_Init(void)
{
    hi2s3.Instance = SPI3;
    hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
    hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_48K;
    hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
    HAL_I2S_Init(&hi2s3);
}

static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}

// Override _write to redirect printf to UART
int _write(int fd, char *ptr, int len)
{
    (void)fd;
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}
