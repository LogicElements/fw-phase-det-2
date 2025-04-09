/**
 * @file       amc131m02.h
 * @version    $(APP_VERSION)
 * @date       $(RELEASE_DATE)
 * @brief      file_brief
 * @author     jan.bartovsky
 *
 * @copyright  Logic Elements Copyright
 *
 * @defgroup gr group_name
 * @{
 * @brief group_brief
 *
 * This module contains
 *
 * @par Main features:
 *
 * @par Example
 * @code
 *
 * @endcode
 */
#ifndef COMMON_AMC131M02_H_
#define COMMON_AMC131M02_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/

#define AMC_REG_ID        0x00

/* Typedefs-------------------------------------------------------------------*/


/* Functions -----------------------------------------------------------------*/

Status_t Amc_Init(void);

uint16_t Amc_ReadRegister(uint16_t address);

#endif /* COMMON_AMC131M02_H_ */
/** @} */
