/**
 * @file      iwdg.h
 * @brief     WatchDog module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grIwdg
 * @{
 */

#ifndef __IWDG_H__
#define __IWDG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Public variables ----------------------------------------------------------*/

//! IWDG handle
extern IWDG_HandleTypeDef hiwdg;

/* Functions -----------------------------------------------------------------*/

/**
 * @brief IWDG initialization
 */
extern void MX_IWDG_Init(void);

/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __IWDG_H__ */

/** @} */
