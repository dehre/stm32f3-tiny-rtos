//==================================================================================================
// INCLUDES
//==================================================================================================

#include "stm32f3xx_hal_msp.h"

#include "schedl_timer.h"

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    SchedlTimer_ClkEnable();
    HAL_NVIC_SetPriority(SchedlTimer_IRQn, SchedlTimer_IRQPreemptPriority, SchedlTimer_IRQSubPriority);
    HAL_NVIC_EnableIRQ(SchedlTimer_IRQn);
}
