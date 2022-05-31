//==================================================================================================
// INCLUDES
//==================================================================================================

#include "schedl_timer.h"

#include "iferr.h"
#include "instrument_trigger.h"

//==================================================================================================
// DEFINES - MACROS
//==================================================================================================

/* The pin PB0 is used for monitoring the interval between context-switches with the logic analyzer */
InstrumentTrigger_Create(B, 0);

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

void SchedlTimer_Init(uint32_t reload_frequency_hz)
{
    /* Compute the prescaler value to have TIM2 counter clock equal to 1 KHz */
    uint32_t prescaler_divisions = 10000;
    uint32_t period = prescaler_divisions / reload_frequency_hz;
    assert_or_panic(period > 1);

    TIMHandle.Instance = SchedlTimer_Instance;
    TIMHandle.Init.Prescaler = (SystemCoreClock / prescaler_divisions) - 1; /* Off by 1 because it’s 0-based */
    TIMHandle.Init.Period = period - 1;                                     /* Off by 1 because it’s 0-based */
    TIMHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIMHandle.Init.CounterMode = TIM_COUNTERMODE_DOWN;
    IFERR_PANIC(HAL_TIM_Base_Init(&TIMHandle));
    InstrumentTriggerPB0_Init();
}

void SchedlTimer_Start(void)
{
    IFERR_PANIC(HAL_TIM_Base_Start_IT(&TIMHandle));
}

void SchedlTimer_ClearITFlag(void)
{
    __HAL_TIM_CLEAR_IT(&TIMHandle, TIM_IT_UPDATE);
    InstrumentTriggerPB0_Toggle();
}

void SchedlTimer_ResetCounter(void)
{
    __HAL_TIM_SET_COUNTER(&TIMHandle, 0);
    /* Don't return from this fn before the interrupt triggered */
    HAL_Delay(1);
}

//==================================================================================================
// STATIC FUNCTIONS
//==================================================================================================
