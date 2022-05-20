//==================================================================================================
// INCLUDES
//==================================================================================================

#include "os_timer.h"

#include "stm32f3xx_hal.h"

//==================================================================================================
// DEFINES - MACROS
//==================================================================================================

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

TIM_HandleTypeDef OSTimer_Handle;

// TODO LORIS: adjust values
HAL_StatusTypeDef OSTimer_Init(void)
{
    /* Compute the prescaler value to have TIM2 counter clock equal to 10 KHz */
    uint32_t uwPrescalerValue = (SystemCoreClock / 10000) - 1;
    OSTimer_Handle.Instance = OSTimer_Instance;
    OSTimer_Handle.Init.Period = 10000 - 1;
    OSTimer_Handle.Init.Prescaler = uwPrescalerValue;
    OSTimer_Handle.Init.ClockDivision = 0;
    OSTimer_Handle.Init.CounterMode = TIM_COUNTERMODE_DOWN;
    OSTimer_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    return HAL_TIM_Base_Init(&OSTimer_Handle);
}

HAL_StatusTypeDef OSTimer_Start(void)
{
    return HAL_TIM_Base_Start_IT(&OSTimer_Handle);
}

void OSTimer_ClearITFlag(void)
{
    __HAL_TIM_CLEAR_IT(&OSTimer_Handle, TIM_IT_UPDATE);
}

//==================================================================================================
// STATIC FUNCTIONS
//==================================================================================================
