#pragma once

/**
 * @brief  The application's entry point.
 */
int main(void);

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void);

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line);
#endif /* USE_FULL_ASSERT */
