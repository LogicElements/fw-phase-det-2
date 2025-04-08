/**
 * @file      tim.h
 * @brief     Timer module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grTim
 * @{
 */

#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Public variables ----------------------------------------------------------*/

//! TIM15 handle
extern TIM_HandleTypeDef htim15;

/* Functions -----------------------------------------------------------------*/

/**
 * @brief TIM15 initialization
 */
extern void MX_TIM15_Init(void);

/**
 * @brief TIM15 start
 */
extern Status_t MX_TIM15_Start(void);

/**
 * @brief TIM15 stop
 */
extern Status_t MX_TIM15_Stop(void);

/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

/** @} */
