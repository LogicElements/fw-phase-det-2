/**
 * @file      crc.c
 * @brief     CRC module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grCrc
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "crc.h"

/* Public variables ----------------------------------------------------------*/

//! CRC unit handle
CRC_HandleTypeDef hcrc;

/* Functions -----------------------------------------------------------------*/

/* CRC unit initialization */
void MX_CRC_Init(void)
{
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_WORDS;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
}


/* CRC unit clock initialization */
void HAL_CRC_MspInit(CRC_HandleTypeDef* crcHandle)
{
  if(crcHandle->Instance==CRC)
  {
    __HAL_RCC_CRC_CLK_ENABLE();
  }
}


/* CRC unit clock deinitialization */
void HAL_CRC_MspDeInit(CRC_HandleTypeDef* crcHandle)
{
  if(crcHandle->Instance==CRC)
  {
    __HAL_RCC_CRC_CLK_DISABLE();
  }
}


/* Clear CRC computation */
void System_CrcClear(void)
{
  __HAL_CRC_DR_RESET(&hcrc);
}


/* Calculate CRC computation */
uint32_t System_CrcAccumulate(uint32_t *data, uint32_t length)
{
  return HAL_CRC_Accumulate(&hcrc, data, length);
}

/* ---------------------------------------------------------------------------*/

/** @} */
