//==================================================================================================
// INCLUDES
//==================================================================================================

#include "onboard_user_button.h"

#include "instrument_trigger.h"
#include "os.h"

#include "stm32f3xx_hal.h"

//==================================================================================================
// DEFINES - MACROS
//==================================================================================================

InstrumentTrigger_Create(E, 8);

//==================================================================================================
// ENUMS - STRUCTS - TYPEDEFS
//==================================================================================================

//==================================================================================================
// STATIC PROTOTYPES
//==================================================================================================

static void OnTouch(void);

//==================================================================================================
// STATIC VARIABLES
//==================================================================================================

static Semaphore_t SemaphoreButtonPressed = 0;

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

void OnboardUserButton_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0x0F, 0); /* Minimum pre-emption priority */
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    InstrumentTriggerPE8_Init();
}

void OnboardUserButton_IRQHandler(void)
{
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    OS_Semaphore_Signal(&SemaphoreButtonPressed);
}

void OnboardUserButton_Task(void)
{
    while (1)
    {
        OS_Semaphore_Wait(&SemaphoreButtonPressed);
        OnTouch();
        HAL_Delay(10);
        HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    }
}

//==================================================================================================
// STATIC FUNCTIONS
//==================================================================================================

static void OnTouch(void)
{
    InstrumentTriggerPE8_Toggle();
}
