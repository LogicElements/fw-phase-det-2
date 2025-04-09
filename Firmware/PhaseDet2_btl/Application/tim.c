/**
 * @file      tim.c
 * @brief     Timer module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grTim
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "tim.h"

/* Public variables ----------------------------------------------------------*/

//! TIM15 handle
TIM_HandleTypeDef htim15;

/* Functions -----------------------------------------------------------------*/

/* TIM15 initialization */
void MX_TIM15_Init(void)
{
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 40000 - 1;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 250 - 1;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
}


/* TIM clock and interrupt initialization */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance == TIM15)
  {
    __HAL_RCC_TIM15_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
  }
}


/* TIM clock and interrupt deinitialization */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance == TIM15)
  {
    __HAL_RCC_TIM15_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM1_BRK_TIM15_IRQn);
  }
}


/* TIM15 start */
Status_t MX_TIM15_Start(void)
{
  Status_t ret = STATUS_OK;

  if (HAL_TIM_Base_Start_IT(&htim15) != HAL_OK)
  {
    Error_Handler();
  }

  return ret;
}


/* TIM15 stop */
Status_t MX_TIM15_Stop(void)
{
  Status_t ret = STATUS_OK;

  if (HAL_TIM_Base_Stop_IT(&htim15) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_Base_DeInit(&htim15) != HAL_OK)
  {
    Error_Handler();
  }

  return ret;
}

/* ---------------------------------------------------------------------------*/

/** @} */
