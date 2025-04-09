/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */
#include "configuration.h"
#include "com_proto.h"
#include "system_msp.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */

/**
 * Length of the receive buffer
 */
#define USB_BUF_DATA_SIZE      64

/**
 * Number of receive buffers
 */
#define USB_BUF_COUNT   64

/**
 * Length of the transmit buffer
 */
#define MESSAGE_BUF_SIZE  4096

/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */
typedef struct
{
  /* USB receive buffers */
  uint8_t usbBuf[USB_BUF_COUNT][USB_BUF_DATA_SIZE];
  uint8_t usbLen[USB_BUF_COUNT];
  uint8_t bufFirst;
  uint8_t bufLast;

  /* Message buffers */
  uint8_t txBuffer[MESSAGE_BUF_SIZE];
  uint8_t rxBuffer[MESSAGE_BUF_SIZE];
  uint32_t recvLength;
  uint32_t procLength;


}Cdc_Private_t;
/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
static USBD_CDC_LineCodingTypeDef LineCoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
  };
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */
static Cdc_Private_t cdc;
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);
static int8_t CDC_TransmitCplt_FS(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS,
  CDC_TransmitCplt_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, cdc.txBuffer, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, cdc.usbBuf[0]);

  memset(cdc.usbLen, 0, USB_BUF_COUNT);
  cdc.bufFirst = 0;
  cdc.bufLast = 0;

  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:

    break;

    case CDC_GET_LINE_CODING:
      pbuf[0] = (uint8_t)(LineCoding.bitrate);
      pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8);
      pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16);
      pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24);
      pbuf[4] = LineCoding.format;
      pbuf[5] = LineCoding.paritytype;
      pbuf[6] = LineCoding.datatype;

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
  /* Save received length */
  cdc.usbLen[cdc.bufFirst] = *Len;

  /* Increment First pointer */
  if (cdc.bufFirst == USB_BUF_COUNT - 1)
  {
    cdc.bufFirst = 0;
  }
  else
  {
    cdc.bufFirst++;
  }
  /* If First jumps over Last, there is an overflow */
  if (cdc.bufFirst == cdc.bufLast)
  {
    /* FIXME: Overflow error */
    Error_Handler();
  }

//  if (TICK_EXPIRED(cdc.recvTick))
//  {
//    cdc.recvTick = HAL_GetTick() + COMM_DELAY_NEW_PACKET;
//  }

  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, cdc.usbBuf[cdc.bufFirst]);
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  uint32_t countdown = 50;
  USBD_CDC_HandleTypeDef *hcdc =
      (USBD_CDC_HandleTypeDef*) hUsbDeviceFS.pClassData;
  do
  {
    countdown--;

    if (hcdc->TxState != 0)
    {
      System_Delay(1);
    }
  } while (hcdc->TxState != 0 && countdown);

  if (hcdc->TxState == 0)
  {
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  }
  /* USER CODE END 7 */
  return result;
}

/**
  * @brief  CDC_TransmitCplt_FS
  *         Data transmitted callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_TransmitCplt_FS(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 13 */
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);
  /* USER CODE END 13 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

Status_t CDC_TransmitReady(void)
{
  Status_t ret = STATUS_OK;
  USBD_CDC_HandleTypeDef *hcdc =
        (USBD_CDC_HandleTypeDef*) hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0)
  {
    ret = STATUS_ERROR;
  }
  return ret;
}

Status_t CDC_PacketReceived(void)
{
  Status_t ret = STATUS_OK;
  uint32_t countdown;
  uint32_t length;

  /* Copy data from USB buffers into message buffer */
  while (cdc.bufFirst != cdc.bufLast)
  {
    if (cdc.recvLength <= MESSAGE_BUF_SIZE - cdc.usbLen[cdc.bufLast])
    {
      /* Copy data from Last buffer */
      memcpy(cdc.rxBuffer + cdc.recvLength, cdc.usbBuf[cdc.bufLast], cdc.usbLen[cdc.bufLast]);
      /* Increment pointer and clear received length */
      cdc.recvLength += cdc.usbLen[cdc.bufLast];
      cdc.usbLen[cdc.bufLast] = 0;

      /* Increment last pointer */
      if (cdc.bufLast == USB_BUF_COUNT - 1)
      {
        cdc.bufLast = 0;
      }
      else
      {
        cdc.bufLast++;
      }
    }
    else
    {
      CDC_ClearRxBuffer();
      cdc.bufLast = cdc.bufFirst;
    }
  }

  ret = ComProto_CheckProtoLength(cdc.rxBuffer + cdc.procLength, cdc.recvLength - cdc.procLength);

  /* Full packet received */
  if (ret == STATUS_OK)
  {
    /* Read data */
    while (ret == STATUS_OK)
    {
      ret = ComProto_CheckProtoLength(cdc.rxBuffer + cdc.procLength, cdc.recvLength - cdc.procLength);

      if (ret == STATUS_OK)
      {
        /* Before we start processing data, check that Tx buffer is free to use */
        USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;

        countdown = 50;
        while (hcdc->TxState != 0 && countdown)
        {
          countdown--;
          System_Delay(1);
        }

        length = ComProto_GetLength(cdc.rxBuffer + cdc.procLength);

        ret = ComProto_ProcessPacket(cdc.rxBuffer + cdc.procLength, cdc.txBuffer);

        cdc.procLength += length;

        if (ret == STATUS_OK)
        {
          CDC_Transmit_FS(cdc.txBuffer, ComProto_GetLength(cdc.txBuffer));
        }
      }
    }

    /* Reset status to OK */
    ret = STATUS_OK;

    /* All data processed? */
    if (cdc.procLength == cdc.recvLength)
    {
      ret |= CDC_ClearRxBuffer();
    }
    else
    {
      /* Some data remains */
      length = cdc.recvLength - cdc.procLength;
      memcpy(cdc.rxBuffer, cdc.rxBuffer + cdc.procLength, length);
      cdc.recvLength = length;
      cdc.procLength = 0;
    }
  }

  else
  {
    /* Packet not received yet -> do nothing */
    ret = STATUS_OK;
  }

  return ret;
}


Status_t CDC_ClearRxBuffer(void)
{
  Status_t ret = STATUS_OK;

  cdc.recvLength = 0;
  cdc.procLength = 0;

  return ret;
}

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
