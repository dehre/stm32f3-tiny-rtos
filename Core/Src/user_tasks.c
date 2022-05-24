#include "user_tasks.h"

#include "instrument_trigger.h"

#include "stm32f3xx_hal.h"
#include <stdint.h>

/*
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

InstrumentTrigger_Create(E, 11);
InstrumentTrigger_Create(E, 12);
InstrumentTrigger_Create(E, 13);

void UserTask_0(void)
{
    InstrumentTriggerPE11_Init();
    while (1)
    {
        InstrumentTriggerPE11_Toggle();
        HAL_Delay(800);
    }
}

void UserTask_1(void)
{
    InstrumentTriggerPE12_Init();
    while (1)
    {
        InstrumentTriggerPE12_Toggle();
        HAL_Delay(400);
    }
}

void UserTask_2(void)
{
    InstrumentTriggerPE13_Init();
    while (1)
    {
        InstrumentTriggerPE13_Toggle();
        HAL_Delay(150);
    }
}
