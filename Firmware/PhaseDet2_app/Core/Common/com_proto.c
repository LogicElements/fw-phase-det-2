/**
 * @file       comm_proto.c
 * @brief      LeBin communication protocol communication
 * @addtogroup grComProto
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "com_proto.h"

#include "system_msp.h"
#include "configuration.h"
#include "usbd_cdc_if.h"
#include "control.h"

/* Private defines -----------------------------------------------------------*/\

//! Countdown of parsing loop (depends on number of registers in a single request
#define PARSE_COUNTDOWN_VALUE                 400

//! Length of event buffer
#define EVENT_BUFFER                         1024

//! Text message length
#define MSG_LENGTH                             32

//! Upload data timeout in ms
#define UPD_TIMEOUT                          2000

/* Private macros  -----------------------------------------------------------*/
/* Private typedefs ----------------------------------------------------------*/

//! Definition of all private variables
typedef struct
{
  uint32_t sentResp;                            ///< number of received and answered packets
  uint8_t log_buffer[COM_PROTO_TEXT_BUFFER];    ///< text log buffer
  uint32_t exp_size;                            ///< expected uploaded file size
  uint32_t upload_time;                         ///< timer for file upload timeout
  uint16_t ptr;                                 ///< number of characters in the log buffer
} ComProto_Private_t;

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//! Instance of all private variables (except HAL handles)
static ComProto_Private_t com;

/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
 * @brief Arrived register write request
 * @param data Pointer to the request message data
 * @param txresp Pointer to the response buffer
 */
static Status_t ComProto_WriteReg(uint8_t *data, uint8_t *txResp);

/**
 * @brief Arrived register read request
 * @param data Pointer to the request message data
 * @param txresp Pointer to the response buffer
 */
static Status_t ComProto_ReadReg(uint8_t *data, uint8_t *txResp);

/**
 * @brief Arrived firmware upgrade data chunk
 * @param data Pointer to the request message data
 * @param txresp Pointer to the response buffer
 */
static Status_t ComProto_FwUpgrade(uint8_t *data, uint8_t *txResp);

/**
 * @brief Arrived text log download request
 * @param data Pointer to the request message data
 * @param txresp Pointer to the response buffer
 */
static Status_t ComProto_GetLog(uint8_t *data, uint8_t *txResp);


/* Functions -----------------------------------------------------------------*/

/* Initialize module */
Status_t ComProto_Init(void)
{
  Status_t ret = STATUS_OK;

  com.upload_time = 0;  // upload inactive
  return(ret);
}


/* Permanent handle */
Status_t ComProto_Handle(void)
{
  Status_t ret = STATUS_OK;


  return(ret);
}


/* Get length of received packet */
uint16_t ComProto_GetLength(uint8_t *data)
{
  // extract data length from header
  return(data[2] + data[3] * 256);
}


/* Check the length and basic consistency of received packet */
Status_t ComProto_CheckProtoLength(uint8_t *data, uint16_t length)
{
  Status_t ret = STATUS_ERROR;
  uint16_t packetLen;

  // get packet length
  packetLen = ComProto_GetLength(data);

  // check length, start byte, minimal length
  if (length >= packetLen && length >= COM_PROTO_MIN_LENGTH)
  {
    ret = STATUS_OK;
  }

  if (data[0] != COM_PROTO_UART_START_BYTE && length >= COM_PROTO_MIN_LENGTH)
  {
    // possible dead-lock
    CDC_ClearRxBuffer();
  }

  return(ret);
}


/*  */
Status_t ComProto_FillSeries(uint8_t *buffer, uint16_t type, uint16_t id, uint32_t timestamp, uint32_t delta, uint32_t count)
{
  Status_t ret = STATUS_OK;
  uint32_t *buf = (uint32_t *)buffer;
  uint16_t length;

  // length of packet
  length = COM_PROTO_HEADER_LEN + 16 + count * CONF_BYTE_LEN_ID(type);

  // header
  buffer[0] = COM_PROTO_UART_START_BYTE;
  buffer[1] = COM_PROTO_TIME_SER;
  buffer[2] = length;
  buffer[3] = length >> 8;

  // series information
  buf[1] = type + (id << 16);
  buf[2] = timestamp;
  buf[3] = delta;
  buf[4] = count;

  return(ret);
}


