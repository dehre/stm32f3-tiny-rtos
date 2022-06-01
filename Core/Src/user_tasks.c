//==================================================================================================
// INCLUDES
//==================================================================================================

#include "user_tasks.h"

#include "instrument_trigger.h"
#include "os.h"

#include "stm32f3xx_hal.h"
#include <stdint.h>

//==================================================================================================
// DEFINES - MACROS
//==================================================================================================

/* See instrument_trigger.h for the available LEDs on the STM32F3DISCOVERY board */
InstrumentTrigger_Create(E, 11); /* UserTask_0 */
InstrumentTrigger_Create(E, 12); /* UserTask_1 */
InstrumentTrigger_Create(E, 13); /* UserTask_2 */
InstrumentTrigger_Create(E, 14); /* UserTask_3 */

//==================================================================================================
// ENUMS - STRUCTS - TYPEDEFS
//==================================================================================================

//==================================================================================================
// STATIC PROTOTYPES
//==================================================================================================

//==================================================================================================
// STATIC VARIABLES
//==================================================================================================

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

void UserTask_0(void)
{
    InstrumentTriggerPE11_Init();
    uint32_t count = 0;
    while (1)
    {
        InstrumentTriggerPE11_Toggle();
        HAL_Delay(60);
        count++;
        if (count == 100)
        {
            OS_Thread_Create(UserTask_3, OS_SCHEDL_PRIO_MAIN_THREAD);
        }
        if (count == 200)
        {
            InstrumentTriggerPE11_Reset();
            OS_Thread_Kill();
        }
    }
}

void UserTask_1(void)
{
    InstrumentTriggerPE12_Init();
    while (1)
    {
        for (uint32_t i = 0; i <= 12; i++)
        {
            InstrumentTriggerPE12_Toggle();
            HAL_Delay(50);
        }
        OS_Thread_Suspend();
    }
}

void UserTask_2(void)
{
    InstrumentTriggerPE13_Init();
    uint32_t count = 0;
    while (1)
    {
        InstrumentTriggerPE13_Toggle();
        count++;
        if (count % 35 == 0)
            OS_Thread_Sleep(4500);
        else
            HAL_Delay(70);
    }
}

void UserTask_3(void)
{
    InstrumentTriggerPE14_Init();
    while (1)
    {
        InstrumentTriggerPE14_Toggle();
        HAL_Delay(60);
    }
}

//==================================================================================================
// STATIC FUNCTIONS
//==================================================================================================
