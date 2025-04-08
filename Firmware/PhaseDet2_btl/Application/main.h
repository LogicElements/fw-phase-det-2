/**
 * @file      main.h
 * @brief     Simple buffer based bootloader main module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grMain
 * @{
 */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32l4xx_hal.h"
#include "common.h"

/* Defines -------------------------------------------------------------------*/

// Green LED
#define LEDR_0_Pin GPIO_PIN_8
#define LEDR_0_GPIO_Port GPIOB
// Red LED
#define LEDR_1_Pin GPIO_PIN_9
#define LEDR_1_GPIO_Port GPIOB


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
extern void Error_Handler(void);

/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/** @} */
