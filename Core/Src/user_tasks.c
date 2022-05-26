#include "user_tasks.h"

#include "instrument_trigger.h"
#include "os.h"

#include "stm32f3xx_hal.h"
#include <stdint.h>

InstrumentTrigger_Create(E, 11);
InstrumentTrigger_Create(E, 12);
InstrumentTrigger_Create(E, 13);

void UserTask_0(void)
{
    InstrumentTriggerPE11_Init();
    while (1)
    {
        InstrumentTriggerPE11_Toggle();
        HAL_Delay(200);
    }
}

void UserTask_1(void)
{
    InstrumentTriggerPE12_Init();
    while (1)
    {
        for (uint32_t i = 0; i <= 8; i++)
        {
            InstrumentTriggerPE12_Toggle();
            HAL_Delay(80);
        }
        OS_Suspend();
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
