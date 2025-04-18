/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC_Pin GPIO_PIN_0
#define ADC_GPIO_Port GPIOA
#define RS485_DERE_Pin GPIO_PIN_1
#define RS485_DERE_GPIO_Port GPIOA
#define RS485_D_Pin GPIO_PIN_2
#define RS485_D_GPIO_Port GPIOA
#define RS485_R_Pin GPIO_PIN_3
#define RS485_R_GPIO_Port GPIOA
#define SYNCnRESET_Pin GPIO_PIN_1
#define SYNCnRESET_GPIO_Port GPIOB
#define ADC_DRDY_Pin GPIO_PIN_2
#define ADC_DRDY_GPIO_Port GPIOB
#define ADC_DRDY_EXTI_IRQn EXTI2_IRQn
#define ADDR3_Pin GPIO_PIN_12
#define ADDR3_GPIO_Port GPIOB
#define ADDR2_Pin GPIO_PIN_13
#define ADDR2_GPIO_Port GPIOB
#define ADDR1_Pin GPIO_PIN_14
#define ADDR1_GPIO_Port GPIOB
#define ADDR0_Pin GPIO_PIN_15
#define ADDR0_GPIO_Port GPIOB
#define CLKIN_Pin GPIO_PIN_3
#define CLKIN_GPIO_Port GPIOB
#define CS_A_Pin GPIO_PIN_4
#define CS_A_GPIO_Port GPIOB
#define CS_B_Pin GPIO_PIN_5
#define CS_B_GPIO_Port GPIOB
#define LED_0_Pin GPIO_PIN_9
#define LED_0_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