Status_t ComProto_LogText(char *text)
{
  Status_t ret = STATUS_OK;

  while (*text != '\0')
  {
    ret |= ComProto_LogChar(*text);
    text++;
  }

  return ret;
}


Status_t ComProto_LogChar(char chr)
{
  Status_t ret = STATUS_OK;

  if (chr != '\0')
  {
    if (com.ptr >= COM_PROTO_TEXT_BUFFER)
    {
      com.ptr = COM_PROTO_TEXT_BUFFER / 2;
      memcpy(com.log_buffer, com.log_buffer + com.ptr, com.ptr);
    }
    *(com.log_buffer + com.ptr) = chr;
    com.ptr++;
  }

  return ret;
}


/* Process the received packet */
Status_t ComProto_ProcessPacket(uint8_t *data, uint8_t* txResp)
{
  Status_t ret = STATUS_OK;
  uint16_t respIdx = 0;

  UNUSED(respIdx);

  // pre-fill response packet header
  txResp[0] = data[0];
  txResp[1] = COM_PROTO_READ_REG_RESP;
  txResp[2] = data[2];
  txResp[3] = data[3];

  switch (data[1])
  {

    // write register
    case COM_PROTO_WRITE_REG:
      ret = ComProto_WriteReg(data, txResp);
      break;

    // read register
    case COM_PROTO_READ_REG:
      ret = ComProto_ReadReg(data, txResp);
      break;

    // update firmware
    case COM_PROTO_FW_UPGRADE:
      ret = ComProto_FwUpgrade(data, txResp);
      break;

    // text log download
    case COM_PROTO_GET_TEXT:
      ret = ComProto_GetLog(data, txResp);
      break;

    default:
      break;
  }

  if (ret == STATUS_OK)
  {
    com.sentResp++;
  }

  return(ret);
}

/* Private Functions ---------------------------------------------------------*/

/* Arrived register write request */
static Status_t ComProto_WriteReg(uint8_t *data, uint8_t *txResp)
{
  Status_t ret = STATUS_OK;
  uint32_t countdown = PARSE_COUNTDOWN_VALUE;
  uint32_t reqIdx;
  uint32_t id;
  uint32_t sizeReg;
  uint32_t length;

  // get length
  length = ComProto_GetLength(data);

  // iterate until the end of request packet (countdown guard)
  reqIdx = COM_PROTO_HEADER_LEN;
  id = *((uint32_t *) (data + reqIdx));

  // write command guard according to login. When no permission

  while (reqIdx < length && countdown != 0)
  {
    id = *((uint32_t *) (data + reqIdx));
    memcpy(txResp + reqIdx, &id, sizeof(uint32_t));
    reqIdx += sizeof(uint32_t);

    sizeReg = CONF_BYTE_LEN_ID(id);

    // check the correctness of ID
    ret = Config_CheckLimits(id);

    if (ret == STATUS_OK)
    {
      // write new value into register storage
      memcpy(CONF_PTR(id), data + reqIdx, sizeReg);

      // copy register value into response
      memcpy(txResp + reqIdx, CONF_PTR(id), sizeReg);

      // apply received configuration
      ret = Config_ApplyConfig(id);
    }
    else
    {
      // id outside memory boundaries, return 0xF0 values
      memset(data + reqIdx, 0xF0, sizeReg);
    }

    reqIdx += sizeReg;
    countdown--;
  }

  if (countdown)
  {
    ret = STATUS_OK;
  }
  else
  {
    ret = STATUS_ERROR;
  }

  return(ret);
}


