/**
 * @file      gpio.c
 * @brief     GPIO module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grGpio
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "gpio.h"

/* Functions -----------------------------------------------------------------*/

/* GPIO initialization */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  HAL_GPIO_WritePin(LEDR_0_GPIO_Port, LEDR_0_Pin | LEDR_1_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = LEDR_0_Pin | LEDR_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LEDR_0_GPIO_Port, &GPIO_InitStruct);
}

/* ---------------------------------------------------------------------------*/

/** @} */
