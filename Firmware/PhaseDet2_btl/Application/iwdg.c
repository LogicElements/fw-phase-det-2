/**
 * @file      iwdg.c
 * @brief     WatchDog module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grIwdg
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "iwdg.h"

/* Public variables ----------------------------------------------------------*/

//! IWDG handle
IWDG_HandleTypeDef hiwdg;

/* Functions -----------------------------------------------------------------*/

/* IWDG initialization */
void MX_IWDG_Init(void)
{
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
}

/* ---------------------------------------------------------------------------*/

/** @} */
