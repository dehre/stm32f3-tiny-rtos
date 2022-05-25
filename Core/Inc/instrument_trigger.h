/**
 * The module instrument_trigger provides a macro that generates functions to set up and
 * use a GPIO pin as push-pull output.
 * Those pins are useful for toggling LED and/or triggering a debugging instrument,
 * i.e. an oscilloscope or a logic analyzer.
 *
 * Example:
 * ```c
 * #include "instrument-trigger.h"
 * InstrumentTrigger_Create(E, 13);
 *
 * InstrumentTriggerPE13_Init();
 * while (1)
 * {
 *     InstrumentTriggerPE13_Toggle();
 *     HAL_Delay(1000);
 * }
 * ```
 *
 * Available user LEDs on the STM32F3 Discovery Board:
 * - LD3:  Red    LED connected to the I/O PE9  of the STM32F303VCT6
 * - LD4:  Blue   LED connected to the I/O PE8  of the STM32F303VCT6
 * - LD5:  Orange LED connected to the I/O PE10 of the STM32F303VCT6
 * - LD6:  Green  LED connected to the I/O PE15 of the STM32F303VCT6
 * - LD7:  Green  LED connected to the I/O PE11 of the STM32F303VCT6
 * - LD8:  Orange LED connected to the I/O PE14 of the STM32F303VCT6
 * - LD9:  Blue   LED connected to the I/O PE12 of the STM32F303VCT6
 * - LD10: Red    LED connected to the I/O PE13 of the STM32F303VCT6
 */

#pragma once

#include "stm32f3xx_hal.h"

#define InstrumentTrigger_Create(PORT, PIN)                                                                            \
    void InstrumentTriggerP##PORT##PIN##_Init(void)                                                                    \
    {                                                                                                                  \
        __HAL_RCC_GPIO##PORT##_CLK_ENABLE();                                                                           \
        GPIO_InitTypeDef GPIO_InitStruct = {0};                                                                        \
        GPIO_InitStruct.Pin = GPIO_PIN_##PIN;                                                                          \
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;                                                                    \
        GPIO_InitStruct.Pull = GPIO_NOPULL;                                                                            \
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;                                                                   \
        HAL_GPIO_Init(GPIO##PORT, &GPIO_InitStruct);                                                                   \
    }                                                                                                                  \
    void InstrumentTriggerP##PORT##PIN##_Set(void)                                                                     \
    {                                                                                                                  \
        HAL_GPIO_WritePin(GPIO##PORT, GPIO_PIN_##PIN, GPIO_PIN_SET);                                                   \
    }                                                                                                                  \
    void InstrumentTriggerP##PORT##PIN##_Reset(void)                                                                   \
    {                                                                                                                  \
        HAL_GPIO_WritePin(GPIO##PORT, GPIO_PIN_##PIN, GPIO_PIN_RESET);                                                 \
    }                                                                                                                  \
    void InstrumentTriggerP##PORT##PIN##_Toggle(void)                                                                  \
    {                                                                                                                  \
        HAL_GPIO_TogglePin(GPIO##PORT, GPIO_PIN_##PIN);                                                                \
    }                                                                                                                  \
    GPIO_PinState InstrumentTriggerP##PORT##PIN##_Read(void)                                                           \
    {                                                                                                                  \
        return HAL_GPIO_ReadPin(GPIO##PORT, GPIO_PIN_##PIN);                                                           \
    }                                                                                                                  \
    _Static_assert(1, "semi-colon required after this macro, see https://stackoverflow.com/a/59153563/7168774")
