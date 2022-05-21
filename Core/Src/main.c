//==================================================================================================
// INCLUDES
//==================================================================================================

#include "iferr.h"
#include "os.h"
#include "pins_map.h"
#include "user_tasks.h"

#include "stm32f3xx_hal.h"
#include <assert.h>

//==================================================================================================
// DEFINES - MACROS
//==================================================================================================

// TODO LORIS: maybe move Error_Handler as part of iferr.h,
//   and create macro IFERR_HALT;
// or use assert_failed from hal_conf.h, and get rid of Error_Handler completely: how would it behave
//   in case of errors?
static void Error_Handler(void);

#define CHECK(x)                                                                                                       \
    {                                                                                                                  \
        HAL_StatusTypeDef err = (x);                                                                                   \
        if (err != HAL_OK)                                                                                             \
        {                                                                                                              \
            Error_Handler();                                                                                           \
        }                                                                                                              \
    }

//==================================================================================================
// ENUMS - STRUCTS - TYPEDEFS
//==================================================================================================

//==================================================================================================
// STATIC PROTOTYPES
//==================================================================================================

static HAL_StatusTypeDef SystemClock_Config(void);
static void MX_GPIO_Init(void);

//==================================================================================================
// STATIC VARIABLES
//==================================================================================================

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    CHECK(HAL_Init());

    /* Configure the system clock */
    CHECK(SystemClock_Config());

    /* Initialize all configured peripherals */
    MX_GPIO_Init();

    OS_AddThreads(UserTask_0, UserTask_1, UserTask_2);
    CHECK(OS_Init(10));
    CHECK(OS_Launch());

    /* This statement should not be reached */
    assert(0);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */

//==================================================================================================
// STATIC FUNCTIONS
//==================================================================================================

static void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

static HAL_StatusTypeDef SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
    IFERR_RETE(HAL_RCC_OscConfig(&RCC_OscInitStruct))

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    IFERR_RETE(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1));

    return HAL_OK;
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE,
                      CS_I2C_SPI_Pin | LD4_Pin | LD3_Pin | LD5_Pin | LD7_Pin | LD9_Pin | LD10_Pin | LD8_Pin | LD6_Pin,
                      GPIO_PIN_RESET);

    /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT1_Pin
                             MEMS_INT2_Pin */
    GPIO_InitStruct.Pin = DRDY_Pin | MEMS_INT3_Pin | MEMS_INT4_Pin | MEMS_INT1_Pin | MEMS_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                             LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                             LD6_Pin */
    GPIO_InitStruct.Pin =
        CS_I2C_SPI_Pin | LD4_Pin | LD3_Pin | LD5_Pin | LD7_Pin | LD9_Pin | LD10_Pin | LD8_Pin | LD6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : SPI1_SCK_Pin SPI1_MISO_Pin SPI1_MISOA7_Pin */
    GPIO_InitStruct.Pin = SPI1_SCK_Pin | SPI1_MISO_Pin | SPI1_MISOA7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : DM_Pin DP_Pin */
    GPIO_InitStruct.Pin = DM_Pin | DP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_USB;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : I2C1_SCL_Pin I2C1_SDA_Pin */
    GPIO_InitStruct.Pin = I2C1_SCL_Pin | I2C1_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