/* Arrived register read request */
static Status_t ComProto_ReadReg(uint8_t *data, uint8_t *txResp)
{
  Status_t ret = STATUS_OK;
  uint32_t countdown = PARSE_COUNTDOWN_VALUE;
  uint32_t respIdx;
  uint32_t reqIdx;
  uint32_t id;
  uint32_t sizeReg;
  uint32_t length;

  // get packet length
  length = ComProto_GetLength(data);

  // iterate until the end of request packet (countdown guard)
  reqIdx = COM_PROTO_HEADER_LEN;
  respIdx = COM_PROTO_HEADER_LEN;
  while ((reqIdx < length) && countdown != 0)
  {
    id = *((uint32_t *) (data + reqIdx));
    sizeReg = CONF_BYTE_LEN_ID(id);

    if (Config_CheckLimits(id) == STATUS_OK)
    {
      // copy Id to response
      memcpy(txResp + respIdx, &id, sizeof(uint32_t));
      respIdx += sizeof(uint32_t);

      // copy register value into response
      memcpy(txResp + respIdx, CONF_PTR(id), sizeReg);

      // increment values
      reqIdx += sizeof(uint32_t);
      respIdx += sizeReg;
    }
    else
    {
      // increment values
      reqIdx += sizeof(uint32_t);
    }

    countdown--;
  }
  txResp[2] = respIdx;
  txResp[3] = respIdx / 256;

  if (countdown)
  {
    ret = STATUS_OK;
  }
  else
  {
    ret = STATUS_ERROR;
  }

  return(ret);
}


/* Arrived firmware upgrade data chunk */
static Status_t ComProto_FwUpgrade(uint8_t *data, uint8_t *txResp)
{
  static uint32_t dev_id;
  static uint32_t bf_base = (uint32_t)CONF_C_APP_BUFFER_OFFSET;

  Status_t ret = STATUS_OK;
  uint16_t respIdx;
  uint16_t dataLength;
  uint32_t offset;
  uint32_t retCode = 0;
  uint32_t version;
  uint32_t date;
  uint32_t shift = 0;

  UNUSED(version);
  UNUSED(date);

  dataLength = *(uint16_t *) (data + 2);
  dataLength -= 8;
  offset = *(uint32_t *)(data + 4);

  // find the device ID
  if (offset == 0)
  {
    dev_id = *(uint32_t *)(data + 512 + 40);
    if (dev_id != CONF_TARGET_DEVICE) retCode = 1;  // wrong device ID
  }

  if (retCode == 0)
  {
    // zero offset within sector means we need to erase sector
    if (offset % FLASH_PAGE_SIZE == 0)
    {
      System_FlashErase(bf_base + offset, bf_base + offset);
    }

    if (dataLength == 0)
    {
      // verify and schedule restart only for TESTER fw image
      if (dev_id == CONF_TARGET_DEVICE)
      {
        // the very last packet, verify image
        ret = System_VerifyImage((uint32_t*)bf_base, dev_id);
        retCode = ret;
        if (ret == STATUS_OK) Control_ScheduleRestart();
      } else {
        retCode = STATUS_OK;
      }
    }
    else if (dataLength % 4 != 0)
    {
      // wrong length of data
      retCode = 2;
    }
    else
    {
      // check uint32 alignment
      if (((uint32_t)data % 4) != 0)
      {
        shift = ((uint32_t)data % 4);
        memcpy(data + 8 - shift, data + 8, dataLength);
      }

      // write data to flash
      retCode = System_FlashProgram((uint32_t)bf_base + offset, data + 8 - shift, dataLength);
    }
  }

  respIdx = 12;

  txResp[1] = COM_PROTO_FW_UPG_ACK;
  txResp[2] = respIdx;
  txResp[3] = respIdx / 256;

  *(uint32_t *)(txResp + 4) = offset + dataLength;
  *(uint32_t *)(txResp + 8) = retCode;

  return(ret);
}


/* Arrived text log download request */
static Status_t ComProto_GetLog(uint8_t *data, uint8_t *txResp)
{
  Status_t ret = STATUS_OK;

  // the message MUST end with '\n' to be valid to send
  if (com.log_buffer[com.ptr - 1] == '\n')
  {
    memcpy(txResp + 4, com.log_buffer, com.ptr);
    com.ptr += 4;
    txResp[1] = COM_PROTO_TEXT_RESP;
    txResp[2] = com.ptr;
    txResp[3] = com.ptr / 256;
    com.ptr = 0;
  } else {
    txResp[1] = COM_PROTO_TEXT_RESP;
    txResp[2] = 0;
    txResp[3] = 0;
    txResp[4] = 0;  // \0
  }

  return(ret);
}



/* ---------------------------------------------------------------------------*/

/** @} */
