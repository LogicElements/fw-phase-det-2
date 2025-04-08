/**
 * @file      crc.h
 * @brief     CRC module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grCrc
 * @{
 */

#ifndef __CRC_H__
#define __CRC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Public variables ----------------------------------------------------------*/

//! CRC unit handle
extern CRC_HandleTypeDef hcrc;

/* Functions -----------------------------------------------------------------*/

/**
 * @brief CRC unit initialization
 */
extern void MX_CRC_Init(void);

/**
 * @brief Clear CRC computation
 */
extern void System_CrcClear(void);

/**
 * @brief Calculate CRC computation
 * @param data Pointer to data to calculate
 * @param length Length of the data
 * @return
 */
extern uint32_t System_CrcAccumulate(uint32_t *data, uint32_t length);

/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __CRC_H__ */

/** @} */
