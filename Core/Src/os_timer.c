//==================================================================================================
// INCLUDES
//==================================================================================================

#include "os_timer.h"

#include "iferr.h"

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

static TIM_HandleTypeDef TIMHandle;

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

void OSTimer_Init(uint32_t reload_frequency_hz)
{
    /* Compute the prescaler value to have TIM2 counter clock equal to 1 KHz */
    uint32_t prescaler_divisions = 1000;
    TIMHandle.Instance = OSTimer_Instance;
    TIMHandle.Init.Prescaler = (SystemCoreClock / prescaler_divisions) - 1;
    TIMHandle.Init.Period = (prescaler_divisions / reload_frequency_hz) - 1;
    TIMHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIMHandle.Init.CounterMode = TIM_COUNTERMODE_DOWN;
    assert_or_panic(TIMHandle.Init.Period > 0);
    IFERR_PANIC(HAL_TIM_Base_Init(&TIMHandle));
}

void OSTimer_Start(void)
{
    IFERR_PANIC(HAL_TIM_Base_Start_IT(&TIMHandle));
}

void OSTimer_ClearITFlag(void)
{
    __HAL_TIM_CLEAR_IT(&TIMHandle, TIM_IT_UPDATE);
}

//==================================================================================================
// STATIC FUNCTIONS
//==================================================================================================
