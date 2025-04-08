/**
 * @file      gpio.h
 * @brief     GPIO module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grGpio
 * @{
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Functions -----------------------------------------------------------------*/

/**
 * @brief GPIO initialization
 */
void MX_GPIO_Init(void);

/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /*__ GPIO_H__ */

/** @} */
